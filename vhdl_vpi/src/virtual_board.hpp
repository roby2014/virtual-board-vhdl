#ifndef VIRTUAL_BOARD_HPP
#define VIRTUAL_BOARD_HPP

#include <string>
#include "pin_set.hpp"

/// Represents the virtual board
class virtual_board {
public:
    std::string _top_ent_name;
    pin_set _pin_set;

    void debug() const {
        printf("[debug] PIN SET:\n");
        _pin_set.debug_pin_set();
    }
};

#endif
