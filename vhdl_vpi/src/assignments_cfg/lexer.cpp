#include <string>
#include <vector>
#include "lexer.hpp"
namespace assignments_cfg {

TOKEN_LIST lexer::get_tokens(const std::string& input) {
    TOKEN_LIST tokens(1);
    token curr{._line_number = 1};

    for (char c : input) {

        // if a number is found
        if (c >= '0' && c <= '9') {
            if (curr._type == WHITESPACE || curr._type == INTEGER_LITERAL) {
                curr._type = INTEGER_LITERAL;
                curr._text.append(1, c);
            } else {
                curr._text.append(1, c);
            }
            continue;
        }

        switch (c) {

        // start/end of string
        case '"':
            if (curr._type != STRING_LITERAL) {
                end_token(curr, _line_number, tokens);
                curr._type = STRING_LITERAL;
            } else if (curr._type == STRING_LITERAL) {
                end_token(curr, _line_number, tokens);
            }
            break;

        // comment
        case '#':
            if (curr._type == STRING_LITERAL) {
                curr._text.append(1, c);
            } else {
                end_token(curr, _line_number, tokens);
                curr._type = COMMENT;
                curr._text.erase();
            }
            break;

        case '-':
            if (curr._type == WHITESPACE) {
                curr._type = ARGUMENT;
                curr._text.append(1, c);
            } else {
                curr._text.append(1, c);
            }
            break;

        // whitespaces / new line
        case ' ':
        case '\t':
            if (curr._type == STRING_LITERAL || curr._type == COMMENT) {
                curr._text.append(1, c);
            } else {
                end_token(curr, _line_number, tokens);
            }
            break;
        case '\r':
        case '\n':
            end_token(curr, _line_number, tokens);
            curr._line_number++;
            tokens.resize(++_line_number + 1);
            break;

        // letters/other, maybe an identifier
        default:
            if (curr._type == WHITESPACE || curr._type == INTEGER_LITERAL) {
                end_token(curr, _line_number, tokens);
                curr._type = IDENTIFIER;
                curr._text.append(1, c);
            } else {
                curr._text.append(1, c);
            }
            break;
        }
    }

    end_token(curr, _line_number, tokens);
    return tokens;
}

void lexer::end_token(token& tk, std::size_t line_number, TOKEN_LIST& tokens) {
    if (tk._type != WHITESPACE)
        tokens[line_number].push_back(tk);

    tk._type = WHITESPACE;
    tk._text.erase();
}

void lexer::debug_tokens(const TOKEN_LIST& token_list) {
    printf("[debug] TOKENS:\n");
    for (std::size_t i = 0; i < token_list.size(); i++) {
        if (token_list[i].size() == 0)
            continue;
        for (const auto& t : token_list[i]) {
            t.debug_token();
        }
    }
}

} // namespace assignments_cfg