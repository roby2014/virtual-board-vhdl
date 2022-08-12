#ifndef PIN_HPP
#define PIN_HPP

#include "vpi_user.h"
#include <string>

/// Represents a pin
struct pin {
    std::string id;       // PIN_XXX
    vpiHandle net;        // pointer to signal
    std::string net_name; // signal's name
    std::size_t index;    // signal's index (0 if non array)

    /// returns signal's bit value at index
    int get_value() const;

    /// sets signal bit at index to value
    void set_value(bool value);

    /// debug pin
    void debug_pin() const;
};

#endif