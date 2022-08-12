#ifndef WEBSOCKET_SERVER_HPP
#define WEBSOCKET_SERVER_HPP

class virtual_board;

namespace ws_sv {

/// EXITS if error on creating the websocket server
void open_ws_server(virtual_board* vb);

} // namespace ws_sv

#endif
