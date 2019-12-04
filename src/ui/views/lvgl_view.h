/*
 * lvgl_view.h
 *
 *  Created on: Nov 24, 2019
 *      Author: vadym
 */

#ifndef SRC_UI_VIEWS_LVGL_VIEW_H_
#define SRC_UI_VIEWS_LVGL_VIEW_H_

#include "src/ui/view.h"
#include "lvgl.h"

uint32_t lvgl_basic_view_create(ui_view_t* view_to_init);

lv_obj_t* lvgl_basic_view_get_screen(ui_view_t* basic_view);

void lvgl_basic_view_destroy(ui_view_t* view);

#endif /* SRC_UI_VIEWS_LVGL_VIEW_H_ */
