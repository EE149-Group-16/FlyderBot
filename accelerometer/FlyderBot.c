#include <stdio.h>
#include "pico/stdlib.h"
#include "mpu6050.h"

int main() {
    stdio_init_all();

    mpu6050_t mpu = mpu6050_init(4, 5);

    printf("Calibrating...\n");
    mpu6050_calibrate(&mpu);
    printf("Calibration complete.\n");

    while (true) {
        mpu6050_update(&mpu);

        printf("ax: %.2f g, ay: %.2f g, az: %.2f g\n", mpu.ax, mpu.ay, mpu.az);
        printf("Pitch: %.2f°, Roll: %.2f°\n", mpu.pitch, mpu.roll);

        sleep_ms(500);
    }
}
