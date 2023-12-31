#include <stdio.h>
#include "hw_config.h"
#include "sys_common.h"
#include "qmi8658.h"

int main() {
    stdio_init_all();
    configure_hardware();
    while (true) {
        gprintf(DEBUG, "Hello, world!");
        qmi8658_initialize();
        qmi8658_configure();
        sleep_ms(1000);
    }
    return 0;
}
