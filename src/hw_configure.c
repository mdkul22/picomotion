#include "hardware/gpio.h"
#include "sys_common.h"
#include "hardware/i2c.h"
#include "hw_config.h"

static status_t configure_i2c();
static status_t configure_qmi8658();
static status_t configure_battery();
static status_t configure_flash();
static status_t configure_lcd();

status_t configure_hardware() {
    configure_i2c();
    return STATUS_OK;
}

static status_t configure_i2c() {
    i2c_init(i2c1, 400*1000);
    gpio_set_function(QMI_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(QMI_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(QMI_SDA_PIN);
    gpio_pull_up(QMI_SCL_PIN);
}
