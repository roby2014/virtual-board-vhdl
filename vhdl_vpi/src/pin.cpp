#include <algorithm>
#include "pin.hpp"
#include "vpi.hpp"

/// returns signal's bit value at index
int pin::get_value() const {
    s_vpi_value val{.format = vpiBinStrVal};
    vpi_get_value(net, &val);

    std::string str = std::string(val.value.str);
    std::reverse(str.begin(), str.end()); // make it into little endian
    return str[index] == '1';
}

/// debug pin
void pin::debug_pin() const {
    printf("\tPIN CHANGED -> %s (%s[%ld]) = %d (0x%x) \n", id.c_str(), net_name.c_str(), index,
           get_value(), net);
}
