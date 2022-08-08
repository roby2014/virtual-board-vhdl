#pragma once

#include "qsf_lexer.hpp"
#include <optional>

namespace qsf {

using token_iterator = std::vector<token>::iterator;

typedef struct qsf_assignment {
  std::string pin;        // PIN_XXX
  std::string signal_dst; // SIGNAL DESTINATION (-to <dst>)

  void debug_assignment() const {
    printf("\t %s -> %s \n", pin.c_str(), signal_dst.c_str());
  }
} qsf_assignment;

class parser {
public:
  void parse_line(std::vector<token> &line_tokens,
                  std::vector<qsf_assignment> &assignments);

private:
  token_iterator _curr_token;
  token_iterator _end_token;

  /// Empty string means match any identifier
  std::optional<token> expect_identifier(const std::string &name);

  /// Empty string means match any argument
  std::optional<token> expect_argument(const std::string &name);
};

} // namespace qsf