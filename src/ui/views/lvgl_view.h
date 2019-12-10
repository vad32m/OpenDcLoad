/**
 * @file
 *
 * API for basic view LittlevGL implementation
 */

#ifndef SRC_UI_VIEWS_LVGL_VIEW_H_
#define SRC_UI_VIEWS_LVGL_VIEW_H_

#include "ui/view.h"
#include "lvgl.h"

/**
 * Initialize basic view structure
 * @return 0(ERR_OK) on success, zero otherwise
 */
uint32_t lvgl_basic_view_create(ui_view_t* view_to_init);

/**
 * Get LittlevGL root object (screen) for given basic view.
 * Code of other view that customizes this basic view shall attach
 * it's components to the screen of basic view
 */
lv_obj_t* lvgl_basic_view_get_screen(ui_view_t* basic_view);

/**
 * Free all resources associated with the view
 */
void lvgl_basic_view_destroy(ui_view_t* view);

#endif /* SRC_UI_VIEWS_LVGL_VIEW_H_ */
