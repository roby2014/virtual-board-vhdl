#include "qsf_parser.hpp"
#include "utils.hpp"
#include "vpi_user.h"
#include <algorithm>
#include <string>
#include <vector>

template <typename T> bool qsf_parser::valid_command(const T& arr, const std::string& token) {
    return std::find(std::begin(arr), std::end(arr), token) != std::end(arr);
}

int qsf_parser::valid_pin(const std::string& pin) {
    if (SW_PINS.contains(pin))
        return 1;

    if (LED_PINS.contains(pin) || is_hex_pin(pin))
        return 2;

    return 0;
}

bool qsf_parser::is_hex_pin(const std::string& pin) {
    return HEX0_PINS.contains(pin) || HEX1_PINS.contains(pin) || HEX2_PINS.contains(pin) ||
           HEX3_PINS.contains(pin) || HEX4_PINS.contains(pin) || HEX5_PINS.contains(pin);
}

std::vector<pin_assignment> qsf_parser::parse() {
    std::vector<pin_assignment> assignments;

    while (!input.eof()) {
        skip_whitespaces();

        if (peek() == '#') {
            advance_to_next_line();
            continue;
        }

        auto line_tokens = utils::split(consume_line(), ' ');
        if (line_tokens.size() == 0)
            continue;

        auto cmd = line_tokens[0];
        if (!valid_command(COMMANDS, cmd)) {
            vpi_printf("VPI_ERROR: at line \"%d\": command \"%s\" not found!\n", curr_line,
                       cmd.c_str());
            exit(-1);
        }

        // TODO: parse more than set_location_assignment?
        if (cmd != "set_location_assignment")
            continue;

        auto pin = line_tokens[1];
        auto pin_type = valid_pin(pin);
        if (!pin_type) {
            vpi_printf("VPI_ERROR: at line \"%d\": PIN \"%s\" does not exist!\n", curr_line,
                       pin.c_str());
            exit(-1);
        }

        if (line_tokens[2] != "-to") {
            // FIXME: if we input a space, this throws error
            // e.g: "set_location_assignment PIN_A8  -to R[3]" throws
            // e.g: "set_location_assignment PIN_A8 -to R[3]" doesnt
            // this probably happens with the other tokens aswell
            vpi_printf("VPI_ERROR: at line \"%d\": missing \"-to\" token!\n", curr_line);
            exit(-1);
        }

        auto [net, idx] = get_net_and_index(line_tokens[3]);
        assignments.push_back({pin_type, pin, net, idx});
    }
    return assignments;
}

std::string qsf_parser::consume_line() {
    skip_whitespaces();

    std::string line;
    char c = peek();
    while (c != '\n') {
        if (c == '#') {
            advance_to_next_line();
            break;
        }

        if (advance() == -1)
            break;

        line += c;
        c = peek();
    }

    return line;
}

char qsf_parser::peek() {
    return input.peek();
}

void qsf_parser::advance_to_next_line() {
    while (peek() != '\n' && advance() != -1)
        ;
}

char qsf_parser::advance() {
    char c = input.get();

    if (input.eof()) {
        return -1;
    }

    if (c == '\n') {
        curr_line++;
    }

    curr_pos++;
    return c;
}

bool qsf_parser::is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

void qsf_parser::skip_whitespaces() {
    while (is_whitespace(peek())) {
        advance();
    }
}

std::pair<std::string, std::size_t> qsf_parser::get_net_and_index(const std::string& str) {
    auto offset = str.find("[");

    if (offset == std::string::npos)
        return std::pair<std::string, int>(utils::to_lower(str), 0);

    std::string net;
    std::string num;
    for (std::size_t i = 0; i < offset; i++)
        net += str.at(i);

    for (std::size_t i = offset + 1; i < str.size() - 1; i++)
        num += str.at(i);

    return std::pair<std::string, int>(utils::to_lower(net), std::stoi(num));
}