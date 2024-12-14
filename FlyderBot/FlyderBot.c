#include <stdio.h>
#include "pico/stdlib.h"
#include "encoder.h"

int main() {
    sleep_ms(500);
    stdio_init_all();

    // Initialize two encoders with different GPIO pins
    Encoder* encoder1 = encoder_init(10, 11); // First encoder
    Encoder* encoder2 = encoder_init(13, 14); // Second encoder

    while (1) {
        // Check and print encoder1 status
        if (encoder1->updated) {
            encoder1->updated = false;
            printf("Encoder 1 Position: %d\n", encoder1->position);
        }

        // Check and print encoder2 status
        if (encoder2->updated) {
            encoder2->updated = false;
            printf("Encoder 2 Position: %d\n", encoder2->position);
        }
    }
}
