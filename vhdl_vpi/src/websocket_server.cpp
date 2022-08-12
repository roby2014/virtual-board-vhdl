#include "websocket_server.hpp"
#include "board_config.hpp"
#include "pin.hpp"
#include "utils.hpp"
#include "virtual_board.hpp"

namespace ws_sv {

void open_ws_server(virtual_board* vb) {
    printf("Opening Websocket server...\n");
    try {
        // TODO: no hardcoded values
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

            // TODO: for now this handles only 1 ws conn
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

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
    // GET <PIN_ID> - returns pin's signal value (1/0)
    // PUT <PIN_ID> <VALUE> - puts pin's signal value to value

    boost::trim(buff);

    auto msg = utils::split(buff, ' '); // separates into tokens
    auto cmd = msg[0];

    if (cmd != "GET" && cmd != "PUT") {
        ws.write(net::buffer(std::string("Unknown command, try GET or PUT.\n")));
    }

    // GET <PIN_ID>
    if (cmd == "GET") {
        if (msg.size() != 2) {
            ws.write(
                net::buffer(std::string("Usage: GET <PIN_ID> - returns pin's signal value (1/0)")));
            return;
        }

        auto pin_id = msg[1];
        if (!vb->_pin_set.pin_exists(pin_id)) {
            ws.write(net::buffer(std::string("Unknown PIN_ID.")));
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
            ws.write(net::buffer(
                std::string("Usage: PUT <PIN_ID> <VALUE> - changes pin's signal value")));
            return;
        }

        auto pin_id = msg[1];
        if (!vb->_pin_set.pin_exists(pin_id)) {
            ws.write(net::buffer(std::string("Unknown PIN_ID.")));
            return;
        }

        auto value = msg[2] != "0";
        auto pin = vb->_pin_set.get_pin(pin_id);
        pin->set_value(value);
        printf("setting %s to %d\n", pin->id.c_str(), value);
    }
}

}; // namespace ws_sv