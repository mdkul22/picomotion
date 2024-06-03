#include "qmi8658.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "sys_common.h"
#include <assert.h>
#include <stdint.h>

static status_t _setup_interface(qmi_interface_t *interface);
static status_t _setup_accel(qmi_component_config_t *config);
static status_t _setup_gyro(qmi_component_config_t *config);
static status_t _setup_lpf(qmi_component_type_e type, qmi_lpf_config_t *lpf_config);
static status_t _setup_attitude_engine();

static qmi8658_state_t qmi_state = {
  .chip_info = {
    .chip_verified = false,
    .rev_id = 0x00,
  },
  .imu_timestamp = 0,
  .accl = {0},
  .gyro = {0},
  .imu_config ={
    .imu_interface = {
        .is_auto_increment = true,
        .is_sensor_clock_disabled = false,
    },
    .component_switch = {
      .is_att_enabled = false,
      .is_accl_enabled = true,
      .is_gyro_enabled = true,
    },
    .gyro_config = {
      .type = QMI_TYPE_GYRO,
      .lpf_config = {
        .is_lpf_enabled = true,
        .lpf_mode = QMI_LPF_MODE_0,
      },
      .odr_config = QMI_ODR_500HZ,
      .imu_settings = {
          .dps_config = QMI_DPS_1024,
      },
    },
    .accl_config = {
      .type = QMI_TYPE_ACCL,
      .lpf_config = {
        .is_lpf_enabled = true,
        .lpf_mode = QMI_LPF_MODE_0,
      },
      .odr_config = QMI_ODR_500HZ,
      .imu_settings = {
        .g_config = QMI_G_16,
      },
    },
  },
};

/*
 * NOTE: For now, as everything is declared within the global static config
 * it makes no sense to add anything to this initialize function. There
 * could be a possibility to setup some malloc arrays later down the line
 * or loading flashed values for calibration or some new feature later
 * down the line
 * */
status_t qmi8658_initialize() { return STATUS_OK; }

status_t qmi8658_configure() {
  assert(qmi8658_verify_chip(&qmi_state.chip_info) == STATUS_OK);
  assert(_setup_interface(&qmi_state.imu_config.imu_interface) == STATUS_OK);
  assert(_setup_accel(&qmi_state.imu_config.accl_config) == STATUS_OK);
  assert(_setup_gyro(&qmi_state.imu_config.gyro_config) == STATUS_OK);
  assert(_setup_attitude_engine() == STATUS_OK);
  return STATUS_OK;
}

void qmi8658_read_xyz_raw(int16_t raw_acc_xyz[3], int16_t raw_gyro_xyz[3],
                          uint64_t *ts) {
  unsigned char buf_reg[12];
  if (ts) {
    uint8_t buf[3];
    uint64_t timestamp;
    qmi8658_read_nregisters(QMI_TS_L_REG, buf, 3); // 0x18	24
    timestamp = (unsigned int)(((unsigned int)buf[2] << 16) |
                               ((unsigned int)buf[1] << 8) | buf[0]);
    if (timestamp > qmi_state.imu_timestamp)
      qmi_state.imu_timestamp = timestamp;
    else
      qmi_state.imu_timestamp =
          (timestamp + 0x1000000 - qmi_state.imu_timestamp);

    *ts = qmi_state.imu_timestamp;
  }
  qmi8658_read_nregisters(QMI_AX_L_REG, buf_reg, 12); // 0x19, 25

  raw_acc_xyz[0] = (int16_t)((uint16_t)(buf_reg[1] << 8) | (buf_reg[0]));
  raw_acc_xyz[1] = (int16_t)((uint16_t)(buf_reg[3] << 8) | (buf_reg[2]));
  raw_acc_xyz[2] = (int16_t)((uint16_t)(buf_reg[5] << 8) | (buf_reg[4]));

  raw_gyro_xyz[0] = (int16_t)((uint16_t)(buf_reg[7] << 8) | (buf_reg[6]));
  raw_gyro_xyz[1] = (int16_t)((uint16_t)(buf_reg[9] << 8) | (buf_reg[8]));
  raw_gyro_xyz[2] = (int16_t)((uint16_t)(buf_reg[11] << 8) | (buf_reg[10]));
}

status_t qmi8658_enable_imu() {
  uint8_t to_write = QMIC7_GYRO_EN | QMIC7_ACCL_EN;
  qmi8658_write_register(QMI_CTRL7_REG, to_write, 1);
}

status_t qmi8658_disable_imu() {
  uint8_t to_write = 0x00;
  qmi8658_read_nregisters(QMI_CTRL7_REG, &to_write, 1);
  to_write &= 0xFF & ((!QMIC7_ACCL_EN) | (!QMIC7_GYRO_EN));
  qmi8658_write_register(QMI_CTRL7_REG, to_write, 1);
}

