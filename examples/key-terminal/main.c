#include <stdio.h>

#include "keypad.h"

int main(void) {
    uint row_pins[4] = {2, 3, 4, 5};
    uint column_pins[4] = {6, 7, 8, 9};
    char labels[16] = "123A456B789C*0#D";
    char previous = '\0';

    stdio_init_all();
    pico_keypad_init(column_pins, row_pins, labels);

    while (true) {
        char key = pico_keypad_get_key();
        if (key != '\0' && key != previous) {
            printf("Key %c\n", key);
        }
        previous = key;
        sleep_ms(20);
    }
}
