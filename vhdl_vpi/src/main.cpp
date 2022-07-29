#include "vpi.hpp"
#include "vpi_user.h"

void (*vlog_startup_routines[])() = {vpi::cb_entry_point, 0};