status_t qmi8658_verify_chip(qmi_chip_info_t *chip_info) {
  uint8_t whoami = 0, revid = 0;
  qmi8658_read_register(QMI_WHOAMI_REG, &whoami);
  qmi8658_read_register(QMI_REVID_REG, &revid);
  //gprintf(DEBUG, "whoami: 0x%x, revid: 0x%x\n", whoami, revid);
  // TODO: Find a better recovery process when reflash does not work/
  // not properly read 0x7b
  assert(revid == QMI_DEFAULT_REV_ID);
  chip_info->chip_verified = true;
  chip_info->rev_id = revid;
  return STATUS_OK;
}

status_t qmi8658_write_register(uint8_t reg, uint8_t data, uint8_t len) {
  uint8_t buf[2] = {reg, data};
  i2c_write_blocking(QMI_I2C_PORT, QMI8658_I2C_ADDR, buf, 2, false);
}

status_t qmi8658_read_register(uint8_t reg_addr, uint8_t *value) {
  i2c_write_blocking(QMI_I2C_PORT, QMI8658_I2C_ADDR, &reg_addr, 1,
                     true); // true to keep master control of bus
  int ret = i2c_read_blocking(QMI_I2C_PORT, QMI8658_I2C_ADDR, value, 1, false);
  return (ret == 1) ? STATUS_OK : STATUS_FAIL_HW_COMM;
}

status_t qmi8658_read_nregisters(uint8_t start_addr, uint8_t *pData,
                                 uint8_t len) {
  i2c_write_blocking(QMI_I2C_PORT, QMI8658_I2C_ADDR, &start_addr, 1, true);
  int8_t ret =
      i2c_read_blocking(QMI_I2C_PORT, QMI8658_I2C_ADDR, pData, len, false);
  return ret == len ? STATUS_OK : STATUS_FAIL_HW_COMM;
}

void qmi8658_print_raw() {
  gprintf(DEBUG, "a: [%5d, %5d, %5d] g: [%5d, %5d, %5d], ts: %10d\r",
          qmi_state.accl.raw[0], qmi_state.accl.raw[1], qmi_state.accl.raw[2],
          qmi_state.gyro.raw[0], qmi_state.gyro.raw[1], qmi_state.gyro.raw[2],
          qmi_state.imu_timestamp);
}

void qmi8658_interrupt_handler(uint gpio, uint32_t event_mask) {
  qmi8658_read_xyz_raw(qmi_state.accl.raw, qmi_state.gyro.raw,
                       &qmi_state.imu_timestamp);
}

static status_t _setup_interface(qmi_interface_t *interface) {
  if (interface == NULL) {
    return STATUS_NULL_PTR_ERROR;
  }

  uint8_t reg = 0x00;
  if (interface->is_sensor_clock_disabled) {
    reg |= QMIC1_SENSOR_DISABLE;
  }
  if (interface->is_auto_increment) {
    reg |= QMIC1_AI_EN;
  }
  qmi8658_write_register(QMI_CTRL1_REG, reg, 1);
  return STATUS_OK;
}

static status_t _setup_accel(qmi_component_config_t *config) {
  if (config == NULL) {
    return STATUS_NULL_PTR_ERROR;
  }
  if (config->type != QMI_TYPE_ACCL) {
    return STATUS_FAIL;
  }
  uint8_t reg = 0x00;
  switch (config->imu_settings.g_config) {
    case QMI_G_2:
      reg |= QMIC2_ACCEL_FS_2G;
      break;
    case QMI_G_4:
      reg |= QMIC2_ACCEL_FS_4G;
      break;
    case QMI_G_8:
      reg |= QMIC2_ACCEL_FS_8G;
      break;
    case QMI_G_16:
      reg |= QMIC2_ACCEL_FS_16G;
      break;
    default:
      reg |= QMIC2_ACCEL_FS_16G;
      gprintf(WARNING, "accel config fs out of bounds: %d, setting to 16G\n",
              config->imu_settings.g_config);
  }
  switch (config->odr_config) {
    case QMI_ODR_31HZ:
      reg |= QMIC2_ACCEL_ODR_31HZ;
      break;
    case QMI_ODR_62HZ:
      reg |= QMIC2_ACCEL_ODR_62HZ;
      break;
    case QMI_ODR_500HZ:
      reg |= QMIC2_ACCEL_ODR_500HZ;
      break;
    case QMI_ODR_1KHZ:
      reg |= QMIC2_ACCEL_ODR_1KHZ;
      break;
    case QMI_ODR_2KHZ:
      reg |= QMIC2_ACCEL_ODR_2KHZ;
      break;
    case QMI_ODR_4KHZ:
      reg |= QMIC2_ACCEL_ODR_4KHZ;
      break;
    case QMI_ODR_8KHZ:
      reg |= QMIC2_ACCEL_ODR_8KHZ;
      break;
    default:
      reg |= QMIC2_ACCEL_ODR_500HZ;
      gprintf(WARNING, "accel config odr invalid: %1x, setting to 500Hz\n",
              config->odr_config);
  }
  // lpf is set in another register so its separate from the current reg value
  assert(_setup_lpf(config->type, &config->lpf_config) == STATUS_OK);
  qmi8658_write_register(QMI_CTRL2_REG, reg, 1);
  return STATUS_OK;
}

