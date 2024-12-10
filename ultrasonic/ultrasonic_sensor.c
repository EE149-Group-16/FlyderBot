#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "ultrasonic_sensor.h"

// Static variables for internal use
static uint8_t trig_pin;  // TRIG pin number
static uint8_t echo_pin;  // ECHO pin number
static volatile uint32_t start_time = 0;
static volatile uint32_t end_time = 0;
static volatile bool echo_done = false;

// Timer callback to set TRIG pin LOW
static bool trigger_pulse_timer_callback(repeating_timer_t *rt) {
    gpio_put(trig_pin, 0);  // Set TRIG pin LOW
    return false;           // Single-shot timer, stop repeating
}

// GPIO interrupt callback for ECHO pin
static void echo_edge_callback(uint gpio, uint32_t events) {
    if (events & GPIO_IRQ_EDGE_RISE) {
        start_time = time_us_32();  // Capture the start time
    } else if (events & GPIO_IRQ_EDGE_FALL) {
        end_time = time_us_32();    // Capture the end time
        echo_done = true;           // Signal measurement completion
    }
}

// Initialize the ultrasonic sensor
void ultrasonic_sensor_init(uint8_t trig, uint8_t echo) {
    trig_pin = trig;
    echo_pin = echo;

    // Initialize TRIG pin
    gpio_init(trig_pin);
    gpio_set_dir(trig_pin, GPIO_OUT);
    gpio_put(trig_pin, 0);

    // Initialize ECHO pin
    gpio_init(echo_pin);
    gpio_set_dir(echo_pin, GPIO_IN);

    // Set up GPIO interrupts for ECHO pin
    gpio_set_irq_enabled_with_callback(echo_pin, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, echo_edge_callback);
}

// Send a non-blocking trigger pulse
void send_trigger_pulse(void) {
    gpio_put(trig_pin, 1);  // Set TRIG pin HIGH

    // Configure a one-shot timer for 10 microseconds
    static repeating_timer_t timer;
    add_repeating_timer_us(-10, trigger_pulse_timer_callback, NULL, &timer);
}

// Get the measured distance in cm
float get_distance(void) {
    if (!echo_done) {
        return -1.0f;  // Return -1 if measurement is not ready
    }

    echo_done = false;  // Reset the flag
    uint32_t pulse_width = end_time - start_time;
    return (pulse_width / 2.0f) * 0.0343f;  // Calculate distance in cm
}
