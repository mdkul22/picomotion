#pragma once 
#include <stdint.h>
#include <stdbool.h>
#include "sys_common.h"
#include "hardware/i2c.h"

#define QMI8658_I2C_ADDR    0x6b
#define QMI_I2C_PORT        i2c1
#define QMI8658_WHOAMI      0x05

enum __QMI8658Registers {
    QMI_WHOAMI_REG      =   0x00,
    QMI_REVID_REG       =   0x01,
    QMI_CTRL1_REG       =   0x02,
    QMI_CTRL2_REG       =   0x03,
    QMI_CTRL3_REG       =   0x04,
    QMI_CTRL5_REG       =   0x06,
    QMI_CTRL7_REG       =   0x08,
    QMI_CTRL8_REG       =   0x09,
    QMI_CTRL9_REG       =   0x0A,
};

/*
 * @brief: QMI8658 Register values that are to be written into QMI8658 
 */ 
enum __QMI8658RegisterValues {
    QMI_CTRL1_REG_VALUE     =   0b00100000,
    QMI_CTRL2_REG_VALUE     =   0x00,

};

typedef struct _QMI8658Config {
    bool chip_verified;
    uint8_t rev_id;
} qmi8658_config_t;

status_t qmi8658_initialize();
status_t qmi8658_configure();
status_t qmi8658_verify_chip();
status_t qmi8658_write_register(uint8_t reg, uint8_t data, uint8_t len);
status_t qmi8658_read_register(uint8_t reg, uint8_t* data, uint8_t len);
// status_t qmi8658_write_regs();
