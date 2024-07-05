#include "hw_config.h"
#include "qmi8658.h"
#include "sys_common.h"
#include <stdint.h>
#include <stdio.h>
#include "gc9a01.h"

int main() {
    stdio_init_all();
    gprintf(DEBUG, "\n\033[2J\n"); // CLEAR SCREEN CMD
    init_hardware();
    configure_hardware();
    LCD_1IN28_Init(HORIZONTAL);
    LCD_1IN28_Clear(WHITE);
    int16_t raw_accel[3];
    int16_t raw_gyro[3];
    uint64_t time_count;
    while (true) {
        /* move qmi init and configure to configure hardware */
        qmi8658_process_raw();
        sleep_ms(10);
    }
    return 0;
}
