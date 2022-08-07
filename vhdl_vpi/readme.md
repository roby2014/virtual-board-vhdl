# VPI module
To compile and run the VPI plugin, you will need a recent version of `g++`, `GHDL` and `libconfig++-dev`.

If you want to test other VHDL code, you have to change `VHDL_FILES` and `EXEC_TOP_UNIT` inside the Makefile.

I have tested with:

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
$ apt-cache policy libconfig++-dev
libconfig++-dev:
  Installed: 1.5-0.4build1
  Candidate: 1.5-0.4build1
  Version table:
 *** 1.5-0.4build1 500
        500 http://archive.ubuntu.com/ubuntu focal/universe amd64 Packages
        100 /var/lib/dpkg/status
```

## Windows 10:
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

