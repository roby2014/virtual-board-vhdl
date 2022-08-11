#ifndef PIN_SET_HPP
#define PIN_SET_HPP

#include "vpi.hpp"
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

    /// returns signal's bit value at index
    int get_value() const {
        s_vpi_value val{.format = vpiBinStrVal};
        vpi_get_value(net, &val);

        std::string str = std::string(val.value.str);
        std::reverse(str.begin(), str.end()); // make it into little endian
        return str[index] == '1';
    }

    void debug_pin() const {
        printf("\t%s (%s[%ld]) = %d (0x%x) \n", id.c_str(), net_name.c_str(), index, get_value(),
               net);
    }
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

    /// returns true if [p] exists on pin set
    bool pin_exists(const std::string& pin_id) const {
        for (const auto& p : pins) {
            if (p.id == pin_id)
                return true;
        }
        return false;
    }

    /// returns pin's signal value by his identifier
    bool get_pin_value(const std::string& pin_id) const {
        for (const auto& p : pins) {
            if (p.id == pin_id)
                return p.get_value();
        }
        fprintf(stderr, "VPI_ERROR: Something went wrong @ get_pin_value");
        return 0;
    }

    /// returns net's signal value by his net name and index (e.g cout[3] = cout, 3)
    bool get_pin_value(const std::string& net_name, std::size_t index) const {
        for (const auto& p : pins) {
            if (p.net_name == net_name && p.index == index)
                return p.get_value();
        }
        fprintf(stderr, "VPI_ERROR: Something went wrong @ get_pin_value");
        return 0;
    }

    // TODO: put pin val

    /// returns a net pointer to pin's by his identifier
    vpiHandle get_pin_net(const std::string& pin_id) const {
        for (const auto& p : pins) {
            if (p.id == pin_id)
                return p.net;
        }
        fprintf(stderr, "VPI_ERROR: Something went wrong @ get_pin_net");
        return nullptr;
    }

    /// returns a net pointer to pin's by signal name
    vpiHandle get_pin_net(const std::string& net_name, std::size_t index) const {
        for (const auto& p : pins) {
            if (p.net_name == net_name && p.index == index)
                return p.net;
        }
        fprintf(stderr, "VPI_ERROR: Something went wrong @ get_pin_net");
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