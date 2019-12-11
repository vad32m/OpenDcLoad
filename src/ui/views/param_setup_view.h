/**
 * @file
 * Interface functions for view containing parameter setup view
 * (see UI wireframes, view in the right side of upper row)
 */

#ifndef SRC_UI_VIEWS_PARAM_SETUP_VIEW_H_
#define SRC_UI_VIEWS_PARAM_SETUP_VIEW_H_

#include <stdint.h>

#include "ui/views/basic_view.h"
/**
 * Structure describing parameter setup view
 */
typedef struct param_setup_view_struct {
    ui_view_t view;
    void* private_data;
} param_setup_view_t;

/**
 * Initialize param setup view data structure
 * @param view_to_init pointer to param setup view to be initialized
 * @return 0 on successful initialization, error code otherwise
 */
int32_t param_setup_view_init(param_setup_view_t* view_to_init);


/**
 * Put labels on line on the top of the screen
 * @param view pointer to view
 * @param labels pointer to array of pointers to string containing text to be set
 * @param labels_count number of labels in labels array
 * @return 0 on successful operation, error code otherwise
 */
int32_t param_setup_set_upper_line_texts(param_setup_view_t* view, const char** labels, uint8_t labels_count);


/**
 * Set data that will be displayed on the screen
 * @param view pointer to view
 * @param label text to be used as label
 * @param value text to be used as value
 * @return 0 on successful operation, error code otherwise
 */
int32_t param_setup_view_set_parameter_data(param_setup_view_t* view, const char* label, const char *value);


/**
 * Set how much of the bar shall be filled. Progress bar is used
 * to give better visual understanding of value range
 * @param view pointr to view
 * @param val how much of the progress bar shall be filled (0 - no fill, 255 - fully filled)
 * @return 0 on successful operation, error code otherwise
 */
int32_t param_setup_view_set_bar_position(param_setup_view_t* view, uint8_t val);


/**
 * Dispose param setup view and free associated resources
 */
void param_setup_view_destroy(param_setup_view_t* view_to_destroy);

#endif /* SRC_UI_VIEWS_PARAM_SETUP_VIEW_H_ */
