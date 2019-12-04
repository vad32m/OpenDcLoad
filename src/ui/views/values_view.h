/**
 * @file
 * Interface functions for view containing values table
 * (see UI wireframes, view in the left side of lower row)
 */

#ifndef SRC_UI_VIEWS_VALUES_VIEW_H_
#define SRC_UI_VIEWS_VALUES_VIEW_H_

#include <stdint.h>

#include "src/ui/view.h"

/**
 * Structure describing values table view
 */
typedef struct values_view_struct {
    ui_view_t view;
    void* private_data;
} values_view_t;

/**
 * Structure describing one row in the table
 */
typedef struct values_view_table_entry_struct {
    const char* param_name; //!< parameter name (first table column)
    const char* param_value; //!< parameter value (second table column)
} values_view_table_entry_t;

/**
 * Initialize values table view data structure
 * @param view_to_init pointer to values table view to be initialized
 * @return 0 on successful initialization, error code otherwise
 */
int32_t values_view_init(values_view_t* view_to_init);

/**
 * Fill values table with content
 * @param view pointer to view
 * @param params pointer to array of table entries, each entry represents one row
 * @param params_num number of entries in row
 * @return 0 on successful operation, error code otherwise
 */
int32_t values_view_set_table_content(values_view_t* view, values_view_table_entry_t* params, uint8_t params_num);

/**
 * Dispose values view and free associated resources
 */
void values_view_destroy(values_view_t* view_to_destroy);

#endif /* SRC_UI_VIEWS_VALUES_VIEW_H_ */
