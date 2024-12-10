/**
 * #Single Library
 * lfc -r ~/mylibraries/custom_lib your_program.lfc
 * #Multiple libraries
 * lfc -r ~/lib -r ~/lib2 your_program.lfc
*/

#include <propeller.h>
#include <pico/stdlib.h>
#include <hardware/pwm.h>

void propeller_init() {
    gpio_init(PROPELLER_PIN);
    gpio_set_dir(PROPELLER_PIN, GPIO_OUT);
    // set the pin to output pwm signals
    gpio_set_function(PROPELLER_PIN, GPIO_FUNC_PWM);
    // configure PWM slice and get slice num that controls speed
    uint32_t slice = pwm_gpio_to_slice_num(PROPELLER_PIN);
    pwm_config cfg = pwm_get_default_config();
    pwm_config_set_clkdiv(&cfg, CLOCK_DIVIDER);
    pwm_config_set_wrap(&cfg, WRAP_VAL);
    pwm_init(slice, &cfg, true);

}

void propeller_set_power(uint16_t power_level) {
    // power_level is btwn 0 and 1.0
    // specs say pwm btwn 0 and max throttle is 1ms to 2ms. set in propeller.h
    uint16_t level = power_level* ((MAX_PW - MIN_PW)/100) + MIN_PW
    pwm_set_gpio_level(PROPELLER_PIN, level);
    sleep_ms(20);
}