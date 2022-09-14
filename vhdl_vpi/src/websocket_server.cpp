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

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

/* FUNCTION DECLARATION */

/// handles socket connection on a separate thread
void do_session(tcp::socket socket, virtual_board* vb);

/// handle ws message
void handle_ws_msg(websocket::stream<tcp::socket>& ws, std::string& buff, virtual_board* vb);

namespace ws_sv {

void open_ws_server(virtual_board* vb) {
    printf("Opening Websocket server... ( ws://127.0.0.1:8083 )\n");
    try {
        // TODO: no hardcoded values (make it via cfg file?)
        auto const address = net::ip::make_address("127.0.0.1");
        auto const port = 8083;

        // The io_context is required for all I/O
        net::io_context ioc{1};

        // The acceptor receives incoming connections
        tcp::acceptor acceptor{ioc, {address, port}};
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

        printf("New websocket connection, waiting for messages ...\n");

        for (;;) {
            // This buffer will hold the incoming message
            beast::flat_buffer buffer;
            // Blocks and waits for message
            ws.read(buffer);

            // Echo the message back
            ws.text(ws.got_text());

            auto a = beast::buffers_to_string(buffer.data());
            handle_ws_msg(ws, a, vb);
        }
    } catch (beast::system_error const& se) {
        // This indicates that the session was closed
        if (se.code() != websocket::error::closed)
            std::cerr << "Error: " << se.code().message() << std::endl;
    } catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
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

    if (cmd != "GET" && cmd != "PUT" && cmd != "HELP") {
        ws.write(net::buffer(std::string("Unknown command, try GET or PUT.\n")));
        return;
    }

    // HELP
    if (cmd == "HELP") {
        ws.write(net::buffer(
            std::string("\nGET <PIN_ID> - returns <PIN_ID> signal value (1/0)"
                        "\nPUT <PIN_ID> <VALUE> - changes <PIN_ID> signal value to <VALUE>\n")));
        return;
    }

    // GET <PIN_ID>
    if (cmd == "GET") {
        if (msg.size() != 2) {
            ws.write(net::buffer(
                std::string("Usage: GET <PIN_ID> - returns <PIN_ID> signal value (1/0)")));
            return;
        }

        auto pin_id = msg[1];
        if (!vb->_pin_set.pin_exists(pin_id)) {
            ws.write(net::buffer(std::string("Unknown PIN_ID")));
            return;
        }

        char response[64];
        snprintf(response, sizeof(response), "%s = %d", pin_id.c_str(),
                 vb->_pin_set.get_pin_value(pin_id));
        ws.write(net::buffer(std::string(response)));
    }

    // PUT <PIN_ID> <VALUE>
    if (cmd == "PUT") {
        if (msg.size() != 3) {
            ws.write(net::buffer(std::string(
                "Usage: PUT <PIN_ID> <VALUE> - changes <PIN_ID> signal value to <VALUE>")));
            return;
        }

        auto pin_id = msg[1];
        if (!vb->_pin_set.pin_exists(pin_id)) {
            ws.write(net::buffer(std::string("Unknown PIN_ID")));
            return;
        }

        auto value = msg[2] != "0";
        auto pin = vb->_pin_set.get_pin_net(pin_id);
        vb->_events.push(board_event{pin, value});
    }
}
