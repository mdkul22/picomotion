#pragma once
#include "hardware/dma.h"
#include "hardware/spi.h"
#include "pico/stdlib.h"
#include "sys_common.h"

#define PLL_SYS_KHZ (270 * 1000)
#define LCD_SPI_PORT (spi1)

#define LCD_DC_PIN (8)
#define LCD_CS_PIN (9)
#define LCD_CLK_PIN (10)
#define LCD_MOSI_PIN (11)
#define LCD_RST_PIN (12)
#define LCD_BL_PIN (25)

#define LCD_1IN28_HEIGHT 240
#define LCD_1IN28_WIDTH 240

extern dma_channel_config c;
extern uint dma_tx;

enum _hw_config_pins {
  QMI_SDA_PIN = 6,
  QMI_SCL_PIN = 7,
};

status_t init_hardware();
status_t deinit_hardware();
status_t configure_hardware();
status_t deconfigure_hardware();