static status_t _setup_gyro(qmi_component_config_t *config) {
  if (config == NULL) {
    return STATUS_NULL_PTR_ERROR;
  }
  if (config->type != QMI_TYPE_GYRO) {
    return STATUS_FAIL;
  }
  uint8_t reg = 0x00;
  switch (config->imu_settings.dps_config) {
    case QMI_DPS_16:
      reg |= QMIC3_GYRO_FS_16DPS;
      break;
    case QMI_DPS_32:
      reg |= QMIC3_GYRO_FS_32DPS;
      break;
    case QMI_DPS_64:
      reg |= QMIC3_GYRO_FS_64DPS;
      break;
    case QMI_DPS_128:
      reg |= QMIC3_GYRO_FS_128DPS;
      break;
    case QMI_DPS_256:
      reg |= QMIC3_GYRO_FS_256DPS; 
      break;
    case QMI_DPS_512:
      reg |= QMIC3_GYRO_FS_512DPS;
      break;
    case QMI_DPS_1024:
      reg |= QMIC3_GYRO_FS_1024DPS;
      break;
    case QMI_DPS_2048:
      reg |= QMIC3_GYRO_FS_2048DPS;
      break;
    default:
      reg |= QMIC2_ACCEL_FS_16G;
      gprintf(WARNING, "accel config fs out of bounds: %d, setting to 16G\n",
              config->imu_settings.g_config);
  }
  switch (config->odr_config) {
    case QMI_ODR_31HZ:
      reg |= QMIC3_GYRO_ODR_31HZ;
      break;
    case QMI_ODR_62HZ:
      reg |= QMIC3_GYRO_ODR_62HZ;
      break;
    case QMI_ODR_500HZ:
      reg |= QMIC3_GYRO_ODR_500HZ;
      break;
    case QMI_ODR_1KHZ:
      reg |= QMIC3_GYRO_ODR_1KHZ;
      break;
    case QMI_ODR_2KHZ:
      reg |= QMIC3_GYRO_ODR_2KHZ;
      break;
    case QMI_ODR_4KHZ:
      reg |= QMIC3_GYRO_ODR_4KHZ;
      break;
    case QMI_ODR_8KHZ:
      reg |= QMIC3_GYRO_ODR_8KHZ;
      break;
    default:
      reg |= QMIC3_GYRO_ODR_500HZ;
      gprintf(WARNING, "accel config odr invalid: %1x, setting to 500Hz\n",
              config->odr_config);
  }
  // lpf is set in another register so its separate from the current reg value
  assert(_setup_lpf(config->type, &config->lpf_config) == STATUS_OK);
  qmi8658_write_register(QMI_CTRL3_REG, reg, 1);
  return STATUS_OK;
}

static status_t _setup_lpf(qmi_component_type_e type,
                           qmi_lpf_config_t *lpf_config) {
  uint8_t data = 0;
  // reading to save state
  qmi8658_read_register(QMI_CTRL5_REG, &data);
  uint8_t reg = data;
  switch (type) {
    case QMI_TYPE_ACCL:
      if (lpf_config->is_lpf_enabled) {
        reg |= QMIC5_ACCL_LPF_ENABLE;
        switch(lpf_config->lpf_mode) {
          case QMI_LPF_MODE_0:
            reg |= QMIC5_ACCL_LPF_MODE_0;
            break;
          case QMI_LPF_MODE_1:
            reg |= QMIC5_ACCL_LPF_MODE_1;
            break;
          case QMI_LPF_MODE_2:
            reg |= QMIC5_ACCL_LPF_MODE_2;
            break;
          case QMI_LPF_MODE_3:
            reg |= QMIC5_ACCL_LPF_MODE_3;
            break;
          default:
            return STATUS_FAIL;
        }
      }
    break;
  case QMI_TYPE_GYRO:
    if (lpf_config->is_lpf_enabled) {
      reg |= QMIC5_GYRO_LPF_ENABLE;
      switch (lpf_config->lpf_mode) {
        case QMI_LPF_MODE_0:
          reg |= QMIC5_GYRO_LPF_MODE_0;
          break;
        case QMI_LPF_MODE_1:
          reg |= QMIC5_GYRO_LPF_MODE_1;
          break;
        case QMI_LPF_MODE_2:
          reg |= QMIC5_GYRO_LPF_MODE_2;
          break;
        case QMI_LPF_MODE_3:
          reg |= QMIC5_GYRO_LPF_MODE_3;
          break;
        default:
          return STATUS_FAIL;
      }
    }
    break;
  default:
    return STATUS_FAIL;
  }
  qmi8658_write_register(QMI_CTRL5_REG, reg, 1);
  return STATUS_OK;
}

static status_t _setup_attitude_engine() {
  qmi8658_write_register(QMI_CTRL6_REG, QMIC6_DEFAULT, 1);
  return STATUS_OK;
}
