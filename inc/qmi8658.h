#pragma once
#include "hardware/gpio.h"
#include "sys_common.h"
#include <stdbool.h>
#include <stdint.h>

#define QMI8658_I2C_ADDR 0x6b
#define QMI_I2C_PORT i2c1

// Register addresses
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

// SPI Interface and Sensor Enable
enum __QMICTRL1RegisterActions {
  QMIC1_SENSOR_DISABLE = 0x01,
  QMIC1_SPI_BE_EN = 0x20, // big endian enable
  QMIC1_AI_EN = 0x40,     // auto increment enable
  QMIC1_SIM3_EN = 0x80,
  QMIC1_DEFAULT = QMIC1_SPI_BE_EN | QMIC1_AI_EN,
};

// Accelerometer output data rate, full scale, self test
enum __QMICTRL2RegisterActions {
  QMIC2_ACCEL_ST_EN = 0x01 << 7, // self test enable
  QMIC2_ACCEL_FS_2G = 0x00 << 4,
  QMIC2_ACCEL_FS_4G = 0x01 << 4,
  QMIC2_ACCEL_FS_8G = 0x02 << 4,
  QMIC2_ACCEL_FS_16G = 0x03 << 4,
  QMIC2_ACCEL_ODR_8KHZ = 0x00,
  QMIC2_ACCEL_ODR_4KHZ = 0x01,
  QMIC2_ACCEL_ODR_2KHZ = 0x02,
  QMIC2_ACCEL_ODR_1KHZ = 0x03,
  QMIC2_ACCEL_ODR_500HZ = 0x04,
  QMIC2_ACCEL_ODR_250HZ = 0x05,
  QMIC2_ACCEL_ODR_125HZ = 0x06,
  QMIC2_ACCEL_ODR_62HZ = 0x07,
  QMIC2_ACCEL_ODR_31HZ = 0x08,
  QMIC2_ACCEL_ODR_128HZ_LP = 0x0C,
  QMIC2_ACCEL_ODR_21HZ_LP = 0x0D,
  QMIC2_ACCEL_ODR_11HZ_LP = 0x0E,
  QMIC2_ACCEL_ODR_3HZ_LP = 0x0F,
  QMIC2_DEFAULT = QMIC2_ACCEL_FS_16G | QMIC2_ACCEL_ODR_500HZ,
};

// Gyroscope output data rate, full scale, self test
enum __QMICTRL3RegisterActions {
  QMIC3_GYRO_ST_EN = 0x01 << 7,
  QMIC3_GYRO_FS_16DPS = 0x00 << 4,
  QMIC3_GYRO_FS_32DPS = 0x01 << 4,
  QMIC3_GYRO_FS_64DPS = 0x02 << 4,
  QMIC3_GYRO_FS_128DPS = 0x03 << 4,
  QMIC3_GYRO_FS_256DPS = 0x04 << 4,
  QMIC3_GYRO_FS_512DPS = 0x05 << 4,
  QMIC3_GYRO_FS_1024DPS = 0x06 << 4,
  QMIC3_GYRO_FS_2048DPS = 0x07 << 4,
  QMIC3_GYRO_ODR_8KHZ = 0x00,
  QMIC3_GYRO_ODR_4KHZ = 0x01,
  QMIC3_GYRO_ODR_2KHZ = 0x02,
  QMIC3_GYRO_ODR_1KHZ = 0x03,
  QMIC3_GYRO_ODR_500HZ = 0x04,
  QMIC3_GYRO_ODR_250HZ = 0x05,
  QMIC3_GYRO_ODR_125HZ = 0x06,
  QMIC3_GYRO_ODR_62HZ = 0x07,
  QMIC3_GYRO_ODR_31HZ = 0x08,
  QMIC3_DEFAULT = QMIC3_GYRO_FS_2048DPS | QMIC3_GYRO_ODR_500HZ,
};

