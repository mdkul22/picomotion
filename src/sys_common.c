#include "sys_common.h"
#include <stdarg.h>
#include <stdio.h>

void gprintf(print_type_e type, const char *format, ...) {
    va_list args;
    va_start(args, format);
    switch (type) {
    case DEBUG:
        printf("[Debug]: ");
        break;
    case ERROR:
        printf("[Error]: ");
        break;
    case WARNING:
        printf("[Warning]: ");
        break;
    case INFO:
        printf("[INFO]: ");
        break;
    default:
        return;
    }
    vprintf(format, args);
    va_end(args);
}

/*
 * @brief: handles status of each error / warning condition
 * @input: status_t
 */
void handle_status(status_t status, const char *format, ...) {
    va_list args;
    va_start(args, format);
    switch (status) {
    case STATUS_OK:
        goto ok_status;
        break;
    case STATUS_FAIL_HW_COMM:
        gprintf(ERROR, "HW Comms Error");
        break;
    case STATUS_QMI_CHIP_NOT_VERIFIED:
        gprintf(ERROR, "QMI8658 chip not verified!");
        break;
    case STATUS_QMI_I2C_READ_FAIL:
        gprintf(ERROR, "QMI8658 I2C read failed!");
        break;
    default:
        break;
    }
    printf("[MSG]: ");
    vprintf(format, args);
ok_status:
    va_end(args);
}
