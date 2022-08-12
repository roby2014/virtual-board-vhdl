#ifndef ASSIGNMENTS_CFG_PARSER_HPP
#define ASSIGNMENTS_CFG_PARSER_HPP

#include <optional>
#include <string>
#include <vector>
#include <stdexcept>
#include "lexer.hpp"

/// Parser stuff for our custom pin assignments config file
namespace assignments_cfg {

/// Token iterator alias
using token_iterator = std::vector<token>::iterator;

/// Represents a pin assignment
typedef struct pin_assignment {
    std::string pin;        // PIN_XXX
    std::string signal_dst; // SIGNAL DESTINATION (-to <dst>)
    std::size_t index;      // signal's index (0 if non array)

    void debug_assignment() const {
        printf("\t %s -> %s[%ld] \n", pin.c_str(), signal_dst.c_str(), index);
    }
} pin_assignment;

/// Parser class
class parser {
public:
    /// parses input tokens, exits if any error occurred
    std::vector<pin_assignment> parse(TOKEN_LIST& tokens);

    /// debugs pin assignments
    void debug_assignments(const std::vector<pin_assignment>& assignments);

private:
    token_iterator _curr_token;
    token_iterator _end_token;

    /// Parses line tokens and inserts them into [assignments]
    void parse_line(std::vector<token>& line_tokens, std::vector<pin_assignment>& assignments);

    /// Empty string means match any
    template <token_type TYPE> std::optional<token> expect_(const std::string& name);
};

} // namespace assignments_cfg

#endif
