#pragma once
#include "pico/stdlib.h"
#include "sys_common.h"

enum _hw_config_pins {
  QMI_SDA_PIN = 6,
  QMI_SCL_PIN = 7,
};

status_t init_hardware();
status_t deinit_hardware();
status_t configure_hardware();
status_t deconfigure_hardware();
