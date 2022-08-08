#pragma once

#include <string>
#include <vector>

namespace qsf {

enum token_type {
  WHITESPACE,
  IDENTIFIER,
  INTEGER_LITERAL,
  STRING_LITERAL,
  COMMENT,
  ARGUMENT, // -to, -name, etc..
  OPERATOR,
};

inline const char *token_type_string[] = {
    "WHITESPACE", "IDENTIFIER", "INTEGER_LITERAL", "STRING_LITERAL",
    "COMMENT",    "ARGUMENT",   "OPERATOR",
};

typedef struct token {
  token_type _type{WHITESPACE};
  std::string _text{""};
  std::size_t _start_offs{0};
  std::size_t _end_offs{0};
  std::size_t _line_number{0};
  void debug_token() const {
    printf("\t%s at line %ld = \"%s\" \n", token_type_string[_type],
           _line_number, _text.c_str());
  };
} token;

/// Each line will be a list of vectors
using TOKEN_LIST = std::vector<std::vector<token>>;

class lexer {
public:
  std::size_t _line_number{0};

  /// returns list of tokens from [input]
  TOKEN_LIST get_tokens(const std::string &input);

  /// ends current token (adding it to the token list) and starts a new one
  void end_token(token &tk, std::size_t line_number, TOKEN_LIST &tokens);
};

} // namespace qsf
