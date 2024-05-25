#pragma once
#include "hardware/i2c.h"
#include "sys_common.h"
#include <stdbool.h>
#include <stdint.h>

#define QMI8658_I2C_ADDR 0x6b
#define QMI_I2C_PORT i2c1

enum __QMI8658Registers {
    QMI_WHOAMI_REG = 0x00,
    QMI_CTRL1_REG = 0x02,
    QMI_CTRL2_REG = 0x03,
    QMI_CTRL3_REG = 0x04,
    QMI_CTRL5_REG = 0x06,
    QMI_CTRL6_REG = 0x07,
    QMI_CTRL7_REG = 0x08,
    QMI_CTRL8_REG = 0x09,
    QMI_CTRL9_REG = 0x0A,
    QMI_STATUSINT_REG = 0x2D,
    QMI_STATUS0_REG = 0x2E,
    QMI_STATUS1_REG = 0x2F,
    QMI_TS_L_REG = 0x30,
    QMI_TS_M_REG = 0x31,
    QMI_TS_H_REG = 0x32,
    QMI_TEMP_L_REG = 0x33,
    QMI_TEMP_H_REG = 0x34,
    QMI_AX_L_REG = 0x35,
    QMI_AX_H_REG = 0x36,
    QMI_AY_L_REG = 0x37,
    QMI_AY_H_REG = 0x38,
    QMI_AZ_L_REG = 0x39,
    QMI_AZ_H_REG = 0x3A,
    QMI_GX_L_REG = 0x3B,
    QMI_GX_H_REG = 0x3C,
    QMI_GY_L_REG = 0x3D,
    QMI_GY_H_REG = 0x3E,
    QMI_GZ_L_REG = 0x3F,
    QMI_GZ_H_REG = 0x40,
};

enum __QMICTRL1RegisterActions {
    QMIC1_SENSOR_DISABLE = 1 << 0,
    QMIC1_SPI_BE_EN = 1 << 5, // big endian enable
    QMIC1_AI_EN = 1 << 6, // auto increment enable 
    QMIC1_SIM3_EN = 1 << 7,
    QMIC1_DEFAULT = 0b01100000,
};

enum __QMICTRL2RegisterActions {
    QMIC2_ACCEL_ST_EN = 1 << 7,
    QMIC2_ACCEL_FS_2G = 0 << 6,
    QMIC2_ACCEL_FS_4G = 1 << 6,
    QMIC2_ACCEL_FS_8G = 0b010 << 6,
    QMIC2_ACCEL_FS_16G = 0b011 << 6,
    QMIC2_ACCEL_ODR_8KHZ = 0 << 3,
    QMIC2_ACCEL_ODR_4KHZ = 1 << 3,
    QMIC2_ACCEL_ODR_2KHZ = 2 << 3,
    QMIC2_ACCEL_ODR_1KHZ = 3 << 3,
    QMIC2_ACCEL_ODR_500HZ = 4 << 3,
    QMIC2_ACCEL_ODR_250HZ = 5 << 3,
    QMIC2_ACCEL_ODR_125HZ = 6 << 3,
    QMIC2_ACCEL_ODR_62HZ = 7 << 3,
    QMIC2_ACCEL_ODR_31HZ = 8 << 3,
    QMIC2_ACCEL_ODR_128HZ_LP = 12 << 3,
    QMIC2_ACCEL_ODR_21HZ_LP = 13 << 3,
    QMIC2_ACCEL_ODR_11HZ_LP = 14 << 3,
    QMIC2_ACCEL_ODR_3HZ_LP = 15 << 3,
    QMIC2_DEFAULT = 0x23,
};

