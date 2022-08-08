#ifndef BOARD_CONFIG_HPP
#define BOARD_CONFIG_HPP

#include "libconfig.h++"
#include <vector>

namespace board_config {

typedef struct board_pin {
    std::string id;    // PIN_XXX
    std::string name;  // SW0
    std::size_t index; // 0
    void debug_pin() const {
        printf("\t%s %s (%ld)\n", id.c_str(), name.c_str(), index);
    }
} board_pin;

/// opens "board.cfg" and stores peripherals pins into a generic collection, returning it
std::vector<board_pin> get_board_config();

/// returns [peripheral] pins info from [root] cfg by its name
std::vector<board_pin> get_peripheral(const libconfig::Setting& root, const char* peripheral);

/// returns true if [pin_name] exists in our custom board config
bool pin_exists(const std::vector<board_pin>& board_pins, const std::string& pin_name);

} // namespace board_config

#endif