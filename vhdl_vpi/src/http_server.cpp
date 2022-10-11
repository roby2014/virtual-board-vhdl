#include "http_server.hpp"
#include "virtual_board.hpp"
#include "vpi.hpp"
#include "httplib.h"

namespace http_sv {

void open_http_server(virtual_board* vb) {
    char in_signal_name[128], out_signal_name[128];
    snprintf(in_signal_name, sizeof(in_signal_name), "%s.inputport_sw", vb->_top_ent_name.c_str());
    snprintf(out_signal_name, sizeof(out_signal_name), "%s.outputport_sw",
             vb->_top_ent_name.c_str());
    if (vpi_handle_by_name(in_signal_name, NULL) == nullptr ||
        vpi_handle_by_name(out_signal_name, NULL) == nullptr) {
        // we dont wanna run a HTTP server if top entity does not have UsbPort_SW ports..
        vpi_printf("WARNING: Current project does not support UsbPort via VPI+GHDL (http server "
                   "not running)");
        return;
    }

    httplib::Server svr;

    svr.Get("/usbport/inputport_sw",
            [in_signal_name](const httplib::Request&, httplib::Response& res) {
                auto input_port_ptr = vpi_handle_by_name((PLI_BYTE8*)in_signal_name, NULL);
                auto val = vpi::get_net_val(input_port_ptr);
                res.set_content(std::to_string(val), "text/plain");
            });

    svr.Put("/usbport/outputport_sw",
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

    printf("Opening HTTP server... ( %s:%d )\n", vb->host.c_str(), vb->http_port);
    svr.listen(vb->host, vb->http_port);
}

} // namespace http_sv