enum __QMICTRL3RegisterActions {
    QMIC3_GYRO_ST_EN = 1 << 7,
    QMIC3_GYRO_FS_16DPS = 0b000 << 6,
    QMIC3_GYRO_FS_32DPS = 0b001 << 6,
    QMIC3_GYRO_FS_64DPS = 0b010 << 6,
    QMIC3_GYRO_FS_128DPS = 0b011 << 6,
    QMIC3_GYRO_FS_256DPS = 0b100 << 6,
    QMIC3_GYRO_FS_512DPS = 0b101 << 6,
    QMIC3_GYRO_FS_1024DPS = 0b110 << 6,
    QMIC3_GYRO_FS_2048DPS = 0b111 << 6,
    QMIC3_GYRO_ODR_8KHZ = 0 << 3,
    QMIC3_GYRO_ODR_4KHZ = 1 << 3,
    QMIC3_GYRO_ODR_2KHZ = 2 << 3,
    QMIC3_GYRO_ODR_1KHZ = 3 << 3,
    QMIC3_GYRO_ODR_500HZ = 4 << 3,
    QMIC3_GYRO_ODR_250HZ = 5 << 3,
    QMIC3_GYRO_ODR_125HZ = 6 << 3,
    QMIC3_GYRO_ODR_62HZ = 7 << 3,
    QMIC3_GYRO_ODR_31HZ = 8 << 3,
    QMIC3_DEFAULT = 0x53,
};

enum __QMICTRL5RegisterActions {
    QMIC5_GYRO_LPF_MODE_0 = 0 << 6,
    QMIC5_GYRO_LPF_MODE_1 = 1 << 6,
    QMIC5_GYRO_LPF_MODE_2 = 2 << 6,
    QMIC5_GYRO_LPF_MODE_3 = 3 << 6,
    QMIC5_GYRO_LPF_ENABLE = 1 << 4,
    QMIC5_ACCL_LPF_MODE_0 = 0 << 2,
    QMIC5_ACCL_LPF_MODE_1 = 1 << 2,
    QMIC5_ACCL_LPF_MODE_2 = 2 << 2,
    QMIC5_ACCL_LPF_MODE_3 = 3 << 2,
    QMIC5_ACCL_LPF_ENABLE = 1,
    QMIC5_DEFAULT = 0x11,
};

enum __QMICTRL6RegisterActions {
    QMIC6_SMOD_EN = 1 << 7, // requires sEN = 1
    QMIC6_ATT_ODR_0 = 0 << 2, // 1 Hz 
    QMIC6_ATT_ODR_1 = 1 << 2, // 1 Hz 
    QMIC6_ATT_ODR_2 = 2 << 2, // 1 Hz 
    QMIC6_ATT_ODR_4 = 3 << 2, // 1 Hz 
    QMIC6_ATT_ODR_5 = 4 << 2, // 1 Hz 
    QMIC6_ATT_ODR_6 = 5 << 2, // 1 Hz 
    QMIC6_DEFAULT = 0x00,
};

enum __QMIC7RegisterActions {
    QMIC7_SS_EN = 1 << 7, // sync sample enable 
    QMIC7_HSC_EN = 1 << 6, // high speed internal clock enable 
    QMIC7_GYRO_SNOOZE_EN = 1 << 4, // gyro snooze mode enable 
    QMIC7_ATT_EN = 1 << 3, // attitude engine enable 
    QMIC7_MAG_EN = 1 << 2, // external mag enable 
    QMIC7_GYRO_EN = 1 << 1, // gyro enable 
    QMIC7_ACCL_EN = 1, 
    QMIC7_DEFAULT = 0x03,
};



typedef struct _QMI8658Config {
    bool chip_verified;
    uint8_t rev_id;
} qmi8658_config_t;

status_t qmi8658_initialize();
status_t qmi8658_configure();
status_t qmi8658_enable_imu();
status_t qmi8658_disable_imu();
void qmi8658_read_xyz_raw(int16_t raw_acc_xyz[3], int16_t raw_gyro_xyz[3],
                          unsigned int *tim_count);
status_t qmi8658_verify_chip();
status_t qmi8658_write_register(uint8_t reg, uint8_t data, uint8_t len);
status_t qmi8658_read_register(uint8_t reg_addr, uint16_t *value);
status_t qmi8658_read_nregisters(uint8_t start_addr, uint8_t *pData, uint8_t len);
// status_t qmi8658_write_regs();
