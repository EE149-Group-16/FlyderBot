#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>
#include <stdbool.h>
#include "hardware/gpio.h"

// Encoder structure to hold state for each encoder
typedef struct {
    uint gpio_a;          // GPIO pin for Phase A
    uint gpio_b;          // GPIO pin for Phase B
    volatile int position; // Encoder position
    volatile bool updated; // Encoder updated flag
} Encoder;

// Function declarations
Encoder* encoder_init(uint gpio_a, uint gpio_b);
void encoder_callback(uint gpio, uint32_t events);

#endif // ENCODER_H
