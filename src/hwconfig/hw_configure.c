#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "hardware/dma.h"
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

uint slice_num;
uint dma_tx;
dma_channel_config c;

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
  assert(configure_lcd() == STATUS_OK);
  setup_interrupts();
  return STATUS_OK;
}

status_t configure_lcd() {
    pwm_set_chan_level(slice_num, PWM_CHAN_B, 100);
    gpio_set_function(LCD_BL_PIN, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(LCD_BL_PIN);
    pwm_set_wrap(slice_num, 100);
    pwm_set_chan_level(slice_num, PWM_CHAN_B, 0);
    pwm_set_clkdiv(slice_num, 50);
    pwm_set_enabled(slice_num, true);
    // SPI Config
    spi_init(LCD_SPI_PORT, 270000 * 1000);
    gpio_set_function(LCD_CLK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(LCD_MOSI_PIN, GPIO_FUNC_SPI);
    // DMA Config
    dma_tx = dma_claim_unused_channel(true);
    c = dma_channel_get_default_config(dma_tx);
    channel_config_set_transfer_data_size(&c, DMA_SIZE_8); 
    channel_config_set_dreq(&c, spi_get_dreq(LCD_SPI_PORT, true));
    return STATUS_OK;
}
