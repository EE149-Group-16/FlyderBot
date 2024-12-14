#ifndef MPU6050_H
#define MPU6050_H

#include <stdint.h>

typedef struct {
    float ax;
    float ay;
    float az;
    float pitch;
    float roll;
    float ax_offset;
    float ay_offset;
    float az_offset;
} mpu6050_t;

mpu6050_t* mpu6050_init(uint8_t sda_pin, uint8_t scl_pin);
void mpu6050_update(mpu6050_t *mpu);
void mpu6050_calibrate(mpu6050_t *mpu);

#endif
