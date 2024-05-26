#include "qmi8658.h"
#include "hardware/i2c.h"
#include "sys_common.h"
#include <assert.h>

static qmi8658_state_t qmi_state;

static status_t _setup_interface();
static status_t _setup_accel();
static status_t _setup_gyro();
static status_t _setup_lpfs();
static status_t _setup_attitude_engine();

status_t qmi8658_initialize() {
    qmi_state.chip_verified = false;
    qmi_state.rev_id = 0;
    qmi_state.imu_timestamp = 0;
}


static status_t _setup_interface() {
    qmi8658_write_register(QMI_CTRL1_REG, QMIC1_DEFAULT, 1);
    return STATUS_OK;
}

static status_t _setup_accel() {
    qmi8658_write_register(QMI_CTRL2_REG, QMIC2_DEFAULT, 1);
    return STATUS_OK;
}

static status_t _setup_gyro() {
    qmi8658_write_register(QMI_CTRL3_REG, QMIC3_DEFAULT, 1);
    return STATUS_OK;
}

static status_t _setup_lpfs() {
    qmi8658_write_register(QMI_CTRL5_REG, QMIC5_DEFAULT, 1);
    return STATUS_OK;
}

static status_t _setup_attitude_engine() {
    qmi8658_write_register(QMI_CTRL6_REG, QMIC6_DEFAULT, 1);
    return STATUS_OK;
}

void qmi8658_read_xyz_raw(int16_t raw_acc_xyz[3], int16_t raw_gyro_xyz[3], uint64_t *ts)
{
	unsigned char buf_reg[12];
	if (ts)
	{
		uint8_t buf[3];
		uint64_t timestamp;
		qmi8658_read_nregisters(QMI_TS_L_REG, buf, 3); // 0x18	24
		timestamp = (unsigned int)(((unsigned int)buf[2] << 16) | ((unsigned int)buf[1] << 8) | buf[0]);
		if (timestamp > qmi_state.imu_timestamp)
			qmi_state.imu_timestamp = timestamp;
		else
			qmi_state.imu_timestamp = (timestamp + 0x1000000 - qmi_state.imu_timestamp);

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

status_t qmi8658_configure() {
    assert(qmi8658_verify_chip() == STATUS_OK);
    assert(_setup_interface() == STATUS_OK);
    assert(_setup_accel() == STATUS_OK);
    assert(_setup_gyro() == STATUS_OK);
    assert(_setup_lpfs() == STATUS_OK);
    assert(_setup_attitude_engine() == STATUS_OK);
    return STATUS_OK;
}

status_t qmi8658_verify_chip() {
    uint8_t buf[2] = {0};
    qmi8658_read_register(QMI_WHOAMI_REG, (uint16_t*) &buf);
    // gprintf(DEBUG, "whoami: 0x%x, revid: 0x%x", buf[0], buf[1]);
    assert(buf[0] == 0x7b);
    qmi_state.chip_verified = true;
    qmi_state.rev_id = buf[1];
    return STATUS_OK;
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
