#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "EncUS.h"
#define MAX_SENSORS 4
// Global array to store encoder instances (if multiple encoders are used)
#define MAX_ENCODERS 4
static Encoder* encoders[MAX_ENCODERS] = {NULL};

static HC_SR04_t *sensor_array[MAX_SENSORS];
static int sensor_count = 0;



static bool trigger_pulse_timer_callback(repeating_timer_t *rt) {
    HC_SR04_t *sensor = (HC_SR04_t *)rt->user_data;
    gpio_put(sensor->trig_pin, 0);
    return false;
}


static void echo_edge_callback(uint gpio, uint32_t events) {
    for (int i = 0; i < sensor_count; i++) {
        if (sensor_array[i]->echo_pin == gpio) {
            HC_SR04_t *sensor = sensor_array[i];
            if (events & GPIO_IRQ_EDGE_RISE) {
                sensor->start_time = time_us_32();
            } else if (events & GPIO_IRQ_EDGE_FALL) {
                sensor->end_time = time_us_32();
                sensor->echo_done = true;
            }
            break;
        }
    }
}

// Encoder interrupt callback function
void encoder_callback(uint gpio, uint32_t events) {
    // Identify which encoder triggered the callback
    for (int i = 0; i < MAX_ENCODERS; i++) {
        if (encoders[i] != NULL && (gpio == encoders[i]->gpio_a || gpio == encoders[i]->gpio_b)) {
            Encoder* enc = encoders[i];
            uint32_t gpio_state = (gpio_get_all() >> enc->gpio_a) & 0b0111; // Mask GPIO states
            static bool ccw_fall[MAX_ENCODERS] = {0}; // Per-encoder CCW fall states
            static bool cw_fall[MAX_ENCODERS] = {0};  // Per-encoder CW fall states
            uint8_t enc_value = gpio_state & 0x03;
            if (gpio == enc->gpio_a) {
                if (!cw_fall[i] && enc_value == 0b10) cw_fall[i] = true;

                if (ccw_fall[i] && enc_value == 0b00) {
                    cw_fall[i] = false;
                    ccw_fall[i] = false;
                    enc->position--;
                    enc->updated = true;
                }
            } 

            if (gpio == enc->gpio_b) {
                if (!ccw_fall[i] && enc_value == 0b01) ccw_fall[i] = true;

                if (cw_fall[i] && enc_value == 0b00) {
                    cw_fall[i] = false;
                    ccw_fall[i] = false;
                    enc->position++;
                    enc->updated = true;
                }
            }
            return; // Stop once we've handled the correct encoder
        }
    }
}


void shared_callback(uint gpio, uint32_t events) {
    if (gpio == 14 || gpio == 15 || gpio == 10 || gpio == 11) {
        echo_edge_callback(gpio, events);
    } else if (gpio == 7 || gpio == 8 || gpio == 13|| gpio == 12) {
        encoder_callback(gpio, events);
    }
}

// Initialize encoder GPIOs and interrupts
Encoder* encoder_init(uint gpio_a, uint gpio_b) {
    // Create and initialize an Encoder instance
    Encoder* enc = (Encoder*)malloc(sizeof(Encoder));
    if (enc == NULL) return NULL;

    enc->gpio_a = gpio_a;           // printf("enc position in intr %d\n", enc->position);
    enc->gpio_b = gpio_b;
    enc->updated = false;
    enc->position = 0;

    // Add the encoder to the global array
    for (int i = 0; i < MAX_ENCODERS; i++) {
        if (encoders[i] == NULL) {
            encoders[i] = enc;
            break;
        }
    }

    // Initialize GPIO pins

    gpio_init(enc->gpio_a);
    gpio_set_dir(enc->gpio_a, GPIO_IN);
    gpio_disable_pulls(enc->gpio_a);

    gpio_init(enc->gpio_b);
    gpio_set_dir(enc->gpio_b, GPIO_IN);
    gpio_disable_pulls(enc->gpio_b);

    // Enable interrupts
    gpio_set_irq_enabled_with_callback(enc->gpio_a, GPIO_IRQ_EDGE_FALL, true, &shared_callback);
    gpio_set_irq_enabled(enc->gpio_b, GPIO_IRQ_EDGE_FALL, true);

    return enc;
}




HC_SR04_t *HC_SR04_init(uint8_t trig_pin, uint8_t echo_pin) {
    HC_SR04_t *sensor = malloc(sizeof(HC_SR04_t));
    if (!sensor) {
        printf("Failed to allocate memory for HC_SR04 sensor.\n");
        return NULL;
    }
    sensor->trig_pin = trig_pin;
    sensor->echo_pin = echo_pin;
    sensor->start_time = 0;
    sensor->end_time = 0;
    sensor->echo_done = false;

    gpio_init(sensor->trig_pin);
    gpio_set_dir(sensor->trig_pin, GPIO_OUT);
    gpio_put(sensor->trig_pin, 0);

    gpio_init(sensor->echo_pin);
    gpio_set_dir(sensor->echo_pin, GPIO_IN);
    gpio_set_irq_enabled_with_callback(sensor->echo_pin, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, shared_callback);

    sensor_array[sensor_count++] = sensor;
    return sensor;
}

void HC_SR04_send_trigger_pulse(HC_SR04_t *sensor) {
    gpio_put(sensor->trig_pin, 1);
    static repeating_timer_t timer;
    add_repeating_timer_us(-10, trigger_pulse_timer_callback, sensor, &timer);
}

float HC_SR04_get_distance(HC_SR04_t *sensor) {
    if (!sensor->echo_done) {
        return -1.0f;
    }
    sensor->echo_done = false;
    uint32_t pulse_width = sensor->end_time - sensor->start_time;
    return (pulse_width / 2.0f) * 0.0343f;
}
