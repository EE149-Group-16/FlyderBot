#include "encoder.h"
#include "pico/stdlib.h"

// Global array to store encoder instances (if multiple encoders are used)
#define MAX_ENCODERS 4
static Encoder* encoders[MAX_ENCODERS] = {NULL};

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
    gpio_set_irq_enabled_with_callback(enc->gpio_a, GPIO_IRQ_EDGE_FALL, true, &encoder_callback);
    gpio_set_irq_enabled(enc->gpio_b, GPIO_IRQ_EDGE_FALL, true);

    return enc;
}