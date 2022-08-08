#include "board_config.hpp"
#include "libconfig.h++"
#include <stdio.h>
#include <vector>

namespace board_config {

std::vector<board_pin> get_board_config() {
    libconfig::Config cfg;

    try {
        cfg.readFile("assets/board.cfg");
    } catch (const libconfig::FileIOException& fioex) {
        fprintf(stderr, "I/O error while reading file. \n");
        exit(-1);
    } catch (const libconfig::ParseException& pex) {
        fprintf(stderr, "Parse error at %s:%d - %s \n", pex.getFile(), pex.getLine(),
                pex.getError());
        exit(-1);
    }

    try {
        std::string name = cfg.lookup("board_name");
        printf("Board name: %s \n", name.c_str());
    } catch (const libconfig::SettingNotFoundException& nfex) {
        fprintf(stderr, "No 'board_name' setting in configuration file.\n");
    }

    const libconfig::Setting& root = cfg.getRoot();
    std::vector<board_pin> board_pins;

    auto led_pins = get_peripheral(root, "leds");
    board_pins.insert(board_pins.end(), led_pins.begin(), led_pins.end());

    auto switches_pins = get_peripheral(root, "switches");
    board_pins.insert(board_pins.end(), switches_pins.begin(), switches_pins.end());

    for (const auto& p : board_pins) {
        p.debug_pin();
    }
    // TODO:
    return board_pins;
}

std::vector<board_pin> get_peripheral(const libconfig::Setting& root, const char* peripheral) {
    std::vector<board_pin> pins;

    try {
        const libconfig::Setting& leds = root["board"][peripheral];
        int count = leds.getLength();

        printf("%s (%d in total):\n", peripheral, count);

        for (int i = 0; i < count; ++i) {
            const libconfig::Setting& ledr = leds[i];

            // Only output the record if all of the expected fields are present.
            std::string name, id;
            int index;

            if (!(ledr.lookupValue("pin_name", name) && ledr.lookupValue("pin_id", id) &&
                  ledr.lookupValue("pin_index", index)))
                continue;

            printf("%s %s %d\n", name.c_str(), id.c_str(), index);
            board_pin item = {.id = id, .name = name, .index = (std::size_t)index};
            pins.push_back(item);
        }
    } catch (const libconfig::SettingNotFoundException& nfex) {
        fprintf(stderr, "Setting \"root[board][%s]\" not found.\n", peripheral);
    }

    return pins;
}

bool pin_exists(const std::vector<board_pin>& board_pins, const std::string& pin_name) {
    for (const auto& pin : board_pins) {
        if (pin_name == pin.id) {
            return true;
        }
    }
    return false;
}

} // namespace board_config
