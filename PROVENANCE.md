# Code provenance

The scanner and public API derive from
[`hhoswaldo/pico-keypad4x4` at `8061fd7`](https://github.com/hhoswaldo/pico-keypad4x4/tree/8061fd7ffdd544a18ddbfd40738a579f4f756034).
Its source credits Oswaldo Hernandez and Owen Jauregui. The [license](LICENSE)
retains the upstream MIT terms and copyright notice and identifies Misael
Castillo's adaptation. The matching scanner originally appeared in
[the September 2024 update](https://github.com/misaacastillo/keypad-rpipico/commit/e2beee1df4a806b81174a897cb89230d1144c6f8).

This revision modifies the scanner to reset the column mask on initialization
and to put inactive rows in high impedance during a scan. It includes a
rewritten terminal example, Pico SDK build configuration and host tests.
The [upstream header](https://github.com/hhoswaldo/pico-keypad4x4/blob/8061fd7ffdd544a18ddbfd40738a579f4f756034/pico_keypad4x4.h)
is the reference for the retained function signatures.

The old keypad photograph, LCD example and PlatformIO setup remain in the
repository's history. Their redistribution scope was not established for this
release. This revision includes the scanner, a standalone terminal example
and the files needed to build and verify them.
