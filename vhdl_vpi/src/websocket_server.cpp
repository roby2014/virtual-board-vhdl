#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>

#include "pin.hpp"
#include "utils.hpp"
#include "virtual_board.hpp"
#include "websocket_server.hpp"
#include "vpi.hpp"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

/* FUNCTION DECLARATION */
// declared here and not in the header because we dont need them in other files

/// handles socket connection on a separate thread
void do_session(tcp::socket socket, virtual_board* vb);

/// handle ws message
void handle_ws_msg(websocket::stream<tcp::socket>& ws, std::string& buff, virtual_board* vb);

namespace ws_sv {

void open_ws_server(virtual_board* vb) {
    printf("Opening Websocket server... ( ws://%s:%d )\n", vb->host.c_str(), vb->ws_port);
    try {
        // The io_context is required for all I/O
        net::io_context ioc{1};

        // The acceptor receives incoming connections
        tcp::acceptor acceptor{
            ioc, {net::ip::make_address(vb->host), (boost::asio::ip::port_type)vb->ws_port}};
        for (;;) {
            // This will receive the new connection
            tcp::socket socket{ioc};

            // Block until we get a connection
            acceptor.accept(socket);

            // Launch the session, transferring ownership of the socket
            std::thread(&do_session, std::move(socket), vb).detach();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

}; // namespace ws_sv

/* FUNCTION DEFINITION */

void do_session(tcp::socket socket, virtual_board* vb) {
    try {
        // Construct the stream by moving in the socket
        websocket::stream<tcp::socket> ws{std::move(socket)};

        // Set a decorator to change the Server of the handshake
        ws.set_option(websocket::stream_base::decorator([](websocket::response_type& res) {
            res.set(http::field::server,
                    std::string(BOOST_BEAST_VERSION_STRING) + " websocket-server-sync");
        }));

        // Accept the websocket handshake
        ws.accept();

        printf("New websocket connection...\n");

        printf("entra no pipe\n");
        for (;;) {
            // FIXME: websocket should announce when a ping changed...
        }
    } catch (beast::system_error const& se) {
        // This indicates that the session was closed
        if (se.code() != websocket::error::closed)
            std::cerr << "Websocket error: " << se.code().message() << std::endl;
    } catch (std::exception const& e) {
        std::cerr << "Websocket error: " << e.what() << std::endl;
    }
}

void handle_ws_msg(websocket::stream<tcp::socket>& ws, std::string& buff, virtual_board* vb) {
    // "communication protocol":
    // HELP - prints communication protocol info
    // GET <PIN_ID> - returns <PIN_ID> signal value (1/0)
    // PUT <PIN_ID> <VALUE> - changes <PIN_ID> signal value to <VALUE>

    // Parse message (can be done a lot better)
    boost::trim(buff);
    auto msg = utils::split(buff, ' '); // separates into tokens
    auto cmd = msg[0];

    // HELP
    if (cmd == "HELP") {
        ws.write(
            net::buffer("\nGET <PIN_ID> - returns <PIN_ID> signal value (1/0)"
                        "\nPUT <PIN_ID> <VALUE> - changes <PIN_ID> signal value to <VALUE>"
                        "\nUGET - returns UsbPort input port 7 bit value"
                        "\nUPUT <7bit_val> - changes UsbPort output port value to <7bit_value>\n"));
        return;
    }

    // GET <PIN_ID>
    if (cmd == "GET") {
        if (msg.size() != 2) {
            ws.write(net::buffer("Usage: GET <PIN_ID> - returns <PIN_ID> signal value (1/0)"));
            return;
        }

        auto pin_id = msg[1];
        if (!vb->_pin_set.pin_exists(pin_id)) {
            ws.write(net::buffer("Unknown PIN_ID"));
            return;
        }

        char response[64];
        snprintf(response, sizeof(response), "%s = %d", pin_id.c_str(),
                 vb->_pin_set.get_pin_value(pin_id));
        ws.write(net::buffer(std::string(response)));
        return;
    }

    // PUT <PIN_ID> <VALUE>
    if (cmd == "PUT") {
        if (msg.size() != 3) {
            ws.write(net::buffer(
                "Usage: PUT <PIN_ID> <VALUE> - changes <PIN_ID> signal value to <VALUE>"));
            return;
        }

        auto pin_id = msg[1];
        if (!vb->_pin_set.pin_exists(pin_id)) {
            ws.write(net::buffer("Unknown PIN_ID"));
            return;
        }

        auto value = msg[2] != "0";
        auto p = vb->_pin_set.get_pin_net(pin_id);
        // vb->_events.push(board_event{p, value});
        return;
    }

    // UGET
    if (cmd == "UGET") {
        char signal_name[128];
        snprintf(signal_name, sizeof(signal_name), "%s.inputport_sw", vb->_top_ent_name.c_str());
        auto input_port_ptr = vpi_handle_by_name(signal_name, NULL);
        if (input_port_ptr == nullptr) {
            ws.write(net::buffer(
                "Current project does not support UsbPort via VPI+GHDL (inputport_sw not found)"));
            return;
        }

        char response[64];
        snprintf(response, sizeof(response), "%d", vpi::get_net_val(input_port_ptr));
        ws.write(net::buffer(std::string(response)));
        return;
    }

    // UPUT <7bit_value>
    if (cmd == "UPUT") {
        if (msg.size() != 2) {
            ws.write(net::buffer("Usage: UPUT <7bit_value> <VALUE> - changes UsbPort "
                                 "output values to <7bit_value>"));
            return;
        }

        auto val = std::atoi(msg[1].c_str());
        if (val > 0b1111111) {
            ws.write(net::buffer("Value bigger than 7 bits"));
            return;
        }

        char signal_name[128];
        snprintf(signal_name, sizeof(signal_name), "%s.outputport_sw", vb->_top_ent_name.c_str());
        auto output_port_ptr = vpi_handle_by_name(signal_name, NULL);
        if (output_port_ptr == nullptr) {
            ws.write(net::buffer(
                "Current project does not support UsbPort via VPI+GHDL (outputport_sw not found)"));
            return;
        }

        vpi::set_net_val(output_port_ptr, val);
        return;
    }

    ws.write(net::buffer("Unknown command, try 'HELP'\n"));
    return;
}
