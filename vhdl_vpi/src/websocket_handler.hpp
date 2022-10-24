#ifndef WEBSOCKET_HANDLER_HPP
#define WEBSOCKET_HANDLER_HPP

#include "seasocks/WebSocket.h"
#include "utils.hpp"

class virtual_board;

namespace ws_sv_t {

using namespace seasocks;

class websocket_handler : public WebSocket::Handler {
public:
    /// stores the connected peers
    std::set<seasocks::WebSocket*> connections;
    virtual_board* vb;

    explicit websocket_handler(virtual_board* vb_ptr) : vb(vb_ptr) {}

    /// sends [data] message to all connected peers
    void send_all(const std::string& data) {
        for (const auto& c : connections)
            c->send(data);
    }

    // receive CHANGE PIN_ID VALUE event
    virtual void onData(WebSocket* conn, const char* data) override {
        std::string str = std::string(data);
        auto words = utils::split(str, ' ');

        // printf("on data %x\n", vb->_pin_set.get_pin("ola")); // ERROR HERE

        if (words.size() != 3 || words[0] != "CHANGE" || words[2] != "0" || words[2] != "1") {
            conn->send("Usage: CHANGE PIN_ID VALUE(1/0)");
            return;
        }
        /*
                         auto pin = vb->_pin_set.get_pin(words[1]);
                         if (pin == nullptr) {
                             conn->send("Invalid pin");
                             return;
                         }

                         printf("client want to change %s with index %s", pin->id, pin->index);
                */
    }

    virtual void onConnect(WebSocket* connection) override {
        connections.insert(connection);
    }

    virtual void onDisconnect(WebSocket* connection) override {
        connections.erase(connection);
    }
};

} // namespace ws_sv_t

#endif
