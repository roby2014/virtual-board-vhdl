# VPI module
To compile and run the VPI plugin, you will need a recent version of `g++`, `GHDL`, `libconfig++-dev` and `libboost-dev`.

If you want to test other VHDL code, you have to change `VHDL_FILES` and `EXEC_TOP_UNIT` inside the Makefile.


# Configuration

You can configure your board PIN Set inside [`assets/board.cfg`](https://github.com/roby2014/de10-emulator/blob/main/vhdl_vpi/assets/board.cfg) and the signals assignments inside [`assets/assignments.cfg`](https://github.com/roby2014/de10-emulator/blob/main/vhdl_vpi/assets/assignments.cfg).
For more info read: 

## Custom PIN Set board configuration
This emulator allows having a custom PIN set, to add your desired pins, edit [`assets/board.cfg`](https://github.com/roby2014/de10-emulator/blob/main/vhdl_vpi/assets/board.cfg).
Each pin should have a `pin_name`, `pin_id` and its `index`.
For now, only `leds`, `switches`, (`hex` and `buttons` in the future) are allowed.

## Signal assignments
To assign your top entity VHDL signals to board PINS, edit [`assets/assignments.cfg`](https://github.com/roby2014/de10-emulator/blob/main/vhdl_vpi/assets/assignments.cfg).
The configuration should look pretty obvious if you open the file, but just in case, this is how it's done:
```
// This is a comment
// PIN_ID -> SIGNAL

PIN_A8 -> enable
PIN_A9 -> COUT
```

## Websocket server
Communication protocol:
```
GET <PIN_ID> - returns pin's signal value (1/0)
PUT <PIN_ID> <VALUE> - puts pin's signal value to value
```

# Test and run
I have only tested this project on these platforms:

## WSL Ubuntu 20.04.4 LTS (Focal Fossa):
Compile and run:
```
mkdir build
make
make exec
```

Dependencies (might work with other version):
```
$ g++ -v

gcc version 9.4.0 (Ubuntu 9.4.0-1ubuntu1~20.04.1)
```

```
$ ghdl -v

GHDL 3.0.0-dev (tarball) [Dunoon edition]
 Compiled with GNAT Version: 9.4.0
 mcode code generator
Written by Tristan Gingold.
```

```
$ dpkg -s libconfig++-dev | grep 'Version'
Version: 1.5-0.4build1
```

```
$ dpkg -s libboost-dev | grep 'Version'
Version: 1.71.0.0ubuntu2
```

## Windows 10 (not fully tested yet!):
(mingw64 needed)

Compile and run:
```
mkdir build
mingw32-make
mingw32-make exec
```

Dependencies (might work with other version):
```
> g++ 

gcc version 12.1.0 (x86_64-win32-seh-rev3, Built by MinGW-W64 project)
```

```
> ghdl -v 

GHDL 3.0.0-dev (2.0.0.r527.g539adbe0f) [Dunoon edition]
 Compiled with GNAT Version: 12.1.0
 mcode code generator
Written by Tristan Gingold.
```

