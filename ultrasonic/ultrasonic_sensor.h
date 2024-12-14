#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include <stdint.h>

void ultrasonic_sensor_init(uint8_t trig_pin, uint8_t echo_pin);
void send_trigger_pulse(void);
float get_distance(void);

#endif // ULTRASONIC_SENSOR_H
