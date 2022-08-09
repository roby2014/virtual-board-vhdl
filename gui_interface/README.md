# DE10-Lite emulator interface

## Current Status
<img src="https://github.com/roby2014/de10-emulator/blob/main/gui_interface/git_assets/prev.png" />

## Building from source 
## 1. Install the Prerequisites

Before you build and run, you'll need to [install Ultralight's prerequisites](https://docs.ultralig.ht/docs/installing-prerequisites) for your platform.

## 2. Build the app

To clone the repo and build, run the following:

```shell
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

> **Note**: _To force CMake to generate 64-bit projects on Windows, use `cmake .. -DCMAKE_GENERATOR_PLATFORM=x64` instead of `cmake ..`_

## 3. Run the app

### On macOS and Linux

Navigate to `ultralight-quick-start/build` and run `MyApp` to launch the program.

### On Windows

Navigate to `ultralight-quick-start/build/Release` and run `MyApp` to launch the program.
