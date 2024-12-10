#include <stdio.h>
#include "pico/stdlib.h"
#include "ultrasonic_sensor.h"

#define TRIG_PIN 2  // GPIO pin for the Trig
#define ECHO_PIN 3  // GPIO pin for the Echo

int main() {
    stdio_init_all();

    // Initialize the ultrasonic sensor
    ultrasonic_sensor_init(TRIG_PIN, ECHO_PIN);

    while (1) {
        // Send a trigger pulse
        send_trigger_pulse();

        // Wait for the measurement to complete
        sleep_ms(60);  // Allow enough time for the echo to return

        // Get the distance
        float distance = get_distance();
        if (distance >= 0) {
            printf("Distance: %.2f cm\n", distance);
        } else {
            printf("Measurement in progress...\n");
        }

        sleep_ms(500);  // Wait before the next measurement
    }

    return 0;
}
