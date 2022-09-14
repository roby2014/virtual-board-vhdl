#ifndef PIN_SET_HPP
#define PIN_SET_HPP

#include <string>
#include <vector>
#include "pin.hpp"
#include "vpi_user.h"

/// Collection of pins encapsulated in this class
/// It has some utility functions for pin stuff (searching, debugging, inserting, ...)
class pin_set {
public:
    std::vector<pin> pins;

    pin_set();

    /// adds [p] to pin set
    void add_pin(pin p);

    /// returns true if [p] exists on pin set
    bool pin_exists(pin p) const;

    /// returns true if [p] exists on pin set
    bool pin_exists(const std::string& pin_id) const;

    /// returns pointer to pin by his identifier
    // TODO: returning a pointer can be a really bad idea because pins is a std::vector
    pin* get_pin(const std::string& pin_id);

    /// returns pin's signal value by his identifier
    bool get_pin_value(const std::string& pin_id) const;

    /// returns net's signal value by his net name and index (e.g cout[3] = cout, 3)
    bool get_pin_value(const std::string& net_name, std::size_t index) const;

    /// returns a net pointer to pin by his identifier / signal name
    vpiHandle get_pin_net(const std::string& pin_info) const;

    /// prints pin set
    void debug_pin_set() const;
};

#endif