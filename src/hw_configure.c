#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hw_config.h"
#include "interrupts.h"
#include "qmi8658.h"
#include "sys_common.h"
#include <assert.h>

static status_t init_i2c();
static status_t init_lcd();
static status_t init_imu();
static status_t init_bms();
static status_t init_flash();

static status_t configure_i2c();
static status_t configure_qmi8658();
static status_t configure_battery();
static status_t configure_flash();
static status_t configure_lcd();

static status_t init_i2c() {
  i2c_init(QMI_I2C_PORT, 400 * 1000);
  gpio_set_function(QMI_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(QMI_SCL_PIN, GPIO_FUNC_I2C);
  return STATUS_OK;
}

static status_t init_imu() {
  assert(STATUS_OK == qmi8658_initialize());
  return STATUS_OK;
}

status_t init_hardware() {
  assert(init_i2c() == STATUS_OK);
  assert(init_imu() == STATUS_OK);
}

static status_t configure_i2c() {
  gpio_pull_up(QMI_SDA_PIN);
  gpio_pull_up(QMI_SCL_PIN);
  return STATUS_OK;
}

status_t configure_hardware() {
  assert(configure_i2c() == STATUS_OK);
  assert(qmi8658_configure() == STATUS_OK);
  setup_interrupts();
  return STATUS_OK;
}
