#include "qmi8658.h"
#include "hardware/i2c.h"
#include "sys_common.h"

static qmi8658_config_t qmi_config;

status_t qmi8658_initialize() {
    qmi_config.chip_verified = false;
    qmi_config.rev_id = 0;
}


static status_t qmi8658_setup_ctrls() {
    uint8_t to_write = CTRL1_INT1_EN | CTRL1_BE_EN | CTRL1_ADDR_AI_EN;
    qmi8658_write_register(QMI_CTRL1_REG, to_write, 1);
    to_write = CTRL2_aFS_16G | CTRL2_aST_EN | CTRL2_aODR_500;
    qmi8658_write_register(QMI_CTRL2_REG, to_write, 1);
    to_write = (uint8_t)CTRL3_gFS_2048 | CTRL3_gST_EN | CTRL3_gODR_500;
    qmi8658_write_register(QMI_CTRL3_REG, to_write, 1);
    to_write = !CTRL5_aLPF_EN | !CTRL5_gLPF_EN; 
    qmi8658_write_register(QMI_CTRL5_REG, to_write, 1);
    return STATUS_OK;
}

static status_t qmi8658_setup_accelerometer() {

}

static status_t qmi8658_setup_gyroscope() {

}

void qmi8658_read_xyz_raw(short raw_acc_xyz[3], short raw_gyro_xyz[3], unsigned int *tim_count)
{
	unsigned char buf_reg[12];

	if (tim_count)
	{
		unsigned char buf[3];
		unsigned int timestamp;
		qmi8658_read_register(QMI_TS_L_REG, buf, 3); // 0x18	24
		timestamp = (unsigned int)(((unsigned int)buf[2] << 16) | ((unsigned int)buf[1] << 8) | buf[0]);
        static int imu_timestamp = 0;
		if (timestamp > imu_timestamp)
			imu_timestamp = timestamp;
		else
			imu_timestamp = (timestamp + 0x1000000 - imu_timestamp);

		*tim_count = imu_timestamp;
	}
	qmi8658_read_register(QMI_AX_L_REG, buf_reg, 12); // 0x19, 25

	raw_acc_xyz[0] = (short)((unsigned short)(buf_reg[1] << 8) | (buf_reg[0]));
	raw_acc_xyz[1] = (short)((unsigned short)(buf_reg[3] << 8) | (buf_reg[2]));
	raw_acc_xyz[2] = (short)((unsigned short)(buf_reg[5] << 8) | (buf_reg[4]));

	raw_gyro_xyz[0] = (short)((unsigned short)(buf_reg[7] << 8) | (buf_reg[6]));
	raw_gyro_xyz[1] = (short)((unsigned short)(buf_reg[9] << 8) | (buf_reg[8]));
	raw_gyro_xyz[2] = (short)((unsigned short)(buf_reg[11] << 8) | (buf_reg[10]));
}

status_t qmi8658_enable_imu() {
    uint8_t to_write = CTRL7_GYRO_EN | CTRL7_ACCEL_EN;
    qmi8658_write_register(QMI_CTRL7_REG, to_write, 1);

}

status_t qmi8658_disable_imu() {
    uint8_t to_write = !CTRL7_GYRO_EN | !CTRL7_ACCEL_EN;
    qmi8658_write_register(QMI_CTRL7_REG, to_write, 1);

}

status_t qmi8658_configure() {
    status_t status = qmi8658_verify_chip();
    handle_status(status, "rev_id: 0x%x", qmi_config.rev_id);
    if (status == STATUS_OK) {
        gprintf(DEBUG, "verified! rev_id: 0x%x", qmi_config.rev_id);
    }
    return status;
}

status_t qmi8658_verify_chip() {
    uint8_t whoami = 0x00;
    uint8_t rev_id = 0x00;
    status_t status = STATUS_OK;
    status = qmi8658_read_register(QMI_WHOAMI_REG, &whoami, 1);
    status = qmi8658_read_register(QMI_REVID_REG, &rev_id, 1);
    if (status != STATUS_OK) {
        goto error;
    }
    if (whoami != 0x05) {
        gprintf(ERROR, "whoami: 0x%x, rev_id: 0x%x", whoami, rev_id);
        status = STATUS_QMI_CHIP_NOT_VERIFIED;
        goto error;
    }
    qmi_config.chip_verified = true;
    qmi_config.rev_id = rev_id;
error:
    return status;
}

status_t qmi8658_write_register(uint8_t reg, uint8_t data, uint8_t len) {
    uint8_t buf[2] = {reg, data};
    i2c_write_blocking(QMI_I2C_PORT, QMI8658_I2C_ADDR, buf, 2, false);
}

status_t qmi8658_read_register(uint8_t reg, uint8_t *data, uint8_t len) {
    i2c_write_blocking(QMI_I2C_PORT, QMI8658_I2C_ADDR, &reg, 1, true);
    int ret =
        i2c_read_blocking(QMI_I2C_PORT, QMI8658_I2C_ADDR, data, len, false);
    return (ret == len) ? STATUS_OK : STATUS_FAIL_HW_COMM;
}
