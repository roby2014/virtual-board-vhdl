#ifndef BOARD_CONFIG_HPP
#define BOARD_CONFIG_HPP

#include <vector>
#include "json/json.hpp"

using json = nlohmann::json;

namespace board_config {

typedef struct board_pin {
    std::string id;   // PIN_XXX
    std::string name; // SW0
    void debug_pin() const {
        printf("\t%s %s\n", id.c_str(), name.c_str());
    }
} board_pin;

/// opens "board.cfg" and stores peripherals pins into a generic collection, returning it
std::vector<board_pin> get_board_config();

/// adds [peripheral] pins info from json [data] to [board_pins] collection
void add_pins_to_list(const json& data, std::vector<board_pin>& board_pins,
                      const char* peripheral_name);

/// returns true if [pin_info] exists in our custom board config
/// pin_info can be either pinId or PIN name
bool pin_exists(const std::vector<board_pin>& board_pins, const std::string& pin_info);

} // namespace board_config

#endif