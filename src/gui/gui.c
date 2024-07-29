#include "gui.h"
#include "gc9a01.h"
#include "hardware/dma.h"
#include "hardware/irq.h"
#include "hw_config.h"
#include "lvgl.h"
#include "pico/time.h"
#include "stdio.h"
#include <src/display/lv_display.h>
#include <src/lv_init.h>
#include <src/misc/lv_color.h>

static lv_display_t* display = NULL;
static struct repeating_timer lvgl_timer;
#define BUFSIZE DISP_HOR_RES* DISP_VER_RES / 10
static uint16_t buf1[BUFSIZE]; /*Declare a buffer for 1/10 screen size*/
static uint16_t buf2[BUFSIZE]; /*Declare a buffer for 1/10 screen size*/

static void disp_flush_cb(lv_display_t*, const lv_area_t*, uint8_t*);
static void dma_handler(void);

static void dma_handler(void)
{
  if (dma_channel_get_irq0_status(dma_tx)) {
    dma_channel_acknowledge_irq0(dma_tx);
    lv_display_flush_ready(display); /* Indicate you are ready with the flushing*/
  }
}

static bool repeating_lvgl_timer_callback(struct repeating_timer* t)
{
  lv_tick_inc(5);
  return true;
}

void gui_init()
{
  add_repeating_timer_ms(5, repeating_lvgl_timer_callback, NULL, &lvgl_timer);
  lv_init();
  display = lv_display_create(DISP_HOR_RES, DISP_VER_RES);
  lv_display_set_default(display);
  lv_display_set_flush_cb(display, disp_flush_cb);
  lv_display_set_buffers(display, buf1, buf2, BUFSIZE,
      LV_DISPLAY_RENDER_MODE_PARTIAL); /*Initialize the display buffer.*/
  dma_channel_set_irq0_enabled(dma_tx, true);
  irq_set_exclusive_handler(DMA_IRQ_0, dma_handler);
  irq_set_enabled(DMA_IRQ_0, true);
  /* init DMA */
  gui_widgets_init();
}

void gui_widgets_init()
{
  /*Change the active screen's background color*/
  lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x003a57), LV_PART_MAIN);

  /*Create a white label, set its text and align it to the center*/
  lv_obj_t* label = lv_label_create(lv_screen_active());
  lv_label_set_text(label, "Hello world");
  lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
}

static void disp_flush_cb(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map)
{
  lv_color16_t* cp = (lv_color16_t*)px_map;
  LCD_1IN28_SetWindows(area->x1, area->y1, area->x2, area->y2);
  dma_channel_configure(dma_tx, &c, &spi_get_hw(LCD_SPI_PORT)->dr,
      cp, // read address */
      ((area->x2 + 1 - area->x1) * (area->y2 + 1 - area->y1)) * 2, true);
}
