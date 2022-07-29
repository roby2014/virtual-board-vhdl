#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>

namespace utils {
// splits [s] into tokens, depending on [delim]
std::vector<std::string> split(const std::string& s, char delim);

// returns [str] in lower case
std::string to_lower(const std::string& str);
} // namespace utils

#endif