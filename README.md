# Mozzarella
Also abbreviated as mzr, Mozzarella is a work-in-progress, (somewhat) self-contained rendering library supporting some immediate-mode rendering. If you just take even a quick glance at it, you'll see that raylib inspired the style of mzr's immediate-mode rendering.

Retained-mode rendering is planned.

## Building
#### Disclaimer!
- Mozzarella has only been tested on Linux Ubuntu (X11), so there are no guarantees for Wayland or other distros. Though they should hopefully work.

#### Using CMake
Building Mozzarella is straightforward and follows the regular CMake process.
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

#### Tests
- -DBUILD_TESTS
  - Build tests/examples for mzr
