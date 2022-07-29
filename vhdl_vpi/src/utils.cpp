#include "utils.hpp"
#include <sstream>
#include <utility>

namespace utils {
std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        result.push_back(item);
    }
    return result;
}

std::string to_lower(const std::string& str) {
    std::locale loc;
    std::string buff;

    for (const auto& elem : str) {
        char c = std::tolower(elem, loc);
        buff += c;
    }

    return buff;
}

} // namespace utils