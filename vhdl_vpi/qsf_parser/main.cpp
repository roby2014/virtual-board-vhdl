#include "qsf_lexer.hpp"
#include "qsf_parser.hpp"
#include <iostream>

int main() {
  qsf::parser parser;
  qsf::lexer lexer;
  auto v = lexer.get_tokens("set_location_assignment PIN_N5 -to "
                            "A[3]\nset_location_assignment PIN_N1 -to A[2]");

  std::vector<qsf::qsf_assignment> assignments;

  printf("TOKENS:\n");
  for (std::size_t i = 0; i < v.size(); i++) {
    if (v[i].size() == 0)
      continue;
    for (const auto &t : v[i]) {
      t.debug_token();
    }

    try {
      parser.parse_line(v[i], assignments);
    } catch (std::exception &err) {
      fprintf(stderr, "Parse error at line %ld - %s \n", i + 1, err.what());
      return -1;
    } catch (...) {
      fprintf(stderr, "Unknown parse error :( \n");
      return -1;
    }
  }

  printf("QSF PIN ASSIGNMENTS:\n");
  for (const auto &a : assignments) {
    a.debug_assignment();
  }
}