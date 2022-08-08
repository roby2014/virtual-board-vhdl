#ifndef PIN_MAPS_HPP
#define PIN_MAPS_HPP

#include "vpi_user.h"
#include <string>
#include <unordered_map>
#include <vector>

/// Represents a pin
struct pin {
    std::string id;       // PIN_XXX
    vpiHandle net;        // pointer to signal
    std::string net_name; // signal's name
    std::size_t index;    // signal's index (0 if non array)

    int get_value() const {
        int ret = 0;
        int size = vpi_get(vpiSize, net);
        s_vpi_value val = {.format = vpiBinStrVal};
        vpi_get_value(net, &val);
        for (int i = 0; i < size; i++) {
            ret <<= 1;
            if (val.value.str[i] == '1')
                ret |= 1;
        }
        return ret;
    }

    void debug_pin() const {
        printf("\t%s (%s[%ld]) = %d (0x%x) \n", id.c_str(), net_name.c_str(), index, get_value(),
               net);
    };
};

/// Collection of pins encapsulated in this class
/// It has some utility functions for pin stuff (searching, debugging, inserting, ...)
class pin_set {
public:
    std::vector<pin> pins;

    pin_set() {}
    pin_set(const std::vector<pin>& p) : pins(p) {}

    /// adds [p] to pin set
    void add_pin(pin p) {
        pins.push_back(p);
    }

    /// returns true if [p] exists on pin set
    bool pin_exists(pin p) const {
        for (const auto& p2 : pins) {
            if (p.id == p2.id)
                return true;
        }
        return false;
    }

    /// returns a net pointer to pin's by his identifier
    vpiHandle get_pin_net(const std::string& pin_id) const {
        for (const auto& p : pins) {
            if (p.id == pin_id)
                return p.net;
        }
        return nullptr;
    }

    /// returns a net pointer to pin's by signal name
    vpiHandle get_pin_net(const std::string& net_name, std::size_t index) const {
        for (const auto& p : pins) {
            if (p.net_name == net_name && p.index == index)
                return p.net;
        }
        return nullptr;
    }

    /// prints pin set
    void debug_pin_set() const {
        for (const auto& p : pins) {
            p.debug_pin();
        }
    }
};

#endif