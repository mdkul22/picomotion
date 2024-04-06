#include "hw_config.h"
#include "qmi8658.h"
#include "sys_common.h"
#include <stdio.h>

int main() {
    stdio_init_all();
    configure_hardware();
    qmi8658_configure();
    qmi8658_enable_imu();
    short raw_acc[3];
    short raw_gyro[3];
    unsigned int time_count;
    while (true) {
        gprintf(DEBUG, "Hello, world!");
        /* move qmi init and configure to configure hardware */
        qmi8658_read_xyz_raw(raw_acc, raw_gyro, &time_count);
        gprintf(DEBUG, "%d, %d, %d", raw_acc[1], raw_gyro[1], time_count);
        sleep_ms(1000);
    }
    return 0;
}
