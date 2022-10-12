#ifndef WEBSOCKET_SERVER_HPP
#define WEBSOCKET_SERVER_HPP

#include "seasocks/WebSocket.h"

namespace ws_sv {

using namespace seasocks;

class websocket_handler : public WebSocket::Handler {
public:
    // stores the connected peers
    std::set<seasocks::WebSocket*> connections;

    // sends [data] message to all connected peers
    void send_all(const std::string& data) {
        for (const auto& c : connections)
            c->send(data);
    }

    virtual void onConnect(WebSocket* connection) override {
        connections.insert(connection);
    }

    virtual void onDisconnect(WebSocket* connection) override {
        connections.erase(connection);
    }
};

} // namespace ws_sv

#endif
