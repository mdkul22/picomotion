#include "gui.h"
#include "gc9a01.h"
#include "hardware/dma.h"
#include "hardware/irq.h"
#include "hw_config.h"
#include "lvgl.h"

static lv_display_t* display = NULL;

static void disp_flush_cb(lv_display_t*, const lv_area_t*, uint8_t*);
static void dma_handler(void);

static void dma_handler(void)
{
  if (dma_channel_get_irq0_status(dma_tx)) {
    dma_channel_acknowledge_irq0(dma_tx);
    lv_display_flush_ready(display); /* Indicate you are ready with the flushing*/
  }
}

void gui_init()
{
  lv_init();
  display = lv_display_create(DISP_HOR_RES, DISP_VER_RES);
  dma_channel_set_irq0_enabled(dma_tx, true);
  irq_set_exclusive_handler(DMA_IRQ_0, dma_handler);
  irq_set_enabled(DMA_IRQ_0, true);
  lv_display_set_flush_cb(display, &disp_flush_cb);
}

static void disp_flush_cb(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map)
{
  LCD_1IN28_SetWindows(area->x1, area->y1, area->x2, area->y2);
  dma_channel_configure(dma_tx, &c, &spi_get_hw(LCD_SPI_PORT)->dr,
      px_map, // read address
      ((area->x2 + 1 - area->x1) * (area->y2 + 1 - area->y1)) * 2, true);
}
