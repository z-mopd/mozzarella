# Mozzarella
Also abbreviated as mzr, Mozzarella is a work-in-progress, (somewhat) self-contained rendering library supporting some immediate-mode rendering. If you just take even a quick glance at it, you'll see that raylib inspired the style of mzr's immediate-mode rendering.

Retained-mode rendering is planned.

## Compiling
#### Disclaimer!
- Mozzarella has only been tested on Linux Ubuntu (X11), so there are no guarantees for Wayland or other distros. Though they should hopefully work.

#### Using CMake
Building Mozzarella is straightforward and follows the regular CMake process.
```sh
mkdir build && cd build
cmake ..
cmake --build .
```

## Options
#### BUILD_TESTS

Build tests/examples for Mozzarella
- Default: OFF
- Usage:
```sh
cmake -DBUILD_TESTS=ON ..
```

#### COMPILE_COMMANDS

Export compile commands for Mozzarella
- Default: OFF
- Usage:
```sh
cmake -DCOMPILE_COMMANDS=ON ..
```

#### BACKEND

Specifies what backend is used when Mozzarella is compiled
- Available Values:
  - OPENGL
  - VULKAN
- Default: OPENGL
- Usage:
```sh
cmake -DBACKEND=VULKAN ..
```
