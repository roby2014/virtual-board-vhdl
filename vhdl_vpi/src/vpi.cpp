#include "vpi.hpp"
#include "board_config.hpp"
#include "qsf_parser.hpp"
#include "virtual_board.hpp"
#include "vpi_user.h"
#include <algorithm>
#include <unistd.h>

#define DEBUG

namespace vpi {

void cb_entry_point() {
    // create our virtual board
    virtual_board* vboard = new virtual_board();

    // register start of simulation callback
    s_cb_data cb = {.reason = cbStartOfSimulation,
                    .cb_rtn = &cb_simulation_start,
                    .user_data = (PLI_BYTE8*)vboard};

    vpiHandle callback_handle = vpi_register_cb(&cb);
    if (!callback_handle) {
        vpi_printf("\e[31mERROR: Cannot register cbStartOfSimulation call "
                   "back\e[0m\n");
        exit(EXIT_FAILURE);
    }
    vpi_free_object(callback_handle);

    // register end of simulation callback
    cb = {
        .reason = cbEndOfSimulation, .cb_rtn = &cb_simulation_end, .user_data = (PLI_BYTE8*)vboard};

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
    vpi_printf("DE10-Lite Virtual Board Emulator using VPI + GHDL\n");
    vpi_printf("Copyright (C) 2022 roby\n");
    vpi_printf("%s %s\n", info.product, info.version);
    vpi_printf("-------------------------------------------------\n");

    virtual_board* vb = new virtual_board();

    board_config::get_board_config();
    // qsf_parser parser("assets/pins.qsf");
    // get_io_signals(*vb, parser.parse());

    register_cb_after(cb_init, CLK_SPEED, vb);
    return 0;
}

PLI_INT32 cb_simulation_end(p_cb_data cb_data) {
    delete cb_data->user_data; // delete virtual board heap allocated
    vpi_printf("END OF SIMULATION\n");
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

void assign_net_to_hex(virtual_board* vb, vboard_assignments obj, std::size_t hex_idx,
                       std::size_t pin_idx) {
    switch (hex_idx) {
    case 0:
        vb->HEX0[pin_idx] = obj;
        break;
    case 1:
        vb->HEX1[pin_idx] = obj;
        break;
    case 2:
        vb->HEX2[pin_idx] = obj;
        break;
    case 3:
        vb->HEX3[pin_idx] = obj;
        break;
    case 4:
        vb->HEX4[pin_idx] = obj;
        break;
    case 5:
        vb->HEX5[pin_idx] = obj;
        break;

    default:
        break;
    }
}

void get_io_signals(virtual_board& vb, const std::vector<pin_assignment>& pin_assignments) {
    // get top entity
    vpiHandle iter = vpi_iterate(vpiModule, NULL);
    vpiHandle top = vpi_scan(iter);
    vpi_free_object(iter);
    vb.top_entity = std::string(vpi_get_str(vpiName, top));

    iter = vpi_iterate(vpiNet, top);
    if (!iter) {
        return;
    }

    // iterate net/signals
    vpiHandle net;
    while ((net = vpi_scan(iter))) {
        const char* net_name = vpi_get_str(vpiName, net);
        int net_width = vpi_get(vpiSize, net);
        int net_dir = vpi_get(vpiDirection, net); // 1 = input (SW), 2 = output (LEDR)

        // TODO: do we only care about Input/Output signals?
        if (net_dir != 1 && net_dir != 2)
            continue;

#ifdef DEBUG
        vpi_printf("\n%s[%d] = %d (dir=%d)\n", net_name, net_width, get_net_val(net), net_dir);
#endif

        // true if we current net is linked
        bool found = false;

        // loop our pin/qsf assignments
        // if pin's name and type are equal to current net/signal we are iterating
        // it means we can link them together in another structure (SW and LEDR)
        for (const auto& a : pin_assignments) {
            // FIXME: hex dont get linked because they are type 3 and net_dir is 2
            if (net_dir != a.pin_type || std::string(net_name) != a.net_name)
                continue;

            found = true;

            vboard_assignments obj = {.net = net, .index = a.index};
            if (a.pin_type == 1) {
                auto idx = qsf_parser::get_pin_index(a.pin_name, 1);
                vb.SW[idx] = obj;
#ifdef DEBUG
                vpi_printf("\tSW[%d] (%s) = %s[%ld] (%x)\n", idx, a.pin_name.c_str(),
                           a.net_name.c_str(), obj.index, obj.net);
#endif
            } else if (a.pin_type == 2) {
                if (qsf_parser::is_hex_pin(a.pin_name)) {
                    auto [hex_idx, pin_idx] = qsf_parser::get_hex_pin_index(a.pin_name);
                    assign_net_to_hex(&vb, obj, hex_idx, pin_idx);
#ifdef DEBUG
                    vpi_printf("\tHEX%d[%d] (%s) = %s[%ld] (%x)\n", hex_idx, pin_idx,
                               a.pin_name.c_str(), a.net_name.c_str(), obj.index, obj.net);
#endif
                } else {
                    auto idx = qsf_parser::get_pin_index(a.pin_name, 2);
                    vb.LEDR[idx] = obj;
#ifdef DEBUG
                    vpi_printf("\tLED[%d] (%s) = %s[%ld] (%x)\n", idx, a.pin_name.c_str(),
                               a.net_name.c_str(), obj.index, obj.net);
#endif
                }
            }
        }
        // net link not found, warn the user
        if (!found)
            vpi_printf("[VPI_WARNING] %s[%d] is not linked to any pin!\n", net_name, net_width);
    }

    vpi_free_object(top);
} // namespace vpi

PLI_INT32 cb_init(p_cb_data cb_data) {
    virtual_board* vboard = (virtual_board*)cb_data->user_data;
    static int startup_cnt = 0;

    switch (startup_cnt) {
    case 0:
        // register linked LEDR values change event
        for (std::size_t i = 0; i < vboard->LEDR.size(); i++) {
            if (vboard->LEDR[i].net) {
                // we also send the net index to the callback because
                // in arrays, all indices have the same net linked
                // so we need to send the net index so it only modifies the 1
                // bit that changed we also pass the led index to know which
                // LEDR[x] changed
                led_mapping* obj = new led_mapping{i, vboard->LEDR[i].index, vboard};
                register_ledr_value_change_cb(on_leds_value_change, vboard->LEDR[i].net, obj);
            }
        }

        // register linked HEX values change event
        for (std::size_t i = 0; i < vboard->HEX0.size(); i++) {
            if (vboard->HEX0[i].net)
                register_hex_value_change_cb(on_hex_value_change, vboard->HEX0[i].net,
                                             new hex_mapping{0, vboard->HEX0[i].index, vboard});
            if (vboard->HEX1[i].net)
                register_hex_value_change_cb(on_hex_value_change, vboard->HEX1[i].net,
                                             new hex_mapping{1, vboard->HEX1[i].index, vboard});

            if (vboard->HEX2[i].net)
                register_hex_value_change_cb(on_hex_value_change, vboard->HEX2[i].net,
                                             new hex_mapping{2, vboard->HEX2[i].index, vboard});

            if (vboard->HEX3[i].net)
                register_hex_value_change_cb(on_hex_value_change, vboard->HEX3[i].net,
                                             new hex_mapping{3, vboard->HEX3[i].index, vboard});

            if (vboard->HEX4[i].net)
                register_hex_value_change_cb(on_hex_value_change, vboard->HEX4[i].net,
                                             new hex_mapping{4, vboard->HEX4[i].index, vboard});

            if (vboard->HEX5[i].net)
                register_hex_value_change_cb(on_hex_value_change, vboard->HEX5[i].net,
                                             new hex_mapping{5, vboard->HEX5[i].index, vboard});
        }

        // set all inputs to 0 (switches)
        for (std::size_t i = 0; i < vboard->SW.size(); i++) {
            if (vboard->SW[i].net)
                set_net_val(vboard->SW[i].net, 0);
        }

        // FIXME: here for testing up_counter, this should obviously be handled
        // by the gui
        set_net_val(vboard->SW[1].net, 1); // enable

        register_cb_after(main_callback, CLK_SPEED, vboard);
        break;

    // TODO: do we need this?
    /* set clock to 1*/
    case 1:
        set_net_val(vboard->SW[0].net, 1);
        register_cb_after(cb_init, CLK_SPEED, vboard);
        break;

    /* set clock to 0, rstn to 1, register main callback */
    case 2:
        set_net_val(vboard->SW[0].net, 0);
        register_cb_after(main_callback, CLK_SPEED, vboard);
        break;

    default:
        vpi_printf("ERROR: should not enter this callback again!\n");
    }

    startup_cnt++;
    return 0;
}

PLI_INT32 main_callback(p_cb_data cb_data) {
    virtual_board* vboard = (virtual_board*)cb_data->user_data;
    // printf("main_callback\n");
    // printf("cout = %d\n", vpi::get_net_val(vboard->LED[3].net));
    // TODO: this works if clk is controlled via SW, what if not?

    if (get_net_val(vboard->SW[0].net) == 0) {
        printf("\nclk from 0 to 1\n");
        set_net_val(vboard->SW[0].net, 1);
    } else {
        printf("\nclk from 1 to 0\n");
        set_net_val(vboard->SW[0].net, 0);
    }

    printf("cout = %d\n", get_net_val(vboard->LEDR[4].net));

    // TODO: what should delay be?
    vpi::register_cb_after(main_callback, 1, vboard);
    printf("press enter for next clock\n");
    getchar();
    return 0;
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

void register_ledr_value_change_cb(PLI_INT32 (*cb_rtn)(struct t_cb_data*), vpiHandle net,
                                   led_mapping* obj) {
    s_vpi_time tim = {.type = vpiSuppressTime};
    s_vpi_value val = {.format = vpiSuppressVal};
    s_cb_data cb = {.reason = cbValueChange,
                    .cb_rtn = cb_rtn,
                    .obj = net,
                    .time = &tim,
                    .value = &val,
                    .user_data = (PLI_BYTE8*)obj};

    vpiHandle callback_handle = vpi_register_cb(&cb);
    if (!callback_handle)
        vpi_printf("VPI_ERROR: Cannot register cbValueChange LEDR callback!\n");
    vpi_free_object(callback_handle);
}

PLI_INT32 on_leds_value_change(p_cb_data cb_data) {
    led_mapping* obj = (led_mapping*)cb_data->user_data;
    vpiHandle net = (vpiHandle)cb_data->obj;
    // FIXME: when a LEDR changes , this call back is triggered
    // the problem is that if the net is an array, it also triggers all the
    // indices/nets from the array because it thinks the net changed all, but it
    // didnt. for now we can keep it like this, but in the future we could optimize
    // and try to trigger this only by the bit that changed
    int val = get_bit_net(net, obj->net_idx);

#ifdef DEBUG
    printf("LEDR[%ld] (net[%ld]) changed to %d\n", obj->pin_idx, obj->net_idx, val);
#endif // DEBUG

    // char script[32];
    // snprintf(script, sizeof(script), "jsSetLed('%d', %d)", obj->pin_idx, val);
    // std::string buffAsStdStr = script;
    // TODO: call vboard-> overlay_->view()->EvaluateScript(script);
    return 0;
}

void register_hex_value_change_cb(PLI_INT32 (*cb_rtn)(struct t_cb_data*), vpiHandle net,
                                  hex_mapping* obj) {
    s_vpi_time tim = {.type = vpiSuppressTime};
    s_vpi_value val = {.format = vpiSuppressVal};
    s_cb_data cb = {.reason = cbValueChange,
                    .cb_rtn = cb_rtn,
                    .obj = net,
                    .time = &tim,
                    .value = &val,
                    .user_data = (PLI_BYTE8*)obj};

    vpiHandle callback_handle = vpi_register_cb(&cb);
    if (!callback_handle)
        vpi_printf("VPI_ERROR: Cannot register cbValueChange HEX callback!\n");
    vpi_free_object(callback_handle);
}

PLI_INT32 on_hex_value_change(p_cb_data cb_data) {
    hex_mapping* obj = (hex_mapping*)cb_data->user_data;
    vpiHandle net = (vpiHandle)cb_data->obj;
    int val = get_bit_net(net, obj->net_idx);

#ifdef DEBUG
    printf("HEX%ld (net[%ld]) changed to %d\n", obj->hex_idx, obj->net_idx, val);
#endif

    // TODO: call js fn that changes hex value on the gui
    return 0;
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