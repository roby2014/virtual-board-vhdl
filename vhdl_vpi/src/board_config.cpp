#include <stdio.h>
#include <vector>
#include <fstream>
#include <iostream>
#include "board_config.hpp"

namespace board_config {

std::vector<board_pin> get_board_config() {
    std::vector<board_pin> board_pins;
    try {
        std::ifstream fp("assets/board.json");
        json json_data = json::parse(fp);
        add_pins_to_list(json_data, board_pins, "leds");
        add_pins_to_list(json_data, board_pins, "switches");
        add_pins_to_list(json_data, board_pins, "buttons");
        add_pins_to_list(json_data, board_pins, "otherPins");
    } catch (std::exception& e) {
        fprintf(stderr, "Board config @ ERROR: %s \n", e.what());
        exit(-1);
    }
    return board_pins;
}

/// This struct will be useful to get json data values
/// by using [NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE] macro
typedef struct key_value_t {
    std::string pinName;
    std::string pinId;
} key_value_t;
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(key_value_t, pinName, pinId);

void add_pins_to_list(const json& data, std::vector<board_pin>& board_pins,
                      const char* peripheral_name) {

    auto objects{data.at(peripheral_name).get<std::vector<key_value_t>>()};
    for (const auto& p : objects) {
        board_pins.push_back({board_pin{.id = p.pinId, .name = p.pinName}});
    }
}

bool pin_exists(const std::vector<board_pin>& board_pins, const std::string& pin_info) {
    for (const auto& pin : board_pins) {
        if (pin_info == pin.id || pin_info == pin.name) {
            return true;
        }
    }
    return false;
}

} // namespace board_config
