#include "utils.hpp"
#include <sstream>
#include <string>
#include <utility>
#include <vector>
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

std::pair<std::string, std::size_t> get_net_and_index(const std::string& str) {
    auto offset = str.find("[");

    if (offset == std::string::npos)
        return std::pair<std::string, int>(to_lower(str), 0);

    std::string net;
    std::string num;
    for (std::size_t i = 0; i < offset; i++)
        net += str.at(i);

    for (std::size_t i = offset + 1; i < str.size() - 1; i++)
        num += str.at(i);

    return std::pair<std::string, int>(to_lower(net), std::stoi(num));
}

} // namespace utils