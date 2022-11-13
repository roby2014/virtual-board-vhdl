#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <thread>
#include "vpi.hpp"
#include "board_config.hpp"
#include "virtual_board.hpp"

#define DEBUG

namespace vpi {

void cb_entry_point() {
    // register start of simulation callback
    s_cb_data cb = {
        .reason = cbStartOfSimulation,
        .cb_rtn = &cb_simulation_start,
    };

    vpiHandle callback_handle = vpi_register_cb(&cb);
    if (!callback_handle) {
        vpi_printf("\e[31mERROR: Cannot register cbStartOfSimulation call "
                   "back\e[0m\n");
        exit(EXIT_FAILURE);
    }
    vpi_free_object(callback_handle);

    // register end of simulation callback
    cb = {.reason = cbEndOfSimulation, .cb_rtn = &cb_simulation_end};

    callback_handle = vpi_register_cb(&cb);
    if (!callback_handle) {
        vpi_printf("\e[31mERROR: Cannot register cbEndOfSimulation call back\e[0m\n");
        exit(EXIT_FAILURE);
    }
    vpi_free_object(callback_handle);
}

PLI_INT32 cb_simulation_start(p_cb_data cb_data __attribute__((unused))) {
    s_vpi_vlog_info info;
    vpi_get_vlog_info(&info);
    vpi_printf("-------------------------------------------------\n");
    vpi_printf("VHDL Simulator via Virtual Board using VPI + GHDL\n");
    vpi_printf("Copyright (C) 2022 roby\n");
    vpi_printf("%s %s\n", info.product, info.version);
    vpi_printf("-------------------------------------------------\n");

    // get board pins
    auto board_pins = board_config::get_board_config();

    // parse custom pin assignments
    std::ifstream ifs("assets/assignments.cfg");
    if (!ifs.is_open()) {
        fprintf(stderr, "Assignments config @ I/O error while reading file, does "
                        "'assets/assignments.cfg' exist?. \n");
        exit(-1);
    }

    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    assignments_cfg::lexer lexer;
    assignments_cfg::parser parser;
    auto tokens = lexer.get_tokens(content);
    auto result = parser.parse(tokens);

#ifdef DEBUG
    // lexer.debug_tokens(tokens);
    parser.debug_assignments(result);
#endif

    if (result.size() == 0) {
        vpi_printf("VPI_WARNING: 'assets/assignments.cfg' DOES NOT CONTAIN ANY PIN ASSIGNMENT.\n");
    }

    // check if pins from assignments file are valid
    for (const auto& assignment : result) {
        if (!board_config::pin_exists(board_pins, assignment.pin)) {
            vpi_printf("VPI_ERROR: PIN \"%s\" does not exist in your board config.\n",
                       assignment.pin.c_str());
            exit(EXIT_FAILURE);
        }
        // TODO: is this correct
        if (assignment.signal_dst == "outputport_sw" || assignment.signal_dst == "inputport_sw") {
            vpi_printf("VPI_ERROR: PIN assigning UsbPorts (outputport_sw/inputport_sw) is not "
                       "permitted.\n");
            exit(EXIT_FAILURE);
        }
    }

    virtual_board* vb = new virtual_board();
    vb->_pin_set = get_pins_signals(*vb, result);

#ifdef DEBUG
    vb->debug();
#endif

    register_cb_after(cb_init, CLK_SPEED, vb);
    return 0;
}

PLI_INT32 cb_simulation_end(p_cb_data cb_data) {
    (void)cb_data; // unused parameter compiler warning
    vpi_printf("END OF SIMULATION\n");
    return 0;
}

PLI_INT32 cb_init(p_cb_data cb_data) {
    virtual_board* vb = (virtual_board*)cb_data->user_data;

    // open websocket & http server in a separate thread
    vb->open_ws_server();
    vb->open_http_server();

    // register linked pins values change event
    for (auto& p : vb->_pin_set.pins) {
        register_pins_value_change_cb(on_pins_value_change, &p, vb);
    }

    // register main callback (loop)
    register_cb_after(main_callback, CLK_SPEED, vb);

    return 0;
}

PLI_INT32 main_callback(p_cb_data cb_data) {
    static bool test = false; // testing purposes
    virtual_board* vb = (virtual_board*)cb_data->user_data;

    // printf("press enter for next clock\n");
    // getchar();

    set_net_val(vpi_handle_by_name("up_counter.clk", NULL), test);
    // set_net_val(vb->_pin_set.get_pin_net("clk"), test);

    //printf("Setting clock to %d, cout value = %d\n", test,
           //get_net_val(vpi_handle_by_name("up_counter.cout", NULL)));

    test = !test;

    // printf("dummy = %d\n", get_net_val(vpi_handle_by_name("up_counter.dummy", NULL)));
    printf("cout = %d\t", get_net_val(vpi_handle_by_name("up_counter.cout", NULL)));
    //printf("inp = %d\n", get_net_val(vpi_handle_by_name("up_counter.inputport_sw", NULL)));
    // set_net_val(vpi_handle_by_name("up_counter.outputport_sw", NULL), 15);

    sleep(1);
    // getchar();
    check_error();
    register_cb_after(main_callback, 1, vb);
    return 0;
}

/// wrapper to pass in to leds_change_callback
/// pin is needed because we need to know what INDEX changed (in case of array)
typedef struct vb_pin_t {
    virtual_board* vb;
    pin* p;
} vb_pin_t;

void register_pins_value_change_cb(PLI_INT32 (*cb_rtn)(struct t_cb_data*), pin* p,
                                   virtual_board* vb) {
    s_vpi_time tim = {.type = vpiSuppressTime};
    s_vpi_value val = {.format = vpiSuppressVal};
    s_cb_data cb = {.reason = cbValueChange,
                    .cb_rtn = cb_rtn,
                    .obj = p->net, // trigger callback when p->net value changes!
                    .time = &tim,
                    .value = &val,
                    // we will need this memory the whole program, so its ok mallocing
                    .user_data = (PLI_BYTE8*)new vb_pin_t{.vb = vb, .p = p}};

    vpiHandle callback_handle = vpi_register_cb(&cb);
    if (!callback_handle)
        vpi_printf("VPI_ERROR: Cannot register cbValueChange callback!\n");
    vpi_free_object(callback_handle);
}

PLI_INT32 on_pins_value_change(p_cb_data cb_data) {
    vb_pin_t* vb_pin = (vb_pin_t*)cb_data->user_data;
    virtual_board* vb = vb_pin->vb;
    //vb_pin->p->debug_pin();

    if (vb->handler->connections.size() < 1) {
        // if no peers listening, no point in announce anything..
        return 0;
    }

    // auto pin = vb->_pin_set.get_pin(cb_data->obj);
    // NOTE: this would return the pin with index 0, thats why we are using vb_pin_t here
    // so we can know what index changed, and send the PIN linked to that index

    pin* pin = vb_pin->p;

    // broadcast to all websocket connections that pin changed
    vb->ws_sv.execute([vb, pin] {
        char msg[256];
        snprintf(msg, sizeof(msg), "%s = %d", pin->id.c_str(), pin->get_value());
        vb->handler->send_all(msg);
    });

    return 0;
}

int get_net_val(vpiHandle net) {
    int ret = 0;
    int size = vpi_get(vpiSize, net);
    s_vpi_value val = {.format = vpiBinStrVal};
    vpi_get_value(net, &val);
    for (int i = 0; i < size; i++) {
        ret <<= 1;
        if (val.value.str[i] == '1')
            ret |= 1;
    }
    return ret;
}

void set_net_val(vpiHandle net, int value) {
    if (!net)
        return;
    s_vpi_value val{.format = vpiIntVal, .value = {.integer = value}};
    vpi_put_value(net, &val, NULL, vpiNoDelay);
    check_error();
}

bool get_bit_net(vpiHandle net, std::size_t idx) {
    if (idx >= (std::size_t)vpi_get(vpiSize, net))
        return false;

    s_vpi_value val{.format = vpiBinStrVal};
    vpi_get_value(net, &val);

    std::string str = std::string(val.value.str);
    std::reverse(str.begin(), str.end()); // make it into little endian
    return str[idx] == '1';
}

s_vpi_time double_to_vpi_time(double t, double time_resolution) {
    uint64_t simtime = (uint64_t)(t * time_resolution);
    s_vpi_time ts = {
        .type = vpiSimTime,
        .high = (uint32_t)(simtime >> 32),
        .low = (uint32_t)(simtime & 0xFFFFFFFFUL),
    };
    return ts;
}

pin_set get_pins_signals(virtual_board& vb,
                         const std::vector<assignments_cfg::pin_assignment>& assignments) {
    pin_set pin_set_;

    // get top entity
    vpiHandle iter = vpi_iterate(vpiModule, NULL);
    vpiHandle top = vpi_scan(iter);
    vpi_free_object(iter);
    vb._top_ent_name = std::string(vpi_get_str(vpiName, top));

    iter = vpi_iterate(vpiNet, top);
    if (!iter) {
        vpi_printf("VPI_ERROR: Returning early from get_io_signals.\n");
        return pin_set_;
    }

    std::size_t total_pins_found = 0, total_signals = 0;

    // iterate net/signals
    vpiHandle net;
    while ((net = vpi_scan(iter))) {
        const char* net_name = vpi_get_str(vpiName, net);
        int net_width = vpi_get(vpiSize, net);
        int net_dir = vpi_get(vpiDirection, net); // 1 = input (SW), 2 = output (LEDR)

#ifdef DEBUG
        printf("\t [DBG all signals] %s %d %d\n", net_name, net_width, net_dir);
#endif
        // TODO: do we only care about Input/Output signals?
        if (net_dir != 1 && net_dir != 2)
            continue;

        total_signals++;

#ifdef DEBUG
        printf("\t [top entity signal debug] %s[%d] = %d (dir=%d) at %x\n", net_name, net_width,
               get_net_val(net), net_dir, net);
#endif

        // loop pin assignments
        for (const auto& a : assignments) {
            if (a.signal_dst == std::string(net_name)) {
                if (a.index >= (std::size_t)net_width) {
                    vpi_printf("VPI_ERROR: \"%s[%ld]\" - \"%s\" signal is only %ld bit(s) long.\n",
                               net_name, a.index, net_name, net_width);
                    exit(EXIT_FAILURE);
                }

                pin p{.id = a.pin, .net = net, .net_name = net_name, .index = a.index};
                if (pin_set_.pin_exists(p)) {
                    vpi_printf("VPI_ERROR: \"%s\" is already linked to another signal.\n",
                               a.pin.c_str());
                    exit(EXIT_FAILURE);
                }

                pin_set_.add_pin(p);
                total_pins_found++;
            }
        }
    }

    if (total_pins_found != total_signals) {
        vpi_printf("VPI_WARNING: THERE ARE SIGNALS THAT ARE NOT CONNECTED TO ANY PIN "
                   "(connected/total) = (%ld/%ld).\n",
                   total_pins_found, total_signals);
    }

    vpi_free_object(top);
    return pin_set_;
}

void register_cb_after(PLI_INT32 (*cb_rtn)(struct t_cb_data*), double delay,
                       virtual_board* vboard) {
    s_vpi_time time = double_to_vpi_time(delay, delay);
    s_cb_data cb = {.reason = cbAfterDelay,
                    .cb_rtn = cb_rtn,
                    .obj = NULL,
                    .time = &time,
                    .value = NULL,
                    .user_data = (PLI_BYTE8*)vboard};

    vpiHandle callback_handle = vpi_register_cb(&cb);
    if (!callback_handle) {
        vpi_printf("VPI_ERROR: Cannot register cbAfterDelay\n");
        exit(EXIT_FAILURE);
    }
    vpi_free_object(callback_handle);
}

void register_cb_at_last_known_delta(PLI_INT32 (*cb_rtn)(struct t_cb_data*),
                                     virtual_board* vboard) {

    s_vpi_time time = {.type = vpiSimTime, .high = 0, .low = 0, .real = 0.0};
    s_cb_data cb = {.reason = cbReadWriteSynch,
                    .cb_rtn = cb_rtn,
                    .obj = NULL,
                    .time = &time,
                    .value = NULL,
                    .user_data = (PLI_BYTE8*)vboard};

    vpiHandle callback_handle = vpi_register_cb(&cb);
    if (!callback_handle)
        vpi_printf("VPI_ERROR: Cannot register cbReadWriteSynch callback!\n");

    vpi_free_object(callback_handle);
}

bool check_error() {
    s_vpi_error_info err;
    if (!vpi_chk_error(&err))
        return false;

    if (err.level == vpiError) {
        vpi_printf("VPI_ERROR: %s\n", err.message);
        return true;
    }

    vpi_printf("VPI_MESSAGE: %s\n", err.message);
    return false;
}

} // namespace vpi