#include "keypad.h"
#include "pico/stdlib.h"

// Mapeo de teclas del teclado 4x4
static char keys[KEYPAD_ROWS][KEYPAD_COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

// Función para inicializar las filas y columnas del teclado
void keypad_init(const uint8_t row_pins[KEYPAD_ROWS], const uint8_t col_pins[KEYPAD_COLS]) {
    // Configurar pines de las filas como salidas
    for (int i = 0; i < KEYPAD_ROWS; i++) {
        gpio_init(row_pins[i]);
        gpio_set_dir(row_pins[i], GPIO_OUT);
        gpio_put(row_pins[i], 1); // Colocar en alto inicialmente
    }

    // Configurar pines de las columnas como entradas con pull-down
    for (int i = 0; i < KEYPAD_COLS; i++) {
        gpio_init(col_pins[i]);
        gpio_set_dir(col_pins[i], GPIO_IN);
        gpio_pull_down(col_pins[i]); // Habilitar pull-down para detectar pulsaciones
    }
}

// Función para obtener la tecla presionada
char keypad_get_key(const uint8_t row_pins[KEYPAD_ROWS], const uint8_t col_pins[KEYPAD_COLS]) {
    for (int row = 0; row < KEYPAD_ROWS; row++) {
        // Poner la fila actual en bajo
        gpio_put(row_pins[row], 0);
        
        // Leer todas las columnas
        for (int col = 0; col < KEYPAD_COLS; col++) {
            if (gpio_get(col_pins[col])) {
                // Si se detecta una pulsación, restaurar el estado de la fila
                gpio_put(row_pins[row], 1);
                return keys[row][col];  // Retornar la tecla presionada
            }
        }
        
        // Restaurar la fila a alto después de leerla
        gpio_put(row_pins[row], 1);
    }

    return 0;  // No se presionó ninguna tecla
}
