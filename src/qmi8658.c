#include "qmi8658.h"
#include "hardware/i2c.h"
#include "sys_common.h"

static qmi8658_config_t qmi_config;

status_t qmi8658_initialize() {
    qmi_config.chip_verified = false;
    qmi_config.rev_id = 0;
}


static status_t qmi8658_setup_ctrls() {

}

static status_t qmi8658_setup_accelerometer() {

}

static status_t qmi8658_setup_gyroscope() {

}

static status_t qmi8658_enable_imu() {

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
