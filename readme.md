### ⚠️ PROJECT UNDER DEVELOPMENT ⚠️
*do not expect good documentation, clean and working code (for now)*

## Virtual Board for VHDL code simulation

This project aims to allow VHDL simulation on a "virtual board" (so a FPGA board won't be required), in a more friendly way (so you won't need to look at waveforms again!).

<img src="assets/goal.svg">

*(It's not an emulator since it won't emulate 100% real hardware)*

## VPI

The VHDL simulation is done via [GHDL](https://github.com/ghdl/ghdl), which runs a [`.vpi`](https://en.wikipedia.org/wiki/Verilog_Procedural_Interface) module built by `g++`.

We can manipulate the simulation signals via a websocket server, customize board IO pins and also assign signals to those pins, [read more about the VPI module here](https://github.com/roby2014/virtual-board-vhdl/tree/main/vhdl_vpi).


## UsbPort

One of the goals is also to add compatibility with [UsbPort](https://github.com/roby2014/virtual-board-vhdl/tree/main/UsbPort), which is a "communication" method that allows manipulating VHDL circuit signals via a Kotlin/Java application.

## Board UI

Since it has a websocket server, it won't be hard to display signals in a "pretty" way to the user.

[Here (gui_interface)](https://github.com/roby2014/virtual-board-vhdl/tree/main/gui_interface) is a quick UI prototype built with C++ to show what a board interface can look like.

## Credits
- [GHDL](https://github.com/ghdl/ghdl) - VHDL 2008/93/87 simulator
- [ghdl-cosim](https://ghdl.github.io/ghdl-cosim/vpi/examples/index.html) - GHDL co-simulation projects using VPI
- [libconfig](http://hyperrealm.github.io/libconfig/) - Library for processing structured configuration files
- [beast](https://github.com/boostorg/beast) - HTTP and WebSocket built on Boost.Asio in C++11 
- [ultralight](https://github.com/ultralight-ux/ultralight) - Next-generation HTML renderer for apps and games (C++)
- [Java-WebSocket](https://github.com/TooTallNate/Java-WebSocket) - A barebones WebSocket client and server implementation written in 100% Java.
