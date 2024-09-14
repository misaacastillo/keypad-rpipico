/*
 * Copyright (c) 2024 Misael Castillo <misacastillo08@gmail.com>
*/

#ifndef _KEYPAD_H_
#define _KEYPAD_H_

#include "pico/stdlib.h"
#include "hardware/timer.h"

// prototipos de funciones

void pico_keypad_init(uint columns[4], uint rows[4], char matrix_values[16]);
char pico_keypad_get_key(void);
void pico_keypad_irq_enable(bool enable, gpio_irq_callback_t callback);

#endif

