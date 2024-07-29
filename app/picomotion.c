#include "gc9a01.h"
#include "gui.h"
#include "hw_config.h"
#include "qmi8658.h"
#include "sys_common.h"
#include <stdint.h>
#include <stdio.h>
#include "lvgl.h"

int main()
{
  gprintf(DEBUG, "\n\033[2J\n"); // CLEAR SCREEN CMD
  init_hardware();
  configure_hardware();
  gprintf(DEBUG, "ready\n");
  LCD_1IN28_Init(HORIZONTAL);
  LCD_1IN28_Clear(MAGENTA);
  hw_set_pwm(100);
  gui_init();
  int16_t raw_accel[3];
  int16_t raw_gyro[3];
  uint64_t time_count;
  while (true) {
    /* move qmi init and configure to configure hardware */
    qmi8658_process_raw();
    lv_timer_handler();
    sleep_ms(5);
  }
  return 0;
}
