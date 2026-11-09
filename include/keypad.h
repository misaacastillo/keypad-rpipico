/*
 * API derived from pico-keypad4x4 by Oswaldo Hernandez and Owen Jauregui.
 * Copyright (c) 2023 Oswaldo Hernandez
 * Copyright (c) 2024 Misael Castillo (adaptation)
 * SPDX-License-Identifier: MIT
 */
#ifndef PICO_MATRIX_KEYPAD_H
#define PICO_MATRIX_KEYPAD_H

#include "pico/stdlib.h"

/* Supply eight distinct RP2040 GPIO numbers and 16 nonzero row-major labels.
 * Disable column IRQs before reinitializing or remapping the pins. */
void pico_keypad_init(uint columns[4], uint rows[4], char matrix_values[16]);

/* Returns one key, or NUL for idle or ambiguous input. Poll outside an IRQ. */
char pico_keypad_get_key(void);

/* Optional column edge notifications using the SDK's per-core GPIO callback. */
void pico_keypad_irq_enable(bool enable, gpio_irq_callback_t callback);

#endif
