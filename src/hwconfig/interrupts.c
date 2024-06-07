#include "interrupts.h"
#include "hardware/gpio.h"
#include "qmi8658.h"

static void setup_imu_interrupt();

static void setup_imu_interrupt() {
  gpio_set_irq_enabled_with_callback(24, GPIO_IRQ_EDGE_RISE, true, &qmi8658_interrupt_handler);
}

void setup_interrupts() {
  setup_imu_interrupt();
}

void enable_interrupts() {
  gpio_set_irq_enabled(24, GPIO_IRQ_EDGE_RISE, true);
}

void disable_interrupts() {
  gpio_set_irq_enabled(24, GPIO_IRQ_EDGE_RISE, false);
}
