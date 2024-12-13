#ifndef PROPELLER_H
#define PROPELLER_H

#include <stdint.h>
#include <stdbool.h>


#define WRAP_VAL 20000 // for 50Hz FOR 24kHZ?????
/**
 * Want a 24 kHz PWM Signal
 * RP2040 default clock speed set at 125 MHz
 * Must divide clock speed to produce desired period 20 ms
 * PWM Freq 24 kHz = Clock Freq / (Clock Divider * (Wrap Val + 1))
*/
#define MAX_PW 2000 // 2 ms.
// 1500 is zero??
#define MIN_PW 1000
// #define PROPELLER_PIN_REAR 

typedef struct {
    uint32_t slice;
    uint32_t channel;
    uint8_t pin;
} propeller_t;

propeller_t* propeller_init(uint8_t pin);
void propeller_arm_seq(propeller_t* prop_front, propeller_t* prop_rear);

void propeller_set_power(propeller_t* prop, uint16_t power_level); // from 0 to 1.0?

#endif // PROPELLER_H