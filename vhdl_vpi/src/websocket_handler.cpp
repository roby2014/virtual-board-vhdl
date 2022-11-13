#include "websocket_handler.hpp"
#include "virtual_board.hpp"
#include "vpi.hpp"
#include "utils.hpp"

namespace ws_sv {

websocket_handler::websocket_handler(virtual_board* vb_ptr) : vb(vb_ptr) {}

void websocket_handler::send_all(const std::string& data) {
    for (const auto& c : connections)
        c->send(data);
}

void websocket_handler::onData(WebSocket* conn, const char* data) {
    std::string str = std::string(data);
    auto words = utils::split(str, ' ');

    if (words.size() != 3 || words[0] != "CHANGE" || (words[2] != "0" && words[2] != "1")) {
        conn->send("[ERROR] Usage: CHANGE pinId VALUE(1/0)");
        return;
    }

    auto pin = vb->_pin_set.get_pin(words[1]);
    if (pin == nullptr) {
        conn->send("[ERROR] Invalid pin");
        return;
    }

    printf("chegou aqui\n");

    // NOTE: this is the WHOLE net value
    // we have to do some tricky bit stuff because the net can be an array
    // so setting a net to X value, might change the whole array
    auto net_val = vpi::get_net_val(pin->net);

    // so we do XOR to only activate/deactivate the bit index we want
    auto mask = 1 << pin->index;
    auto new_value = net_val ^ mask;
    vpi::set_net_val(pin->net, new_value); // TODO: should we worry about sign?
}

void websocket_handler::onConnect(WebSocket* connection) {
    connections.insert(connection);
}

void websocket_handler::onDisconnect(WebSocket* connection) {
    connections.erase(connection);
}

} // namespace ws_sv