#include "hardware/clocks.h"
#include "hardware/dma.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "hardware/pll.h"
#include "hardware/pwm.h"
#include "hw_config.h"
#include "interrupts.h"
#include "pico/stdlib.h"
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

static void _gpio_set_mode(uint16_t Pin, uint16_t Mode);

static status_t init_i2c()
{
  i2c_init(QMI_I2C_PORT, 400 * 1000);
  gpio_set_function(QMI_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(QMI_SCL_PIN, GPIO_FUNC_I2C);
  return STATUS_OK;
}

static status_t init_imu()
{
  assert(STATUS_OK == qmi8658_initialize());
  return STATUS_OK;
}

status_t init_hardware()
{
  assert(init_i2c() == STATUS_OK);
  assert(init_imu() == STATUS_OK);
  return STATUS_OK;
}

static status_t configure_i2c()
{
  gpio_pull_up(QMI_SDA_PIN);
  gpio_pull_up(QMI_SCL_PIN);
  return STATUS_OK;
}

status_t configure_hardware()
{
  set_sys_clock_khz(PLL_SYS_KHZ, true);
  clock_configure(clk_peri, 0, CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS, PLL_SYS_KHZ * 1000,
      PLL_SYS_KHZ * 1000);
  stdio_init_all();
  assert(configure_lcd() == STATUS_OK);
  assert(configure_i2c() == STATUS_OK);
  assert(qmi8658_configure() == STATUS_OK);
  setup_interrupts();
  return STATUS_OK;
}

static void _gpio_set_mode(uint16_t Pin, uint16_t Mode)
{
  if (Mode == 0 || Mode == GPIO_IN) {
    gpio_set_dir(Pin, GPIO_IN);
  } else {
    gpio_set_dir(Pin, GPIO_OUT);
  }
}

void hw_set_pwm(uint8_t value)
{
  if (value < 0 || value > 100) {
        return ;
  } else {
    pwm_set_chan_level(slice_num, PWM_CHAN_B, value);
  }
}

status_t configure_lcd()
{
  gpio_init(LCD_RST_PIN);
  gpio_set_dir(LCD_RST_PIN, GPIO_OUT);
  gpio_init(LCD_DC_PIN);
  gpio_set_dir(LCD_DC_PIN, GPIO_OUT);
  gpio_init(LCD_CS_PIN);
  gpio_set_dir(LCD_CS_PIN, GPIO_OUT);
  gpio_init(LCD_BL_PIN);
  gpio_set_dir(LCD_BL_PIN, GPIO_OUT);
  gpio_put(LCD_CS_PIN, 1);
  gpio_put(LCD_DC_PIN, 0);
  gpio_put(LCD_BL_PIN, 1);
  /* adc_init(); */
  /* adc_gpio_init(BAT_ADC_PIN); */
  /* adc_select_input(BAR_CHANNEL); */
  // PWM onfig
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
