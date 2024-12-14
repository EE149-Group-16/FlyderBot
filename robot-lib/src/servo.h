#include <stdint.h>
#include <stdbool.h>


#define CLOCK_DIVIDER 125 // chosen to keep the wrap val smaller
#define WRAP_VAL 19999 // for 50Hz
/**
 * Want a 50 Hz PWM Signal
 * RP2040 default clock speed set at 125 MHz
 * Must divide clock speed to produce desired period 20 ms
 * PWM Freq 50 Hz = Clock Freq / (Clock Divider * (Wrap Val + 1))
 * 
*/

// typedef struct {
//     uint32_t max_pw;
// } servo_t;

#define MAX_PW_Servo3 2725 // FOR SERVO #3
#define MIN_PW_Servo3 500 // FOR SERVO #3
#define SERVO_PIN_3 3

#define MAX_PW_Servo2 2725
#define MIN_PW_Servo2 450
#define SERVO_PIN_2 2

#define MAX_PW_Servo1 2725
#define MIN_PW_Servo1 500
#define SERVO_PIN_1 1

void servo_init(int servoNum);
void servo_set_angle(uint16_t angle, int servoNum);