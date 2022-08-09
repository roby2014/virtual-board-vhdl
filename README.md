### ⚠️ PROJECT UNDER DEVELOPMENT ⚠️


## Board emulator for VHDL code

This project aims to allow simulating VHDL circuits on an board emulator (so you dont need the "real" board).


The board IO pins (leds, buttons, switches, hex displays, etc...) can be configured via configuration file ([`assets/board.cfg`](https://github.com/roby2014/de10-emulator/blob/main/vhdl_vpi/assets/board.cfg)) and the signals assignments can be configured inside [`assets/assignments.cfg`](https://github.com/roby2014/de10-emulator/blob/main/vhdl_vpi/assets/assignments.cfg).

## Goals
[ ] Customizable board interface

[ ] USBPort integration (to enable e.g communication between Java/Kotlin/other modules with the emulator)

## Credits
- [ultralight](https://github.com/ultralight-ux/ultralight) - Next-generation HTML renderer for apps and games (C++)
- [GHDL](https://github.com/ghdl/ghdl) - VHDL 2008/93/87 simulator
- [ghdl-cosim](https://ghdl.github.io/ghdl-cosim/vpi/examples/index.html) - GHDL co-simulation projects using VPI
- [libconfig](http://hyperrealm.github.io/libconfig/) - Library for processing structured configuration files
