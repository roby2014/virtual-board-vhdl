#ifndef VIRTUAL_BOARD_HPP
#define VIRTUAL_BOARD_HPP

#include "vpi_user.h"
#include <array>
#include <string>

#define MAX_SW_PINS 10
#define MAX_LEDR_PINS 10
#define MAX_HEX_PINS 8

/// represents a real assignment
/// [net] net handle
/// [index] index (net can be an array)
typedef struct virtual_board_assignments {
    vpiHandle net;
    std::size_t index;
} vboard_assignments;

class virtual_board {

public:
    std::string top_entity;

    std::array<vboard_assignments, MAX_SW_PINS> SW{}; // input

    std::array<vboard_assignments, MAX_LEDR_PINS> LEDR{}; // output

    std::array<vboard_assignments, MAX_HEX_PINS> HEX0{}; // hex display 0
    std::array<vboard_assignments, MAX_HEX_PINS> HEX1{};
    std::array<vboard_assignments, MAX_HEX_PINS> HEX2{};
    std::array<vboard_assignments, MAX_HEX_PINS> HEX3{};
    std::array<vboard_assignments, MAX_HEX_PINS> HEX4{};
    std::array<vboard_assignments, MAX_HEX_PINS> HEX5{};
};

#endif