#ifndef ASSIGNMENTS_CFG_LEXER_HPP
#define ASSIGNMENTS_CFG_LEXER_HPP

#include <string>
#include <vector>

/// lexer stuff for our custom pin assignments config file
namespace assignments_cfg {

/// possible tokens
enum token_type {
    WHITESPACE,
    IDENTIFIER,
    INTEGER_LITERAL,
    STRING_LITERAL,
    COMMENT,
    ARGUMENT,
    OPERATOR,
};

/// token's string representation
inline const char* token_type_string[] = {
    "WHITESPACE", "IDENTIFIER", "INTEGER_LITERAL", "STRING_LITERAL",
    "COMMENT",    "ARGUMENT",   "OPERATOR",
};

/// represents 1 token
typedef struct token {
    token_type _type{WHITESPACE};
    std::string _text{""};
    // std::size_t _start_offs{0};
    // std::size_t _end_offs{0};
    std::size_t _line_number{0};
    void debug_token() const {
        printf("\t%s at line %ld = \"%s\" \n", token_type_string[_type], _line_number,
               _text.c_str());
    };
} token;

/// TOKEN_LIST represents all the file lines, where each line will be the list of line tokens
using TOKEN_LIST = std::vector<std::vector<token>>;

/// lexer class
class lexer {
public:
    std::size_t _line_number{0};

    /// returns list of tokens from [input]
    TOKEN_LIST get_tokens(const std::string& input);

    /// ends current token (adding it to the token list) and starts a new one
    void end_token(token& tk, std::size_t line_number, TOKEN_LIST& tokens);

    /// debugs token list
    void debug_tokens(const TOKEN_LIST& token_list);
};

} // namespace assignments_cfg

#endif