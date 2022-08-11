#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>

/// Some general utilities
namespace utils {

/// splits [s] into tokens, depending on [delim]
std::vector<std::string> split(const std::string& s, char delim);

/// returns [str] in lower case
std::string to_lower(const std::string& str);

/// returns net's name (in lower case) and index case any
/// e.g : arr[3] will return (arr, 3)
/// CLK will return (clk, 0)
std::pair<std::string, std::size_t> get_net_and_index(const std::string& str);

} // namespace utils

#endif