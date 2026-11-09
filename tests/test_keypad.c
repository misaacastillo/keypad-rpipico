#include <assert.h>
#include <stdio.h>

#include "keypad.h"

#define SIDE 4
#define GPIO_COUNT 30

struct pin_state {
    bool configured;
    bool output;
    bool latch;
    bool pull_down;
    bool irq_enabled;
};

static struct pin_state pins[GPIO_COUNT];
static uint *wired_rows;
static uint *wired_columns;
static uint32_t external_high;
static uint16_t pressed;
static unsigned int scans;

void gpio_init(uint pin) {
    pins[pin] = (struct pin_state){.configured = true};
}

void gpio_deinit(uint pin) {
    pins[pin] = (struct pin_state){0};
}

void gpio_set_dir(uint pin, bool output) {
    assert(pins[pin].configured);
    pins[pin].output = output;
}

void gpio_pull_down(uint pin) {
    pins[pin].pull_down = true;
}

void gpio_disable_pulls(uint pin) {
    pins[pin].pull_down = false;
}

void gpio_put(uint pin, bool value) {
    pins[pin].latch = value;
}

uint32_t gpio_get_all(void) {
    uint32_t value = external_high;
    if (wired_rows == NULL) {
        return value;
    }
    for (uint col = 0; col < SIDE; ++col) {
        bool driven_high = false;
        bool driven_low = false;
        for (uint row = 0; row < SIDE; ++row) {
            struct pin_state pin = pins[wired_rows[row]];
            if ((pressed & (1u << (row * SIDE + col))) && pin.output) {
                driven_high |= pin.latch;
                driven_low |= !pin.latch;
            }
        }
        assert(!(driven_high && driven_low));
        if (driven_high) {
            value |= UINT32_C(1) << wired_columns[col];
        }
    }
    return value;
}

void busy_wait_us(uint64_t delay) {
    assert(delay > 0);
    unsigned int driven = 0;
    for (uint row = 0; row < SIDE; ++row) {
        struct pin_state pin = pins[wired_rows[row]];
        assert(pin.latch && !pin.pull_down);
        driven += pin.output;
    }
    assert(driven == 1);
    ++scans;
}

void gpio_set_irq_enabled_with_callback(uint pin, uint32_t events,
                                        bool enable, gpio_irq_callback_t callback) {
    assert(events == GPIO_IRQ_EDGE_RISE);
    assert(!enable || callback != NULL);
    pins[pin].irq_enabled = enable;
}

static void configure(uint rows[4], uint columns[4], char labels[16]) {
    wired_rows = rows;
    wired_columns = columns;
    pico_keypad_init(columns, rows, labels);
    for (uint i = 0; i < SIDE; ++i) {
        assert(pins[columns[i]].pull_down && !pins[columns[i]].output);
        assert(pins[rows[i]].output && pins[rows[i]].latch);
    }
}

static void check_each_key(const char labels[16]) {
    for (uint i = 0; i < SIDE * SIDE; ++i) {
        pressed = (uint16_t)(1u << i);
        assert(pico_keypad_get_key() == labels[i]);
        for (uint row = 0; row < SIDE; ++row) {
            assert(pins[wired_rows[row]].output && pins[wired_rows[row]].latch);
        }
    }
    pressed = 0;
}

static void on_column_rise(uint pin, uint32_t events) {
    (void)pin;
    (void)events;
}

int main(void) {
    uint rows[4] = {2, 3, 4, 5};
    uint columns[4] = {6, 7, 8, 9};
    uint new_rows[4] = {10, 11, 12, 13};
    uint new_columns[4] = {14, 15, 16, 17};
    char labels[16] = "123A456B789C*0#D";
    char new_labels[16] = "abcdefghijklmnop";

    assert(pico_keypad_get_key() == '\0');
    configure(rows, columns, labels);
    assert(pico_keypad_get_key() == '\0' && scans == 0);
    check_each_key(labels);

    configure(rows, columns, labels);
    check_each_key(labels);

    pressed = 0x0003;
    assert(pico_keypad_get_key() == '\0'); /* Same row, two columns. */
    pressed = 0x0011;
    assert(pico_keypad_get_key() == '\0'); /* Same column, two rows. */
    pressed = 0x0021;
    assert(pico_keypad_get_key() == '\0'); /* Two rows and two columns. */
    pressed = 0;
    for (uint row = 0; row < SIDE; ++row) {
        assert(pins[rows[row]].output && pins[rows[row]].latch);
    }

    pico_keypad_irq_enable(true, on_column_rise);
    for (uint col = 0; col < SIDE; ++col) {
        assert(pins[columns[col]].irq_enabled);
    }
    pico_keypad_irq_enable(false, NULL);
    for (uint col = 0; col < SIDE; ++col) {
        assert(!pins[columns[col]].irq_enabled);
    }

    configure(new_rows, new_columns, new_labels);
    for (uint i = 0; i < SIDE; ++i) {
        assert(!pins[rows[i]].configured && !pins[columns[i]].configured);
    }
    external_high = UINT32_C(1) << columns[0];
    unsigned int scans_before = scans;
    assert(pico_keypad_get_key() == '\0' && scans == scans_before);
    check_each_key(new_labels);

    puts("Keypad tests passed: 16 positions, re-init, remap, ambiguous input and GPIO state");
}
