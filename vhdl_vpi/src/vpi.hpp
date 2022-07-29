#ifndef VPI_HPP
#define VPI_HPP

#include "qsf_parser.hpp"
#include "virtual_board.hpp"
#include "vpi_user.h"
#include <vector>

#define CLK_SPEED 1

/// VPI related functions/wrappers
namespace vpi {

/// Entry point callback
void cb_entry_point();

/// Start of simulation callback
PLI_INT32 cb_simulation_start(p_cb_data cb_data);

/// End of simulation callback
PLI_INT32 cb_simulation_end(p_cb_data cb_data);

/// Returns [net]'s value as integer
int get_net_val(vpiHandle net);

/// Sets [net] value to [value] integer
void set_net_val(vpiHandle net, int value);

/// Returns [net]'s value at [idx] (assuming net is a vector/array)
/// *WARNING* This function assumes that the array is in little endian
bool get_bit_net(vpiHandle net, std::size_t idx = 0);

/// Converts [t] time to vpi simulation time
s_vpi_time double_to_vpi_time(double t, double time_resolution);

/// Assigns [obj] to hex[hex_idx][pin_idx]
void assign_net_to_hex(virtual_board* vb, vboard_assignments obj, std::size_t hex_idx,
                       std::size_t pin_idx);

/// Returns true if something went wrong with VPI
/// It prints the (error) message if any
bool check_error();

/// Iterate over top entity IO signals and store them in our virtual board SW and LED
void get_io_signals(virtual_board& vb, const std::vector<pin_assignment>& pin_assignments);

/// Init simulation callback
/// 3 stages:
PLI_INT32 cb_init(p_cb_data cb_data);

/// Main simulation callback
PLI_INT32 main_callback(p_cb_data cb_data);

/// Registers callback ([cb_rtn]) after [delay].
void register_cb_after(PLI_INT32 (*cb_rtn)(struct t_cb_data*), double delay, virtual_board* vboard);

///
void register_cb_at_last_known_delta(PLI_INT32 (*cb_rtn)(struct t_cb_data*), virtual_board* vboard);

/// This struct will be useful to pass more than 1 argument to the LED value change callback
/// [pin_idx] - LED PIN, we want the pin index to know what pin to toggle
/// [net_idx] - If net is an array, we need to know what bit changed
/// [vboard] - virtual_board pointer so we can update interface
typedef struct led_mapping {
    std::size_t pin_idx{};
    std::size_t net_idx{};
    virtual_board* vboard;
} led_mapping;

/// Registers callback when some output (LED) value changes
void register_ledr_value_change_cb(PLI_INT32 (*cb_rtn)(struct t_cb_data*), vpiHandle net,
                                   led_mapping* obj);

/// Callback when any LEDR changes
/// To know what LEDR/bit changed,, access cb_data->user_data ([led_mapping])
PLI_INT32 on_leds_value_change(p_cb_data cb_data);

/// This struct will be useful to pass more than 1 argument to the HEX value change callback
/// [hex_idx] - HEX INDEX PIN (0 if hex0, 1 if hex1, ...), we want it so we know what hex to refresh
/// [vboard] - virtual_board pointer so we can update interface
typedef struct hex_mapping {
    std::size_t hex_idx{};
    std::size_t net_idx{};
    virtual_board* vboard;
} hex_mapping;

/// Registers callback when some HEX value changes
void register_hex_value_change_cb(PLI_INT32 (*cb_rtn)(struct t_cb_data*), vpiHandle net,
                                  hex_mapping* obj);

/// Callback when any HEX changes
/// To know what HEX changed, access cb_data->user_data ([hex_mapping])
PLI_INT32 on_hex_value_change(p_cb_data cb_data);

} // namespace vpi

#endif // VPI_HPP
