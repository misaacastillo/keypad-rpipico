#ifndef TEST_PICO_STDLIB_H
#define TEST_PICO_STDLIB_H

#include <stdbool.h>
#include <stdint.h>

typedef unsigned int uint;
typedef void (*gpio_irq_callback_t)(uint gpio, uint32_t events);

#define GPIO_IN false
#define GPIO_OUT true
#define GPIO_IRQ_EDGE_RISE (1u << 3)

void gpio_init(uint pin);
void gpio_deinit(uint pin);
void gpio_set_dir(uint pin, bool output);
void gpio_pull_down(uint pin);
void gpio_disable_pulls(uint pin);
void gpio_put(uint pin, bool value);
uint32_t gpio_get_all(void);
void gpio_set_irq_enabled_with_callback(uint pin, uint32_t events,
                                        bool enable, gpio_irq_callback_t callback);

#endif
