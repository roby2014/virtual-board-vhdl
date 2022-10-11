#ifndef VIRTUAL_BOARD_HPP
#define VIRTUAL_BOARD_HPP

#include <string>
#include <queue>
#include "pin_set.hpp"

/// Represents a pin value change
/// Stores a pointer to the pin that changed, and its new value
typedef struct event {
    pin* pin_ptr;
    bool new_value;
} board_event;

/// Represents the virtual board
class virtual_board {
public:
    std::string _top_ent_name;
    pin_set _pin_set;
    std::queue<board_event> _events;

    // TODO: get these via config file on constructor
    std::string host;
    int ws_port;
    int http_port;

    virtual_board();

    void debug() const {
        printf("[debug] PIN SET:\n");
        _pin_set.debug_pin_set();
    }
};

#endif