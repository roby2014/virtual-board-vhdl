#ifndef WEBSOCKET_HANDLER_HPP
#define WEBSOCKET_HANDLER_HPP

#include "seasocks/WebSocket.h"

class virtual_board;

namespace ws_sv {

using namespace seasocks;

class websocket_handler : public WebSocket::Handler {
public:
    /// stores the connected peers
    std::set<seasocks::WebSocket*> connections;
    virtual_board* vb;

    explicit websocket_handler(virtual_board* vb_ptr);

    /// sends [data] message to all connected peers
    void send_all(const std::string& data);

    // receive "CHANGE pinId VALUE" event
    virtual void onData(WebSocket* conn, const char* data) override;

    // called when peer connects
    virtual void onConnect(WebSocket* connection) override;

    // called when peer disconnects
    virtual void onDisconnect(WebSocket* connection) override;
};

} // namespace ws_sv

#endif
