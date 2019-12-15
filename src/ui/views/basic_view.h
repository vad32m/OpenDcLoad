/**
 * @file
 * This file contain parts generic for all views of UI part
 */

#ifndef SRC_UI_VIEW_H_
#define SRC_UI_VIEW_H_

#include <stdint.h>

typedef struct ui_view_struct ui_view_t;

#define UI_VIEW_BTN_COUNT 4

/**
 * Base class for all views in the system
 */
struct ui_view_struct {
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
 * Function used to set labels on the buttons
 * @param view view containing the buttons
 * @param labels pointer to array, containing four strings
 * with labels for buttons
 * @return 0 (ERR_OK) on successful completion, error code otherwise
 */
int32_t view_set_button_labels(ui_view_t* view, char const * const labels[]);

void view_set_button_pressed(ui_view_t* view, uint8_t btn_index);

void view_release_all_buttons(ui_view_t* views);

#endif /* SRC_UI_VIEW_H_ */
