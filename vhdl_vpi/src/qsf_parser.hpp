#ifndef QSF_PARSER_HPP
#define QSF_PARSER_HPP

#include "pin_maps.hpp"
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

/// represents an assignement from pin file
typedef struct assignment_link {
    int pin_type;         // 1 = SW, 2 = LEDR/HEX
    std::string pin_name; // PIN_XXX
    std::string net_name; // SIGNAL NAME
    std::size_t index;    // INDEX, 0 if non ARRAY
} pin_assignment;

/// used to parse .qsf files (associate/linked de10 pins to vhdl signals)
class qsf_parser {
public:
    std::array<std::string, 16> COMMANDS{"set_location_assignment", "set_global_assignment",
                                         "set_instance_assignment"};

    qsf_parser(const std::string& fname) : input{fname} {
        if (!input.is_open()) {
            std::cerr << "[ERROR:qsf_parser] File \"" << fname << "\" does not exist!\n";
            exit(-1);
        }
    }

    /// returns true if [cmd] is a valid command
    /// useful for checking if command from qsf file (set_local_assignment, etc...) exists
    template <typename T> bool valid_command(const T& arr, const std::string& token);

    /// returns 1 if [pin] is a SW pin, 2 if LEDR/HEX, 0 if invalid (set_location_assignment only)
    int valid_pin(const std::string& pin);

    /// returns true if [pin] is a HEX PIN
    static bool is_hex_pin(const std::string& pin);

    /// parse input code (from stream)
    std::vector<pin_assignment> parse();

    /// consumes line and returns it
    std::string consume_line();

    /// returns char at current position
    char peek();

    /// skips current line
    void advance_to_next_line();

    /// advances 1 position, incrementing line if we on a new line
    /// returns false if we hitted EOF
    char advance();

    /// returns true if [c] is a "whitespace"
    bool is_whitespace(char c);

    /// skips whitespaces
    void skip_whitespaces();

    /// returns net's name (in lower case) and index case any
    /// e.g : arr[3] will return (arr, 3)
    /// CLK will return (clk, 0)
    std::pair<std::string, std::size_t> get_net_and_index(const std::string& str);

    /// Returns pin "index" by its name
    /// e.g PIN_C11 is SW[1] (type 1) so it would return 1
    static int get_pin_index(const std::string& pin_name, int type) {
        if (type == 1)
            return SW_PINS.at(pin_name);
        else if (type == 2) {
            return LED_PINS.at(pin_name);
        }
        return -1;
    }

    /// Returns pair of hex "index" and pin index by its name
    /// e.g PIN_E15 is HEX0[1] so it would return <0, 1>
    static std::pair<std::size_t, std::size_t> get_hex_pin_index(const std::string& pin_name) {
        if (HEX0_PINS.contains(pin_name)) {
            return std::pair<std::size_t, std::size_t>(0, HEX0_PINS.at(pin_name));
        } else if (HEX1_PINS.contains(pin_name)) {
            return std::pair<std::size_t, std::size_t>(1, HEX1_PINS.at(pin_name));
        } else if (HEX2_PINS.contains(pin_name)) {
            return std::pair<std::size_t, std::size_t>(2, HEX2_PINS.at(pin_name));
        } else if (HEX3_PINS.contains(pin_name)) {
            return std::pair<std::size_t, std::size_t>(3, HEX3_PINS.at(pin_name));
        } else if (HEX4_PINS.contains(pin_name)) {
            return std::pair<std::size_t, std::size_t>(4, HEX4_PINS.at(pin_name));
        } else if (HEX4_PINS.contains(pin_name)) {
            return std::pair<std::size_t, std::size_t>(5, HEX5_PINS.at(pin_name));
        }
        return std::pair(0, 0);
    }

private:
    std::ifstream input;
    int curr_pos{};
    int curr_line{1};
};

#endif