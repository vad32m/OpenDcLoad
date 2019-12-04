/**
 * @file
 * This file contain parts generic for all views of UI part
 */

#ifndef SRC_UI_VIEW_H_
#define SRC_UI_VIEW_H_

#include <stdint.h>


typedef struct ui_view_struct ui_view_t;

/**
 * All views should have this callback implemented because
 * it will be called by external API
 * @param pointer to view that should be active (self)
 * @return 0 in case of successfull completion, non-null error code otherwise
 */
typedef int32_t (*draw_view_fxn)(ui_view_t* view);

/**
 * Base class for all views in the system
 */
struct ui_view_struct {
    draw_view_fxn draw;
    void* private_data;
};

/**
 * Structure describing color
 */
typedef struct color_struct {
    uint8_t red : 5;
    uint8_t green : 6;
    uint8_t blue : 5;
} color_t;

/**
 * Typedef for callback functions that get called
 * on button press
 * @param btn_id button identifier
 */
typedef void (*view_btn_pressed_fxn)(uint32_t btn_id);

void view_set_button_labels(char const * const labels[]);

#endif /* SRC_UI_VIEW_H_ */
