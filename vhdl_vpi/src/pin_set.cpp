#include "pin_set.hpp"
#include "pin.hpp"

pin_set::pin_set() {}

/// adds [p] to pin set
void pin_set::add_pin(pin p) {
    pins.push_back(p);
}

/// returns true if [p] exists on pin set
bool pin_set::pin_exists(pin p) const {
    for (const auto& p2 : pins) {
        if (p.id == p2.id)
            return true;
    }
    return false;
}

/// returns true if pin with pin_info (net_name/id) exists on pin set
bool pin_set::pin_exists(const std::string& pin_info) const {
    for (const auto& p : pins) {
        if (p.id == pin_info || p.net_name == pin_info)
            return true;
    }
    return false;
}

/// returns pointer to pin by his identifier
pin* pin_set::get_pin(const std::string& pin_info) {
    for (auto& p : pins) {
        if (p.id == pin_info || p.net_name == pin_info)
            return &p;
    }
    return nullptr;
}

/// returns pointer to pin by his identifier
pin* pin_set::get_pin(const vpiHandle pin_handle) {
    for (auto& p : pins) {
        if (p.net == pin_handle)
            return &p;
    }
    return nullptr;
}

/// returns pin's signal value by his identifier
bool pin_set::get_pin_value(const std::string& pinId) const {
    for (const auto& p : pins) {
        if (p.id == pinId)
            return p.get_value();
    }
    return 0;
}

/// returns pin's value by his net name and index (e.g cout[3] = cout, 3)
bool pin_set::get_pin_value(const std::string& net_name, std::size_t index) const {
    for (const auto& p : pins) {
        if (p.net_name == net_name && p.index == index)
            return p.get_value();
    }
    return 0;
}

/// returns a net pointer to pin by his identifier / signal name
vpiHandle pin_set::get_pin_net(const std::string& pin_info) const {
    for (const auto& p : pins) {
        if (pin_info == p.id || pin_info == p.net_name)
            return p.net;
    }
    return nullptr;
}

/// prints pin set
void pin_set::debug_pin_set() const {
    for (const auto& p : pins) {
        p.debug_pin();
    }
}