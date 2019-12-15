/**
 * @file
 * Interface functions for view containing plot view
 * (see UI wireframes, view in the left side of upper row)
 */

#ifndef SRC_UI_VIEWS_MODE_SELECT_VIEW_H_
#define SRC_UI_VIEWS_MODE_SELECT_VIEW_H_

#include <stdint.h>

#include "ui/views/basic_view.h"
/**
 * Structure describing mode select view
 */
typedef struct mode_select_view_struct {
    ui_view_t view;
    void* pvt_data;
} mode_select_view_t;

/**
 * Structure describing item in the list
 */
typedef struct mode_select_view_list_item_struct {
    const char* text;
    uint8_t id;
} mode_select_view_list_item_t;

/**
 * Initialize mode select view data structure
 * @param view_to_init pointer to mode select view to be initialized
 * @return 0 on successful initialization, error code otherwise
 */
int32_t mode_select_view_init(mode_select_view_t* view_to_init);

/**
 * Set list items that user may select
 * @param view_to_init pointer to view
 * @param modes pointer to array of modes that may be set
 * @param modes_num number of modes in the array
 * @return 0 on successful operation, error code otherwise
 */
int32_t mode_select_view_set_modes_list(mode_select_view_t* view,
        mode_select_view_list_item_t* modes,
        uint8_t modes_num);
/**
 * Set selection of current mode (highlight one item in the list)
 * @param view pointer to view
 * @param id ID of the mode to be selected
 * @return 0 on successful operation, error code otherwise
 */
int32_t mode_select_view_set_selected_mode(mode_select_view_t* view, uint8_t id);

/**
 * Get ID of the selected item
 * @param view pointer to view
 * @return ID of the selected item
 */
uint8_t mode_select_view_get_selected_mode(mode_select_view_t* view);

/**
 * Dispose view and free associated resources
 */
void mode_select_view_destroy(mode_select_view_t* view_to_destroy);


#endif /* SRC_UI_VIEWS_MODE_SELECT_VIEW_H_ */
