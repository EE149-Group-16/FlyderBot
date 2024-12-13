/**
 * #Single Library
 * lfc -r ~/mylibraries/custom_lib your_program.lfc
 * #Multiple libraries
 * lfc -r ~/lib -r ~/lib2 your_program.lfc
*/

#include <propeller.h>
#include <pico/stdlib.h>
#include <hardware/pwm.h>

propeller_t* propeller_init(uint8_t pin) {
    propeller_t* propeller = malloc(sizeof(propeller_t));

    gpio_init(pin);
    gpio_set_dir(pin, GPIO_OUT);
    // set the pin to output pwm signals
    gpio_set_function(pin, GPIO_FUNC_PWM);
    // configure PWM slice and get slice num that controls speed
    propeller->pin = pin;
    propeller->slice = pwm_gpio_to_slice_num(pin);
    propeller->channel = pwm_gpio_to_channel(pin);
    pwm_config cfg = pwm_get_default_config();
    // pwm_config_set_clkdiv(&cfg, CLOCK_DIVIDER);
    pwm_config_set_wrap(&cfg, WRAP_VAL);
    pwm_init(propeller->slice, &cfg, true);
    
    return propeller;
}

void propeller_arm_seq(propeller_t* prop_front, propeller_t* prop_rear) {
    // ARMING SEQUENCE
    pwm_set_chan_level(prop_front->slice, prop_front->channel, 1000*WRAP_VAL/20000); // MIN THROTTLE
    pwm_set_chan_level(prop_rear->slice, prop_rear->channel, 1000*WRAP_VAL/20000);
    sleep_ms(10000);
}

void propeller_set_power(propeller_t* prop, uint16_t power_level) {
    // // ACTUAL VALUE
    printf("Seting power to %d\n", power_level);
    pwm_set_chan_level(prop->slice, prop->channel, power_level*WRAP_VAL/20000); 
    // pwm_set_gpio_level(PROPELLER_PIN, 2000);
    // sleep_ms(20);
}

