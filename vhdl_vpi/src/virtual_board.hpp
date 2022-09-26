#ifndef VIRTUAL_BOARD_HPP
#define VIRTUAL_BOARD_HPP

#include <string>
#include <memory>
#include "pin_set.hpp"
#include "websocket_handler.hpp"

#include "seasocks/Server.h"
#include "seasocks/PrintfLogger.h"
#include "cpp-httplib/httplib.h"

/// Represents the virtual board
class virtual_board {
public:
    std::string _top_ent_name;
    pin_set _pin_set;

    // TODO: get these via config file on constructor
    std::string host;
    int ws_port;
    int http_port;

    /// Handler to the http server
    httplib::Server http_sv;

    /// Server & handler to the websocket server
    // TODO: logger should not be static, but also should be read from file like host
    seasocks::Server ws_sv{std::make_shared<seasocks::PrintfLogger>(seasocks::Logger::Level::Info)};
    std::shared_ptr<ws_sv::websocket_handler> handler;

    /// Websocket connections
    int _peers_count;

    /// Ctor
    virtual_board();

    /// Opens the websocket server, in a separate thread
    void open_ws_server();

    /// Opens the HTTP server, in a separate thread
    void open_http_server();

    /// Debugs board's pin set
    void debug() const;
};

#endif