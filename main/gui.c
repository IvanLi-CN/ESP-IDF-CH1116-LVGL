/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>

#include "ch1116.c"
#include "driver/i2c.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lvgl.h"

static const char *TAG = "LVGL_DEMO_UI";

#define I2C_HOST 0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your
/// LCD spec //////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define EXAMPLE_LCD_PIXEL_CLOCK_HZ (400 * 1000)

#define EXAMPLE_LCD_H_RES CH1116_WIDTH
#define EXAMPLE_LCD_V_RES CH1116_HEIGHT

#define EXAMPLE_LVGL_TICK_PERIOD_MS 2

extern void example_lvgl_demo_ui(lv_disp_t *disp);

static bool example_notify_lvgl_flush_ready(void *panel_io, void *edata,
                                            void *user_ctx) {
  lv_disp_drv_t *disp_driver = (lv_disp_drv_t *)user_ctx;
  lv_disp_flush_ready(disp_driver);
  return false;
}

static void example_lvgl_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area,
                                  lv_color_t *color_map) {
  int offsetx1 = area->x1;
  int offsetx2 = area->x2;
  int offsety1 = area->y1;
  int offsety2 = area->y2;
  // copy a buffer's content to a specific area of the display
  ch1116_draw_bitmap(offsetx1, offsety1, offsetx2, offsety2, color_map);
  lv_disp_flush_ready(drv);
}

static void example_lvgl_set_px_cb(lv_disp_drv_t *disp_drv, uint8_t *buf,
                                   lv_coord_t buf_w, lv_coord_t x, lv_coord_t y,
                                   lv_color_t color, lv_opa_t opa) {
  uint16_t byte_index = x + ((y >> 3) * buf_w);
  uint8_t bit_index = y & 0x7;

  if ((color.full == 0) && (LV_OPA_TRANSP != opa)) {
    buf[byte_index] |= (1 << bit_index);
  } else {
    buf[byte_index] &= ~(1 << bit_index);
  }
}

static void example_lvgl_rounder(lv_disp_drv_t *disp_drv, lv_area_t *area) {
  area->y1 = area->y1 & (~0x7);
  area->y2 = area->y2 | 0x7;
}

static void example_increase_lvgl_tick(void *arg) {
  /* Tell LVGL how many milliseconds has elapsed */
  lv_tick_inc(EXAMPLE_LVGL_TICK_PERIOD_MS);
}

void gui_main(void) {
  static lv_disp_draw_buf_t
      disp_buf;  // contains internal graphic buffer(s) called draw buffer(s)
  static lv_disp_drv_t disp_drv;  // contains callback functions

  ESP_LOGI(TAG, "Initialize LVGL library");
  lv_init();
  // alloc draw buffers used by LVGL
  // it's recommended to choose the size of the draw buffer(s) to be at least
  // 1/10 screen sized
  lv_color_t *buf1 = malloc(EXAMPLE_LCD_H_RES * 20 * sizeof(lv_color_t));
  assert(buf1);
  lv_color_t *buf2 = malloc(EXAMPLE_LCD_H_RES * 20 * sizeof(lv_color_t));
  assert(buf2);
  // initialize LVGL draw buffers
  lv_disp_draw_buf_init(&disp_buf, buf1, buf2, EXAMPLE_LCD_H_RES * 20);

  ESP_LOGI(TAG, "Register display driver to LVGL");
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = EXAMPLE_LCD_H_RES;
  disp_drv.ver_res = EXAMPLE_LCD_V_RES;
  disp_drv.flush_cb = example_lvgl_flush_cb;
  disp_drv.draw_buf = &disp_buf;
  disp_drv.rounder_cb = example_lvgl_rounder;
  disp_drv.set_px_cb = example_lvgl_set_px_cb;
  lv_disp_t *disp = lv_disp_drv_register(&disp_drv);

  ESP_LOGI(TAG, "Install LVGL tick timer");
  // Tick interface for LVGL (using esp_timer to generate 2ms periodic event)
  const esp_timer_create_args_t lvgl_tick_timer_args = {
      .callback = &example_increase_lvgl_tick, .name = "lvgl_tick"};
  esp_timer_handle_t lvgl_tick_timer = NULL;
  ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer));
  ESP_ERROR_CHECK(esp_timer_start_periodic(lvgl_tick_timer,
                                           EXAMPLE_LVGL_TICK_PERIOD_MS * 1000));

  ESP_LOGI(TAG, "Display LVGL Scroll Text");
  example_lvgl_demo_ui(disp);

  while (1) {
    // raise the task priority of LVGL and/or reduce the handler period can
    // improve the performance
    vTaskDelay(pdMS_TO_TICKS(10));
    // The task running lv_timer_handler should have lower priority than that
    // running `lv_tick_inc`
    lv_timer_handler();
  }
}
