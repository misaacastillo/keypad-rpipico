# 4×4 matrix keypad for Raspberry Pi Pico

A C driver for a passive 4×4 matrix keypad on RP2040, with a USB terminal
example and host tests. It derives from
[pico-keypad4x4](https://github.com/hhoswaldo/pico-keypad4x4) by Oswaldo Hernandez
and Owen Jauregui.

The column mask is rebuilt when the driver is initialized again. During a scan,
one row drives high and the other rows are high impedance. Columns have internal
pull-downs. The driver reports a single key and returns NUL for idle or
ambiguous input. The polling example prints a key on a change in reported
state. It does not implement software debounce.

## Build

Install CMake 3.17 or newer, Ninja, Python 3 and an Arm GNU toolchain with
newlib. Clone the Pico SDK 2.2.0 and its TinyUSB submodule.

```sh
git clone --branch 2.2.0 --depth 1 https://github.com/raspberrypi/pico-sdk.git ../pico-sdk-2.2.0
git -C ../pico-sdk-2.2.0 submodule update --init --depth 1 lib/tinyusb
export PICO_SDK_PATH="$(cd ../pico-sdk-2.2.0 && pwd)"
cmake -S . -B build-pico -G Ninja -DPICO_BOARD=pico
cmake --build build-pico
```

The SDK may fetch picotool on the first build. The output is
`build-pico/key_terminal.uf2`. Use `-DPICO_BOARD=pico_w` in a separate build
directory for Pico W.

## Run the terminal example

With the Pico unplugged, wire the passive keypad's rows to GP2–GP5 and its
columns to GP6–GP9 in order. Identify rows and columns using the keypad's pinout.
The example expects eight switch-matrix wires. Connect no supply rail to the
keypad.

Enter BOOTSEL mode and copy the board-specific UF2 to the USB drive. Open the
Pico's USB CDC serial port and press one key. For example, pressing the
top-left key prints `Key 1`. Board builds and host simulations are documented
below. Physical matrix and USB behavior should be verified on the bench.

## Reuse the driver

Add `src/keypad.c` and the `include` directory to a Pico SDK project, or link
the `pico_matrix_keypad` target. Pass eight distinct, exposed RP2040 GPIO numbers
and a row-major array of 16 nonzero labels to `pico_keypad_init`.

The driver stores one keypad configuration. When idle, all row outputs are
high. A scan takes up to about 40 ms on a detected press. Use one key at a time
and call the scan from the main loop. The optional IRQ API uses the Pico SDK's
shared callback per core, and a scan can itself generate column edges. Disable
column IRQs before reinitializing or changing pins.

## Host tests

```sh
mkdir -p build-host
cc -std=c11 -Wall -Wextra -Werror -pedantic -Itests/fake-sdk -Iinclude \
  src/keypad.c tests/test_keypad.c -o build-host/test_keypad
./build-host/test_keypad
```

The fake GPIO model checks all 16 key positions, repeated initialization,
remapping with stale-pin noise, ambiguous presses, idle, and GPIO direction
and latch state during a scan. It checks simulated logic rather than analog
settling, key bounce or electrical behavior.

## License

The current source, terminal example, tests and documentation use the
[MIT License](LICENSE). See [provenance](PROVENANCE.md) for the upstream
reference and the scope of the historical examples.
