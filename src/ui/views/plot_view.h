/**
 * @file
 * Interface functions for view containing plot view
 * (see UI wireframes, view in the middle of upper row)
 */

#ifndef SRC_UI_VIEWS_PLOT_VIEW_H_
#define SRC_UI_VIEWS_PLOT_VIEW_H_

#include <stdint.h>

#include "ui/views/basic_view.h"
/**
 * @brief shorthand typedef for user convenience
 */
typedef struct plot_view_plot_struct plot_view_plot_t;

/**
 * Structure describing plot view
 */
typedef struct plot_view_struct {
    ui_view_t view;
    void* private_data;
} plot_view_t;

/**
 * Enumeration to specify which axis shall be chosen
 */
enum plot_view_axis {
    AXIS_VERTICAL_PRIMARY,  //!< Left vertical axis
    AXIS_VERTICAL_SECONDARY,//!< Right vertical axis
    AXIS_HORIZONTAL         //!< Horizontal axis (usually time)
};

/**
 * Enumeration to specify how to interpret invariant number type
 */
enum num_invariant_type {
    NUMBER_INVARIANT_INT, //!< Invariant number is integer
    NUMBER_INVARIANT_FLOAT//!< Invariant number is floating point number
};

/**
 * Structure that contains either float or integer number
 */
typedef struct number_invariant_struct {
    enum num_invariant_type type;
    union {
        float float_repr;
        int16_t int_repr;
    };
} number_invariant_t;

/**
 * Structure containing description of two ranges:
 * - one is used for values that displayed on axis legend
 * - second range specifies upper and lower positions of points on the chart
 */
typedef struct plot_view_axis_range_struct {
    number_invariant_t display_min;
    number_invariant_t display_max;
    int16_t points_min;
    int16_t points_max;
} plot_view_axis_range_t;

/**
 * Initialize plot view data structure
 * @param view_to_init pointer to plot view to be initialized
 * @return 0 on successful initialization, error code otherwise
 */
int32_t plot_view_init(plot_view_t* view_to_init);

/**
 * Put labels on line on the top of the screen
 * @param view pointer to plot view
 * @param labels pointer to array of pointers to string containing text to be set
 * @param labels_count number of labels in labels array
 * @return 0 on successful operation, error code otherwise
 */
int32_t plot_view_set_upper_line_texts(plot_view_t* view, const char** labels, uint8_t labels_count);

/**
 * Set horizontal and vertical divisions count
 * @param view pointer to plot view
 * @param horizontal number of horizontal divisions count
 * @param vertical number of vertical divisions count
 * @return 0 on successful operation, error code otherwise
 */
int32_t plot_view_set_div_count(plot_view_t* view, uint8_t horizontal, uint8_t vertical);

/**
 * Set range that will be displayed on axis labels
 * @param view pointer to plot view
 * @param which_axis parameter to specify which axis to configure
 * @param range pointer to range that shall be used for axis
 * @return 0 on successful operation, error code otherwise
 */
int32_t plot_view_set_axis_range(plot_view_t* view,
                                 enum plot_view_axis which_axis,
                                 plot_view_axis_range_t* range);

/**
 * Create plot handle with respect to some axis
 * @param view pointer to parent plot view
 * @param which_axis pointer to vertical axis to be used as a reference
 * @param color plot color
 * @return pointer to plot handle, null on error
 */
plot_view_plot_t* plot_view_create_plot(plot_view_t* view, enum plot_view_axis which_axis, color_t color);

/**
 * Set plot data points that should be displayed
 * @param plot pointer to plot handle
 * @param points array with points to be displayed, value of each point
 * should be between plot_view_axis_range_t::points_min and plot_view_axis_range_t::points_max for vertical axis
 * @param points_num number of points
 * @return 0 on successful operation, error code otherwise
 */
int32_t plot_view_plot_add_points(plot_view_plot_t* plot, int16_t* points, int16_t points_num);

/**
 * Dispose plot handle and free associated resources
 */
void plot_view_plot_destroy(plot_view_plot_t* plot_to_destroy);

/**
 * Dispose plot view and free associated resources
 */
void plot_view_destroy(plot_view_t* view_to_destroy);

#endif /* SRC_UI_VIEWS_PLOT_VIEW_H_ */
