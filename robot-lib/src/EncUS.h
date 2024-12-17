#ifndef hc_sr04_H
#define hc_sr04_H

#include <stdint.h>
#include <stdbool.h>
#include "hardware/gpio.h"

typedef struct {
    uint8_t trig_pin;
    uint8_t echo_pin;
    volatile uint32_t start_time;
    volatile uint32_t end_time;
    volatile bool echo_done;
} HC_SR04_t;

typedef struct {
    uint gpio_a;          // GPIO pin for Phase A
    uint gpio_b;          // GPIO pin for Phase B
    volatile int position; // Encoder position
    volatile bool updated; // Encoder updated flag
} Encoder;


HC_SR04_t *HC_SR04_init(uint8_t trig_pin, uint8_t echo_pin);
void HC_SR04_send_trigger_pulse(HC_SR04_t *sensor);
float HC_SR04_get_distance(HC_SR04_t *sensor);


// Function declarations
Encoder* encoder_init(uint gpio_a, uint gpio_b);
void encoder_callback(uint gpio, uint32_t events);

#endif // ENCODER_H