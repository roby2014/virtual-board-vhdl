#ifndef PIN_HPP
#define PIN_HPP

#include <string>
#include "vpi_user.h"

/// Represents a pin
struct pin {
    std::string id;       // PIN_XXX
    vpiHandle net;        // pointer to signal
    std::string net_name; // signal's name
    std::size_t index;    // signal's index (0 if non array)
    // this field is useful because our signal can be an array, which means the net pointer will be
    // always the same, so having an "index" we can distinguish the array values

    /// returns signal's bit value at index
    int get_value() const;

    /// debug pin
    void debug_pin() const;
};

#endif