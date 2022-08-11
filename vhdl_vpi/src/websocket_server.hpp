#ifndef WEBSOCKET_SERVER_HPP
#define WEBSOCKET_SERVER_HPP

#include "virtual_board.hpp"
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>

namespace ws_sv {

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

/// EXITS if error on creating the websocket server
void open_ws_server(virtual_board* vb);

/// handles socket connection on a separate thread
void do_session(tcp::socket socket, virtual_board* vb);

/// handle ws message
void handle_ws_msg(websocket::stream<tcp::socket>& ws, std::string& buff, virtual_board* vb);

} // namespace ws_sv

#endif