// IMU LPF setup
enum __QMICTRL5RegisterActions {
  QMIC5_GYRO_LPF_MODE_0 = 0x00 << 5, // 2.62% BW
  QMIC5_GYRO_LPF_MODE_1 = 0x01 << 5, // 3.59% BW
  QMIC5_GYRO_LPF_MODE_2 = 0x02 << 5, // 5.32% BW
  QMIC5_GYRO_LPF_MODE_3 = 0x03 << 5, // 14.0% BW
  QMIC5_GYRO_LPF_ENABLE = 0x01 << 4, // Gyro LPF Enable
  QMIC5_ACCL_LPF_MODE_0 = 0x00 << 1, // 2.62% BW
  QMIC5_ACCL_LPF_MODE_1 = 0x01 << 1, // 3.59% BW
  QMIC5_ACCL_LPF_MODE_2 = 0x02 << 1, // 5.32% BW
  QMIC5_ACCL_LPF_MODE_3 = 0x03 << 1, // 14.0% BW
  QMIC5_ACCL_LPF_ENABLE = 0x01 << 0, // Accl LPF Enable
  QMIC5_DEFAULT = QMIC5_GYRO_LPF_MODE_0 | QMIC5_GYRO_LPF_ENABLE |
                  QMIC5_ACCL_LPF_MODE_0 | QMIC5_ACCL_LPF_ENABLE,
};

// Attitude Engine actions
enum __QMICTRL6RegisterActions {
  QMIC6_SMOD_EN = 1 << 7,   // requires sEN = 1
  QMIC6_ATT_ODR_0 = 0x00 << 0, // 1 Hz
  QMIC6_ATT_ODR_1 = 0x01 << 0, // 2 Hz
  QMIC6_ATT_ODR_2 = 0x02 << 0, // 4 Hz
  QMIC6_ATT_ODR_3 = 0x03 << 0, // 8 Hz
  QMIC6_ATT_ODR_4 = 0x04 << 0, // 16 Hz
  QMIC6_ATT_ODR_5 = 0x05 << 0, // 32 Hz
  QMIC6_ATT_ODR_6 = 0x06 << 0, // 64 Hz
  QMIC6_DEFAULT = 0x00, // Disable Attitude Engine
};

// Sensor enable + data read configure
enum __QMIC7RegisterActions {
  QMIC7_SS_EN = 0x01 << 7,          // sync sample enable
  QMIC7_HSC_EN = 0x01 << 6,         // high speed internal clock enable
  QMIC7_GYRO_SNOOZE_EN = 0x01 << 4, // gyro snooze mode enable
  QMIC7_ATT_EN = 0x01 << 3,         // attitude engine enable
  QMIC7_MAG_EN = 0x01 << 2,         // external mag enable
  QMIC7_GYRO_EN = 0x01 << 1,        // gyro enable
  QMIC7_ACCL_EN = 0x01 << 0,             // accel enable
  QMIC7_DEFAULT = QMIC7_ACCL_EN | QMIC7_GYRO_EN,
};

typedef struct _qmi_imu_pkt {
  int16_t raw[3];
  float x;
  float y;
  float z;
} imu_pkt_t;

typedef struct _QMI8658State {
  bool chip_verified;
  uint8_t rev_id;
  uint64_t imu_timestamp;
  imu_pkt_t accl;
  imu_pkt_t gyro;
} qmi8658_state_t;

status_t qmi8658_initialize();
status_t qmi8658_configure();
status_t qmi8658_enable_imu();
status_t qmi8658_disable_imu();
void qmi8658_print_raw();
void qmi8658_interrupt_handler(uint gpio, uint32_t event_mask); // read data
void qmi8658_read_xyz_raw(int16_t raw_acc_xyz[3], int16_t raw_gyro_xyz[3],
                          uint64_t *ts);
status_t qmi8658_verify_chip();
status_t qmi8658_write_register(uint8_t reg, uint8_t data, uint8_t len);
status_t qmi8658_read_register(uint8_t reg_addr, uint16_t *value);
status_t qmi8658_read_nregisters(uint8_t start_addr, uint8_t *pData,
                                 uint8_t len);
// status_t qmi8658_write_regs();
