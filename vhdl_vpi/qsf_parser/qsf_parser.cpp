#include "qsf_parser.hpp"

namespace qsf {

void parser::parse_line(std::vector<token> &line_tokens,
                        std::vector<qsf_assignment> &assignments) {
  _curr_token = line_tokens.begin();
  if (_curr_token->_type == COMMENT)
    return;

  _end_token = line_tokens.end();

  while (_curr_token != _end_token) {
    auto cmd = expect_identifier("set_location_assignment");
    if (cmd.has_value()) {
      // TODO: technically, this parsing method does not follow the original
      // https://www.intel.com/content/www/us/en/docs/programmable/683432/21-3/tcl_pkg_project_ver_7.0_cmd_set_location_assignment.html
      // but in school projects, the assignments usually look always the same:
      // set_location_assignment <PIN> -to <SIGNAL_DST>
      // so in this case, this parsing method should be fine **for now**

      auto pin = expect_identifier("");
      if (!pin.has_value())
        throw std::runtime_error("Expected PIN assignment.");

      auto arg = expect_argument("");
      if (arg.has_value()) {

        if (arg->_text == "-to") {

          auto dst = expect_identifier("");
          if (!dst.has_value())
            throw std::runtime_error(
                "Expected destination of assignment, usage: \"-to <dst>\"");

          printf("dst = ");
          dst->debug_token();
          assignments.push_back({pin->_text, dst->_text});

        } else {
          throw std::runtime_error("Expected a valid argument.");
        }
      }

      break;
    }

    throw std::runtime_error("Expected a valid command.");
  }
}

std::optional<token> parser::expect_identifier(const std::string &name) {
  if (_curr_token == _end_token || _curr_token->_type != IDENTIFIER) {
    return std::nullopt;
  }

  if (!name.empty() && _curr_token->_text != name) {
    return std::nullopt;
  }

  return *(_curr_token++);
}

std::optional<token> parser::expect_argument(const std::string &name) {
  if (_curr_token == _end_token || _curr_token->_type != ARGUMENT) {
    return std::nullopt;
  }

  if (!name.empty() && _curr_token->_text != name) {
    return std::nullopt;
  }

  return *(_curr_token++);
}

} // namespace qsf