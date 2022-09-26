#include <string>
#include <vector>
#include "parser.hpp"
#include "utils.hpp"

namespace assignments_cfg {

std::vector<pin_assignment> parser::parse(TOKEN_LIST& tokens) {
    std::vector<pin_assignment> assignments;

    for (std::size_t i = 0; i < tokens.size(); i++) {
        if (tokens[i].size() == 0)
            continue;

        try {
            parse_line(tokens[i], assignments);
        } catch (std::exception& err) {
            fprintf(stderr, "Parse error at line %ld - %s \n", i + 1, err.what());
            exit(-1);
        } catch (...) {
            fprintf(stderr, "Unknown parse error :( \n");
            exit(-1);
        }
    }

    return assignments;
}

void parser::parse_line(std::vector<token>& line_tokens, std::vector<pin_assignment>& assignments) {
    _curr_token = line_tokens.begin();
    if (_curr_token->_type == COMMENT)
        return;

    _end_token = line_tokens.end();

    auto pin = expect_<IDENTIFIER>("");
    if (!pin.has_value())
        throw std::runtime_error("Expected PIN assignment.");

    auto arg = expect_<ARGUMENT>("->");
    if (!arg.has_value())
        throw std::runtime_error("Expected a valid argument.");

    auto dst = expect_<IDENTIFIER>("");
    if (!dst.has_value())
        throw std::runtime_error("Expected destination of assignment, usage: \"-> <dst>\"");

    // printf("dst = ");
    // dst->debug_token();
    // TODO: check if dst is correctly typed
    // e.g a[3] and not a{3} or a[3 etc..
    auto [signal_dst, idx] = utils::get_net_and_index(dst->_text);
    assignments.push_back({pin->_text, signal_dst, idx});
}

template <token_type TYPE> std::optional<token> parser::expect_(const std::string& name) {
    if (_curr_token == _end_token || _curr_token->_type != TYPE) {
        return std::nullopt;
    }

    if (!name.empty() && _curr_token->_text != name) {
        return std::nullopt;
    }

    return *(_curr_token++);
}

void parser::debug_assignments(const std::vector<pin_assignment>& assignments) {
    printf("[debug PIN ASSIGNMENTS] start\n");
    for (const auto& a : assignments) {
        a.debug_assignment();
    }
    printf("[debug PIN ASSIGNMENTS] end\n");
}

} // namespace assignments_cfg