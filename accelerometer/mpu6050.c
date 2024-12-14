#include "mpu6050.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include <math.h>

#define I2C_PORT i2c0
#define MPU6050_ADDR 0x68
#define ACCEL_XOUT_H 0x3B
#define PWR_MGMT_1 0x6B
#define I2C_BAUDRATE 100000

static void mpu6050_write(uint8_t reg, uint8_t data) {
    uint8_t buf[2] = {reg, data};
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, buf, 2, false);
}

static void mpu6050_read(uint8_t reg, uint8_t *buf, uint8_t len) {
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, MPU6050_ADDR, buf, len, false);
}

mpu6050_t mpu6050_init(uint8_t sda_pin, uint8_t scl_pin) {
    i2c_init(I2C_PORT, I2C_BAUDRATE);
    gpio_set_function(sda_pin, GPIO_FUNC_I2C); // SDA
    gpio_set_function(scl_pin, GPIO_FUNC_I2C); // SCL
    gpio_pull_up(sda_pin);
    gpio_pull_up(scl_pin);

    mpu6050_write(PWR_MGMT_1, 0x00);

    mpu6050_t mpu = {
        .ax = 0.0,
        .ay = 0.0,
        .az = 0.0,
        .pitch = 0.0,
        .roll = 0.0,
        .ax_offset = 0.0,
        .ay_offset = 0.0,
        .az_offset = 0.0
    };
    return mpu;
}

void mpu6050_update(mpu6050_t *mpu) {
    uint8_t buf[6];
    int16_t accel_raw[3];

    mpu6050_read(ACCEL_XOUT_H, buf, 6);
    accel_raw[0] = (buf[0] << 8) | buf[1]; // X-axis
    accel_raw[1] = (buf[2] << 8) | buf[3]; // Y-axis
    accel_raw[2] = (buf[4] << 8) | buf[5]; // Z-axis

    mpu->ax = accel_raw[0] / 16384.0 - mpu->ax_offset;
    mpu->ay = accel_raw[1] / 16384.0 - mpu->ay_offset;
    mpu->az = accel_raw[2] / 16384.0 - mpu->az_offset;

    mpu->pitch = atan2(mpu->ay, sqrt(mpu->ax * mpu->ax + mpu->az * mpu->az)) * 180.0 / M_PI;
    mpu->roll = atan2(-mpu->ax, mpu->az) * 180.0 / M_PI;
}

void mpu6050_calibrate(mpu6050_t *mpu) {
    uint8_t buf[6];
    int16_t accel_raw[3];

    mpu6050_read(ACCEL_XOUT_H, buf, 6);
    accel_raw[0] = (buf[0] << 8) | buf[1]; // X-axis
    accel_raw[1] = (buf[2] << 8) | buf[3]; // Y-axis
    accel_raw[2] = (buf[4] << 8) | buf[5]; // Z-axis

    mpu->ax_offset = accel_raw[0] / 16384.0;
    mpu->ay_offset = accel_raw[1] / 16384.0;
    mpu->az_offset = accel_raw[2] / 16384.0;
}
