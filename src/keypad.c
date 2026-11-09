/*
 * Derived from pico-keypad4x4 by Oswaldo Hernandez and Owen Jauregui.
 * Copyright (c) 2023 Oswaldo Hernandez
 * Copyright (c) 2026 Misael Castillo (modifications)
 * SPDX-License-Identifier: MIT
 */
#include "keypad.h"

#include "hardware/timer.h"

#define SIDE 4
#define KEY_COUNT (SIDE * SIDE)

static uint column_pins[SIDE];
static uint row_pins[SIDE];
static uint32_t column_masks[SIDE];
static uint32_t all_columns_mask;
static char keymap[KEY_COUNT];

void pico_keypad_init(uint columns[4], uint rows[4], char matrix_values[16]) {
    if (all_columns_mask != 0) {
        for (uint i = 0; i < SIDE; ++i) {
            gpio_disable_pulls(column_pins[i]);
            gpio_deinit(column_pins[i]);
            gpio_deinit(row_pins[i]);
        }
    }

    all_columns_mask = 0;
    for (uint i = 0; i < KEY_COUNT; ++i) {
        keymap[i] = matrix_values[i];
    }

    for (uint i = 0; i < SIDE; ++i) {
        column_pins[i] = columns[i];
        row_pins[i] = rows[i];
        column_masks[i] = UINT32_C(1) << column_pins[i];
        all_columns_mask |= column_masks[i];

        gpio_init(column_pins[i]);
        gpio_pull_down(column_pins[i]);
        gpio_init(row_pins[i]);
        gpio_disable_pulls(row_pins[i]);
        gpio_put(row_pins[i], true);
        gpio_set_dir(row_pins[i], GPIO_OUT);
    }
}

char pico_keypad_get_key(void) {
    if ((gpio_get_all() & all_columns_mask) == 0) {
        return '\0';
    }

    /* All row latches stay high. Inactive rows float instead of driving low. */
    for (uint i = 0; i < SIDE; ++i) {
        gpio_set_dir(row_pins[i], GPIO_IN);
    }

    int key = -1;
    for (uint row = 0; row < SIDE; ++row) {
        gpio_set_dir(row_pins[row], GPIO_OUT);
        busy_wait_us(10000);
        uint32_t active = gpio_get_all() & all_columns_mask;
        gpio_set_dir(row_pins[row], GPIO_IN);

        if (active == 0) {
            continue;
        }
        if (key >= 0 || (active & (active - 1)) != 0) {
            key = -1;
            break;
        }
        for (uint col = 0; col < SIDE; ++col) {
            if (active == column_masks[col]) {
                key = (int)(row * SIDE + col);
                break;
            }
        }
    }

    for (uint i = 0; i < SIDE; ++i) {
        gpio_set_dir(row_pins[i], GPIO_OUT);
    }

    return key < 0 ? '\0' : keymap[key];
}

void pico_keypad_irq_enable(bool enable, gpio_irq_callback_t callback) {
    if (all_columns_mask == 0) {
        return;
    }
    for (uint i = 0; i < SIDE; ++i) {
        gpio_set_irq_enabled_with_callback(column_pins[i], GPIO_IRQ_EDGE_RISE,
                                           enable, callback);
    }
}
