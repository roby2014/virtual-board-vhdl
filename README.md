### ⚠️ PROJECT UNDER DEVELOPMENT ⚠️
*do not expect good documentation, clean and working code (for now)*

## Board emulator for VHDL code

This project aims to allow simulating VHDL circuits on a board emulator (so you dont need the "real" FPGA board).

The VHDL simulation is done via [GHDL](https://github.com/ghdl/ghdl), which runs a [`.vpi`](https://en.wikipedia.org/wiki/Verilog_Procedural_Interface) module built by `g++` from the C++ code.

The board IO pins (leds, buttons, switches, hex displays, etc...) can be set/configured via configuration file ([`assets/board.cfg`](https://github.com/roby2014/de10-emulator/blob/main/vhdl_vpi/assets/board.cfg)) and the signals assignments can be configured inside [`assets/assignments.cfg`](https://github.com/roby2014/de10-emulator/blob/main/vhdl_vpi/assets/assignments.cfg).

It also has a websocket server implemented which means it is easy to access to all the pins/signals via other applications, this will make the interface implementation process way easier.



*Soon I will show examples/documentate how the configuration is made, communication protocol works, etc..*

## Credits
- [ultralight](https://github.com/ultralight-ux/ultralight) - Next-generation HTML renderer for apps and games (C++)
- [GHDL](https://github.com/ghdl/ghdl) - VHDL 2008/93/87 simulator
- [ghdl-cosim](https://ghdl.github.io/ghdl-cosim/vpi/examples/index.html) - GHDL co-simulation projects using VPI
- [libconfig](http://hyperrealm.github.io/libconfig/) - Library for processing structured configuration files
- [beast](https://github.com/boostorg/beast) - HTTP and WebSocket built on Boost.Asio in C++11 
