#include <stdint.h>
#include <stdbool.h>

#define SERVO_PIN 3

#define CLOCK_DIVIDER 64.0f // chosen to keep the wrap val smaller
#define WRAP_VAL 39062 // for 50Hz
/**
 * Want a 50 Hz PWM Signal
 * RP2040 default clock speed set at 125 MHz
 * Must divide clock speed to produce desired period 20 ms
 * PWM Freq 50 Hz = Clock Freq / (Clock Divider * (Wrap Val + 1))
*/
#define MAX_PULSE_WIDTH 2500
#define MIN_PULSE_WIDTH 500

void servo_init();

void servo_set_angle();