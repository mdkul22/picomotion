#pragma once 
#include <stdint.h>
#include <stdbool.h>
#include "sys_common.h"
#include "hardware/i2c.h"

#define QMI8658_I2C_ADDR    0x6A
#define QMI_I2C_PORT        i2c1

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
    QMI_STATUSINT_REG   =   0x2D,
    QMI_STATUS0_REG     =   0x2E,
    QMI_STATUS1_REG     =   0x2F,
    QMI_TS_L_REG        =   0x30,
    QMI_TS_M_REG        =   0x31,
    QMI_TS_H_REG        =   0x32,
    QMI_TEMP_L_REG      =   0x33,
    QMI_TEMP_H_REG      =   0x34,
    QMI_AX_L_REG        =   0x35,
    QMI_AX_H_REG        =   0x36,
    QMI_AY_L_REG        =   0x37,
    QMI_AY_H_REG        =   0x38,
    QMI_AZ_L_REG        =   0x39,
    QMI_AZ_H_REG        =   0x3A,
    QMI_GX_L_REG        =   0x3B,
    QMI_GX_H_REG        =   0x3C,
    QMI_GY_L_REG        =   0x3D,
    QMI_GY_H_REG        =   0x3E,
    QMI_GZ_L_REG        =   0x3F,
    QMI_GZ_H_REG        =   0x40,
};

enum __QMI8658RegisterActions {
    CTRL1_INT2_EN       =   5 << 1,
    CTRL1_INT1_EN       =   6 << 1,
    CTRL2_aST_EN        =   7 << 1, // accel self test enable
    CTRL2_aFS_2G        =   4 << 0b000,
    CTRL2_aFS_4G        =   4 << 0b001,
    CTRL2_aFS_8G        =   4 << 0b010,
    CTRL2_aFS_16G       =   4 << 0b011,
    CTRL2_aODR_1K       =   0b0011,
    CTRL2_aODR_500      =   0b0100,
    CTRL3_gST_EN        =   7 << 1, // gyro self test enable
    CTRL3_gFS_16        =   4 << 0b000,
    CTRL3_gFS_32        =   4 << 0b001,
    CTRL3_gFS_64        =   4 << 0b010,
    CTRL3_gFS_128       =   4 << 0b011,
    CTRL3_gFS_256       =   4 << 0b100,
    CTRL3_gFS_512       =   4 << 0b101,
    CTRL3_gFS_1024      =   4 << 0b110,
    CTRL3_gFS_2048      =   4 << 0b111,
    CTRL3_gODR_1K       =   0b0011,
    CTRL3_gODR_500      =   0b0100,
    CTRL5_gLPF0         =   5 << 0b00,
    CTRL5_gLPF1         =   5 << 0b01,
    CTRL5_gLPF2         =   5 << 0b10,
    CTRL5_gLPF3         =   5 << 0b11,
    CTRL5_gLPF_EN       =   4 << 1, 
    CTRL5_aLPF0         =   1 << 0b00,
    CTRL5_aLPF1         =   1 << 0b01,
    CTRL5_aLPF2         =   1 << 0b10,
    CTRL5_aLPF3         =   1 << 0b11,
    CTRL5_aLPF_EN       =   1, 
    CTRL7_SYNCS_EN      =   7 << 1,
    CTRL7_DRDY_DIS      =   5 << 1,
    CTRL7_GYRO_EN       =   1 << 1,
    CTRL7_ACCEL_EN      =   1,
    CTRL8_C9HS_EN       =   7 << 1, // CTRL9 HANDSHAKE ENABLE
    /* not adding any interrupt actions as of yet */
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
