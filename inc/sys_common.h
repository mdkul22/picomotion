#pragma once 

typedef enum _status_t {
    STATUS_OK = 0,
    STATUS_FAIL_HW_COMM,
    STATUS_QMI_I2C_READ_FAIL,
    STATUS_QMI_CHIP_NOT_VERIFIED,
    STATUS_NULL_PTR_ERROR,
    STATUS_FAIL,
} status_t;

typedef enum _print_type_e {
    DEBUG = 0,
    WARNING, 
    ERROR,
    INFO,
} print_type_e;

void gprintf(print_type_e type, const char* format, ...); 
void handle_status(status_t status, const char* format, ...);
