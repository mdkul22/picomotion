#include "hw_config.h"
#include "qmi8658.h"
#include "sys_common.h"
#include <stdio.h>

int main() {
    stdio_init_all();
    configure_hardware();
    while (true) {
        gprintf(DEBUG, "Hello, world!");
        /* move qmi init and configure to configure hardware */
        qmi8658_initialize();
        qmi8658_configure();
        sleep_ms(1000);
    }
    return 0;
}
