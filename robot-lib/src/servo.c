/**
 * #Single Library
 * lfc -r ~/mylibraries/custom_lib your_program.lfc
 * #Multiple libraries
 * lfc -r ~/lib -r ~/lib2 your_program.lfc
*/

#include <servo.h>
#include <pico/stdlib.h>
#include <hardware/pwm.h>

void servo_init() {
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

void servo_set_angle(int angle) {
    /*
    if (angle > 180) angle = 180; // prevent going past max angle
    if (angle < 0) angle = 0;
    // pulse widths in microseconds
    int pulse_width_deg = (angle * ((MAX_PULSE_WIDTH - MIN_PULSE_WIDTH) / 180)+ MIN_PULSE_WIDTH);
    */
    printf("AGnel inside func, %d", angle);
    int level = (1 * angle); // 20000 is 20ms (period of 50Hz)
    pwm_set_gpio_level(SERVO_PIN, angle);
    sleep_ms(20);

    //pwm_set_enabled(pwm_gpio_to_slice_num(SERVO_PIN), true);
}