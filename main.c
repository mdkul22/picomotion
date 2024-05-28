#include "hw_config.h"
#include "qmi8658.h"
#include "sys_common.h"
#include <stdint.h>
#include <stdio.h>

int main() {
    stdio_init_all();
    init_hardware();
    configure_hardware();
    int16_t raw_accel[3];
    int16_t raw_gyro[3];
    uint64_t time_count;
    gprintf(DEBUG, "\n\033[2J\n");
    while (true) {
        /* move qmi init and configure to configure hardware */
        qmi8658_print_raw();
        sleep_ms(10);
    }
    return 0;
}
