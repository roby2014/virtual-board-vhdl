#include "vpi.hpp"

void (*vlog_startup_routines[])() = {vpi::cb_entry_point, 0};
