#include "qmi8658.h"
#include "hardware/i2c.h"
#include "sys_common.h"

static qmi8658_config_t qmi_config;

status_t qmi8658_initialize() {
    qmi_config.chip_verified = false;
    qmi_config.rev_id = 0;
}


static status_t qmi8658_setup_ctrls() {
    qmi8658_write_register(QMI_CTRL1_REG, QMIC1_DEFAULT, 1);
    qmi8658_write_register(QMI_CTRL2_REG, QMIC2_DEFAULT, 1);
    qmi8658_write_register(QMI_CTRL3_REG, QMIC3_DEFAULT, 1);
    qmi8658_write_register(QMI_CTRL5_REG, QMIC5_DEFAULT, 1);
    qmi8658_write_register(QMI_CTRL6_REG, QMIC6_DEFAULT, 1);
    qmi8658_write_register(QMI_CTRL7_REG, QMIC7_DEFAULT, 1);
    return STATUS_OK;
}

static status_t qmi8658_setup_accelerometer() {

}

static status_t qmi8658_setup_gyroscope() {

}

void qmi8658_read_xyz_raw(int16_t raw_acc_xyz[3], int16_t raw_gyro_xyz[3], unsigned int *tim_count)
{
	unsigned char buf_reg[12];

	if (tim_count)
	{
		uint8_t buf[3];
		uint64_t timestamp;
		qmi8658_read_nregisters(QMI_TS_L_REG, buf, 3); // 0x18	24
		timestamp = (unsigned int)(((unsigned int)buf[2] << 16) | ((unsigned int)buf[1] << 8) | buf[0]);
    static uint64_t imu_timestamp = 0;
		if (timestamp > imu_timestamp)
			imu_timestamp = timestamp;
		else
			imu_timestamp = (timestamp + 0x1000000 - imu_timestamp);

		*tim_count = imu_timestamp;
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

status_t qmi8658_configure() {
    status_t status = qmi8658_verify_chip();
    handle_status(status, "rev_id: 0x%x\n", qmi_config.rev_id);
    if (status == STATUS_OK) {
        gprintf(DEBUG, "verified! rev_id: 0x%x", qmi_config.rev_id);
    }
    qmi8658_setup_ctrls();
    qmi8658_enable_imu();
    return status;
}

status_t qmi8658_verify_chip() {
    uint8_t buf[2] = {0};
    status_t status = STATUS_OK;
    status = qmi8658_read_register(QMI_WHOAMI_REG, (uint16_t*) &buf);
    gprintf(DEBUG, "status %d, whoami: 0x%x, revid: 0x%x", status, buf[0], buf[1]);
    if (status != STATUS_OK) {
        goto error;
    }
    if (buf[0] != 0x05) {
        gprintf(ERROR, "whoami: 0x%x ", buf[0]);
        status = STATUS_QMI_CHIP_NOT_VERIFIED;
        goto error;
    }
    qmi_config.chip_verified = true;
    error:
      return status;
}

status_t qmi8658_write_register(uint8_t reg, uint8_t data, uint8_t len) {
    uint8_t buf[2] = {reg, data};
    i2c_write_blocking(QMI_I2C_PORT, QMI8658_I2C_ADDR, buf, 2, false);
}

status_t qmi8658_read_register(uint8_t reg_addr, uint16_t *value) {
    uint8_t tmpi[2];
    i2c_write_blocking(QMI_I2C_PORT, QMI8658_I2C_ADDR, &reg_addr, 1, true); // true to keep master control of bus
    int ret = i2c_read_blocking(QMI_I2C_PORT, QMI8658_I2C_ADDR, tmpi, 2, false);
    *value = (((uint16_t)tmpi[0] << 8) | (uint16_t)tmpi[1]);
    return (ret == 2) ? STATUS_OK : STATUS_FAIL_HW_COMM;
}

status_t qmi8658_read_nregisters(uint8_t start_addr, uint8_t *pData, uint8_t len)
{
    i2c_write_blocking(QMI_I2C_PORT, QMI8658_I2C_ADDR, &start_addr, 1, true);
    int8_t ret = i2c_read_blocking(QMI_I2C_PORT, QMI8658_I2C_ADDR, pData, len, false);
    return ret == len ? STATUS_OK : STATUS_FAIL_HW_COMM;
}
