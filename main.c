#include "hw_config.h"
#include "qmi8658.h"
#include "sys_common.h"
#include <stdio.h>
#include <stdint.h>

int main() {
    stdio_init_all();
    configure_hardware();
    qmi8658_configure();
    int16_t raw_accel[3];
    int16_t raw_gyro[3];
    uint32_t time_count;
    while (true) {
        gprintf(DEBUG, "Hello, world!");
        /* move qmi init and configure to configure hardware */
        qmi8658_read_xyz_raw(raw_accel, raw_gyro, &time_count);
        gprintf(DEBUG, "accel_x: %d, accel_y: %d, accel_z: %d\n", raw_accel[0], raw_accel[1], raw_accel[2]);
        gprintf(DEBUG, "gyro_x: %d, gyro_y: %d, gyro_z: %d\n", raw_accel[0], raw_gyro[1], raw_gyro[2]);
        gprintf(DEBUG, "timestamp: %d", time_count);
        sleep_ms(1000);
    }
    return 0;
}
