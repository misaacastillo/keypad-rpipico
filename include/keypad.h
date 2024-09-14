#ifndef KEYPAD_H
#define KEYPAD_H

#include "pico/stdlib.h"

// Definir el número de filas y columnas del teclado
#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4

// Declaración de las funciones de la biblioteca prototipos
void keypad_init(const uint8_t row_pins[KEYPAD_ROWS], const uint8_t col_pins[KEYPAD_COLS]);
char keypad_get_key(const uint8_t row_pins[KEYPAD_ROWS], const uint8_t col_pins[KEYPAD_COLS]);

#endif // KEYPAD_H
