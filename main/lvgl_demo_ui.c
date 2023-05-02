/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "lvgl.h"

void example_lvgl_demo_ui(lv_disp_t *disp)
{
    lv_obj_t *scr = lv_disp_get_scr_act(disp);

    lv_obj_t *label = lv_label_create(scr);
    lv_label_set_long_mode(label,
                           LV_LABEL_LONG_SCROLL_CIRCULAR); /* Circular scroll
                                                            */
    lv_label_set_text(label, "Hello Espressif, Hello LVGL.");
    lv_obj_set_width(label, 120);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);

    lv_obj_t *label2 = lv_label_create(scr);
    lv_label_set_long_mode(label2,
                           LV_LABEL_LONG_SCROLL_CIRCULAR); /* Circular scroll
                                                            */
    lv_label_set_text(label2, "Hello Ivan, Hello LVGL.");
    lv_obj_set_width(label2, 120);
    lv_obj_align(label2, LV_ALIGN_TOP_MID, 0, 25);

    lv_obj_t *label3 = lv_label_create(scr);
    lv_label_set_long_mode(label3,
                           LV_LABEL_LONG_SCROLL_CIRCULAR); /* Circular scroll
                                                            */
    lv_label_set_text(label3, "Hello LVGL, Hello SH1116.");
    lv_obj_set_width(label3, 120);
    lv_obj_align(label3, LV_ALIGN_TOP_MID, 0, 40);
}
