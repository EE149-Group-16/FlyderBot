#include <stdint.h>
#include <stdbool.h>

#define PROPELLER_PIN 3

#define CLOCK_DIVIDER 3 
#define WRAP_VAL 1999 // for 50Hz FOR 24kHZ?????
/**
 * Want a 50 Hz PWM Signal
 * RP2040 default clock speed set at 125 MHz
 * Must divide clock speed to produce desired period 20 ms
 * PWM Freq 24 kHz = Clock Freq / (Clock Divider * (Wrap Val + 1))
*/
#define MAX_PW 2000 // 2 ms.
#define MIN_PW 1000

void propeller_init();

void propeller_set_power(uint16_t power); // from 0 to 1.0?