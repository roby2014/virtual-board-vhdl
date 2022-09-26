#include "virtual_board.hpp"
#include <thread>
#include "vpi.hpp"

virtual_board::virtual_board() {
    host = "0.0.0.0";
    ws_port = 8083;
    http_port = 8080;
}

void virtual_board::open_ws_server() {
    handler = std::make_shared<ws_sv::websocket_handler>(this);
    ws_sv.addWebSocketHandler("/", handler);
    std::thread([this]() { this->ws_sv.serve(this->host.c_str(), this->ws_port); }).detach();
}

void virtual_board::open_http_server() {
    char in_signal_name[128], out_signal_name[128];
    snprintf(in_signal_name, sizeof(in_signal_name), "%s.inputport_sw", _top_ent_name.c_str());
    snprintf(out_signal_name, sizeof(out_signal_name), "%s.outputport_sw", _top_ent_name.c_str());
    if (vpi_handle_by_name(in_signal_name, NULL) == nullptr ||
        vpi_handle_by_name(out_signal_name, NULL) == nullptr) {
        // we dont wanna run a HTTP server if top entity does not have UsbPort_SW ports..
        vpi_printf("WARNING: Current project does not support UsbPort via VPI+GHDL (http server "
                   "not running)");
        return;
    }

    http_sv.Get("/usbport/inputport_sw",
                [in_signal_name](const httplib::Request&, httplib::Response& res) {
                    auto input_port_ptr = vpi_handle_by_name((PLI_BYTE8*)in_signal_name, NULL);
                    auto val = vpi::get_net_val(input_port_ptr);
                    res.set_content(std::to_string(val), "text/plain");
                });

    http_sv.Put("/usbport/outputport_sw",
                [out_signal_name](const httplib::Request& req, httplib::Response& res) {
                    if (!req.has_param("value")) {
                        res.set_content("<value> parameter missing", "text/plain");
                        return;
                    }

                    auto value = std::atoi(req.get_param_value("value").c_str());
                    if (value > 0b1111111) {
                        res.set_content("Value bigger than 7 bits", "text/plain");
                        return;
                    }

                    auto output_port_ptr = vpi_handle_by_name((PLI_BYTE8*)out_signal_name, NULL);
                    vpi::set_net_val(output_port_ptr, value);
                    res.set_content(std::to_string(value), "text/plain");
                });

    printf("Opening HTTP server... ( http://%s:%d )\n", host.c_str(), http_port);
    std::thread([this]() { this->http_sv.listen(this->host, this->http_port); }).detach();
}

void virtual_board::debug() const {
    printf("[debug PIN SET] start\n");
    _pin_set.debug_pin_set();
    printf("[debug PIN SET] end\n");
}