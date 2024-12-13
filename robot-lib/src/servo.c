/**
 * #Single Library
 * lfc -r ~/mylibraries/custom_lib your_program.lfc
 * #Multiple libraries
 * lfc -r ~/lib -r ~/lib2 your_program.lfc
*/

#include <servo.h>
#include <pico/stdlib.h>
#include <hardware/pwm.h>

// struct *servo_t servo_init(uint8_t pin, uint32_t max_pw, ) {
//     servo_t *servo = malloc(sizeof(servo_t));
    

void servo_init(int servoNum) {
    int SERVO_PIN;
    if (servoNum == 1) {
        SERVO_PIN = SERVO_PIN_1;
    } else if (servoNum == 2)
    {
        SERVO_PIN = SERVO_PIN_2;
    } else
    {
        SERVO_PIN = SERVO_PIN_3;
    }
    
    gpio_init(SERVO_PIN);
    gpio_set_dir(SERVO_PIN, GPIO_OUT);

    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM); //from youtube DrJonea.co.uk servos rasberry pi pico

    // configure PWM slice and get slice num that controls speed
    uint32_t slice = pwm_gpio_to_slice_num(SERVO_PIN);
    pwm_config cfg = pwm_get_default_config();
    pwm_config_set_clkdiv(&cfg, CLOCK_DIVIDER);
    pwm_config_set_wrap(&cfg, WRAP_VAL);
    pwm_init(slice, &cfg, true);
}

// void servo_set_angle(uint16_t angle) {
//     // CODE FOR TESTING WHEN TAKING IN PULSE WIDTH, NOT ANGLE
//     pwm_set_gpio_level(SERVO_PIN, angle);
//     sleep_ms(20);
// }

// void servo_set_angle(servo_t* servo, uint16_t angle) {

void servo_set_angle(uint16_t angle, int servoNum) {    
    int MIN_PW;
    int MAX_PW;
    int SERVO_PIN;
    if (servoNum == 1) {
        MIN_PW = MIN_PW_Servo1;
        MAX_PW = MAX_PW_Servo1;
        SERVO_PIN = SERVO_PIN_1;
    } else if (servoNum == 2)
    {
        MIN_PW = MIN_PW_Servo2;
        MAX_PW = MAX_PW_Servo2;
        SERVO_PIN = SERVO_PIN_2;
    } else
    {
        MIN_PW = MIN_PW_Servo3;
        MAX_PW = MAX_PW_Servo3;
        SERVO_PIN = SERVO_PIN_3;
    }

    if (angle > 180) angle = 180; // prevent going past max angle
    if (angle < 0) angle = 0;
    // int pulse_width = angle * ((servo->max_pw - MIN_PW) / 180) + MIN_PW;
    int pulse_width = angle * ((MAX_PW - MIN_PW) / 180) + MIN_PW;
    // printf("pulse_width inside func, %d, pulse_width);
    // printf("angle inside func, %d", angle")
    // pwm_set_gpio_level(servo->pin, pulse_width);
    pwm_set_gpio_level(SERVO_PIN, pulse_width);

}