#include "board_config.hpp"
#include "libconfig.h++"
#include <stdio.h>

namespace board_config {

bool get_board_config() {
    libconfig::Config cfg;

    try {
        cfg.readFile("assets/board.cfg");
    } catch (const libconfig::FileIOException& fioex) {
        fprintf(stderr, "I/O error while reading file. \n");
        return false;
    } catch (const libconfig::ParseException& pex) {
        fprintf(stderr, "Parse error at %s:%d - %s \n", pex.getFile(), pex.getLine(),
                pex.getError());
        return false;
    }

    try {
        std::string name = cfg.lookup("board_name");
        printf("Board name: %s \n", name.c_str());
    } catch (const libconfig::SettingNotFoundException& nfex) {
        fprintf(stderr, "No 'board_name' setting in configuration file.\n");
    }

    const libconfig::Setting& root = cfg.getRoot();
    get_peripheral(root, "leds");
    get_peripheral(root, "switches");
    // TODO:
    return true;
}

void get_peripheral(const libconfig::Setting& root, const char* peripheral) {
    // Output a list of all books in the inventory.
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
        }
    } catch (const libconfig::SettingNotFoundException& nfex) {
        fprintf(stderr, "Setting \"root[board][%s]\" not found.\n", peripheral);
    }
}

} // namespace board_config
