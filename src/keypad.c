#include "keypad.h"
#include "pico/stdlib.h"

#define DEBOUNCE_DELAY_MS 20  // Tiempo para el debounce

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
    // Iterar sobre cada fila
    for (int row = 0; row < KEYPAD_ROWS; row++) {
        // Poner todas las filas en alto antes de poner una en bajo
        for (int r = 0; r < KEYPAD_ROWS; r++) {
            gpio_put(row_pins[r], 1);
        }

        // Poner la fila actual en bajo
        gpio_put(row_pins[row], 0);
        sleep_us(10);  // Pausa corta para estabilizar el estado

        // Leer todas las columnas
        for (int col = 0; col < KEYPAD_COLS; col++) {
            if (gpio_get(col_pins[col])) {
                sleep_ms(DEBOUNCE_DELAY_MS);  // Debounce
                // Verificar si la tecla sigue presionada
                if (gpio_get(col_pins[col])) {
                    // Restaurar la fila a alto después de detectar la tecla
                    gpio_put(row_pins[row], 1);
                    return keys[row][col];  // Retornar la tecla presionada
                }
            }
        }

        // Restaurar la fila a alto antes de probar la siguiente
        gpio_put(row_pins[row], 1);
    }

    return 0;  // No se presionó ninguna tecla
}
