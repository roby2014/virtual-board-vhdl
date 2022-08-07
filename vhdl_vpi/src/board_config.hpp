#include "libconfig.h++"

namespace board_config {

/// opens "board.cfg" and stores peripherals pins into a generic collection
/// returns false in case of fatal error
bool get_board_config();

/// gets peripheral info from [root] cfg by its name
/// TODO: for now it only prints info, sooner it should store/return
// a collection with the items found
void get_peripheral(const libconfig::Setting& root, const char* peripheral);

} // namespace board_config
