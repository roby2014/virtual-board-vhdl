#ifndef VPI_HPP
#define VPI_HPP

#include "assignments_cfg/parser.hpp"
#include "pin_set.hpp"
#include "virtual_board.hpp"
#include "vpi_user.h"
#include <vector>

#define CLK_SPEED 1

// compiler things..
class virtual_board;
class pin_set;
struct pin;

/// VPI related functions/wrappers
namespace vpi {

/// Entry point callback
void cb_entry_point();

/// Start of simulation callback
PLI_INT32 cb_simulation_start(p_cb_data cb_data);

/// End of simulation callback
PLI_INT32 cb_simulation_end(p_cb_data cb_data);

/// Init simulation callback
/// Register pins change event and other stuff
PLI_INT32 cb_init(p_cb_data cb_data);

/// Main simulation callback
PLI_INT32 main_callback(p_cb_data cb_data);

/// Registers pin [p]'s value change, setting [cb_rtn] as calback
void register_pins_value_change_cb(PLI_INT32 (*cb_rtn)(struct t_cb_data*), pin* p);

/// Callback called when any pin changes
/// Pin information is [cb_data->user_data]
PLI_INT32 on_pins_value_change(p_cb_data cb_data);

/// Returns [net]'s value as integer
int get_net_val(vpiHandle net);

/// Sets [net] value to [value] integer
void set_net_val(vpiHandle net, int value);

/// Returns [net]'s value at [idx] (assuming net is a vector/array)
/// *WARNING* This function assumes that the array is in little endian
bool get_bit_net(vpiHandle net, std::size_t idx = 0);

/// Converts [t] time to vpi simulation time
s_vpi_time double_to_vpi_time(double t, double time_resolution);

/// Iterate over top entity IO signals, check which pins they are connected to
/// and store them in our pin_set ()
pin_set get_pins_signals(virtual_board& vb,
                         const std::vector<assignments_cfg::pin_assignment>& assignments);

/// Registers callback ([cb_rtn]) after [delay].
void register_cb_after(PLI_INT32 (*cb_rtn)(struct t_cb_data*), double delay, virtual_board* vboard);

///
void register_cb_at_last_known_delta(PLI_INT32 (*cb_rtn)(struct t_cb_data*), virtual_board* vboard);

/// Returns true if something went wrong with VPI
/// It prints the (error) message if any
bool check_error();

} // namespace vpi

#endif // VPI_HPP
