/*
 * plot_view.c
 *
 *  Created on: Sep 22, 2019
 *      Author: vadym
 */

#include "src/ui/views/plot_view.h"
#include "src/ui/views/lvgl_view.h"

#include "lvgl.h"

#include <stdbool.h>

#define AXIS_LABELS_BUFFER_SIZE 30

#define CHART_VERT_RESOLUTION (210 - 50)
#define MAX_PLOTS_PER_CHART 2

struct plot_view_plot_struct {
    lv_chart_series_t* series;
    plot_view_t* view;
    plot_view_axis_range_t* reference_range;
};

typedef struct plot_view_pvt_data_struct {
    lv_obj_t* chart;
    lv_obj_t* top_container;
    plot_view_axis_range_t x_axis;
    char x_axis_ticks[AXIS_LABELS_BUFFER_SIZE];
    plot_view_axis_range_t primary_y_axis;
    char primary_y_axis_ticks[AXIS_LABELS_BUFFER_SIZE];
    plot_view_axis_range_t secondary_y_axis;
    char secondary_y_axis_ticks[AXIS_LABELS_BUFFER_SIZE];
    uint8_t vertical_div_lines;
    uint8_t horiz_div_lines;
    plot_view_plot_t plot_pool[MAX_PLOTS_PER_CHART];
} plot_view_pvt_data_t;

static int32_t plot_view_draw(ui_view_t* view)
{
    plot_view_t* this = view;
    lv_obj_t* screen = lvgl_basic_view_get_screen(view);
    lv_disp_load_scr(screen);
    return 0;
}

static int32_t get_axis_labels(int16_t start, int16_t end, uint8_t divisions_num, char* result)
{
    uint8_t label_str_len = 0;

    //TODO: check if too much text or labels
    if (result) {
        label_str_len = sprintf(result, "%d", start);
        for (uint8_t i = 0; i < divisions_num; i++) {
            //TODO: notice maximal tick label length for adaptive margin
            int16_t current_label = start + (((end - start) * (i + 1)) / divisions_num);
            label_str_len += sprintf(result + label_str_len, "\n%d", current_label);
        }
        return 0;
    } else {
        return -1;
    }

}

//TODO: return status
static void redraw_plot_legend(plot_view_pvt_data_t* view_data)
{
    get_axis_labels(view_data->primary_y_axis.display_min.int_repr,
                    view_data->primary_y_axis.display_max.int_repr,
                    view_data->horiz_div_lines + 1,
                    view_data->primary_y_axis_ticks);

    lv_chart_set_y_tick_texts(view_data->chart, view_data->primary_y_axis_ticks, 2,
                              LV_CHART_AXIS_SKIP_LAST_TICK | LV_CHART_AXIS_INVERSE_LABELS_ORDER);

    get_axis_labels(view_data->secondary_y_axis.display_min.int_repr,
                    view_data->secondary_y_axis.display_max.int_repr,
                    view_data->horiz_div_lines + 1,
                    view_data->secondary_y_axis_ticks);

    lv_chart_set_secondary_y_tick_texts(view_data->chart, view_data->secondary_y_axis_ticks, 2,
                                        LV_CHART_AXIS_SKIP_LAST_TICK | LV_CHART_AXIS_INVERSE_LABELS_ORDER);

    get_axis_labels(view_data->x_axis.display_min.int_repr,
                    view_data->x_axis.display_max.int_repr,
                    view_data->vertical_div_lines + 1,
                    view_data->x_axis_ticks);

    lv_chart_set_x_tick_texts(view_data->chart, view_data->x_axis_ticks, 2,
                                LV_CHART_AXIS_SKIP_LAST_TICK);

    lv_chart_set_div_line_count(view_data->chart, view_data->horiz_div_lines, view_data->vertical_div_lines);
}

//TODO: move magics to constants to be used for scaling
int32_t plot_view_init(plot_view_t* view_to_init)
{
    lvgl_basic_view_create(&view_to_init->view);
    lv_obj_t* screen = lvgl_basic_view_get_screen(view_to_init);

    plot_view_pvt_data_t* view_data = malloc(sizeof(plot_view_pvt_data_t));

    if (view_data) {
        view_data->x_axis.display_min.int_repr = 0;
        view_data->x_axis.display_max.int_repr = 10;

        view_data->primary_y_axis.display_min.int_repr = 0;
        view_data->primary_y_axis.display_max.int_repr = CHART_VERT_RESOLUTION;

        view_data->secondary_y_axis.display_min.int_repr = 0;
        view_data->secondary_y_axis.display_max.int_repr = CHART_VERT_RESOLUTION;

        view_data->vertical_div_lines = 4;
        view_data->horiz_div_lines = 3;

        view_to_init->private_data = view_data;
        //Create informational ribbon on top

        view_data->top_container = lv_cont_create(screen, NULL);
        lv_cont_set_fit(view_data->top_container, LV_FIT_NONE);
        lv_cont_set_layout(view_data->top_container, LV_LAYOUT_PRETTY);
        lv_obj_set_pos(view_data->top_container, 0, 0);
        lv_obj_set_size(view_data->top_container, 480, 35);

        view_data->chart = lv_chart_create(screen, NULL);
        //TODO: check if leaks
        lv_style_t* style = malloc(sizeof(lv_style_t));
        lv_style_copy(style, lv_obj_get_style(view_data->chart));

        style->text.font = &lv_font_roboto_12;
        lv_obj_set_style(view_data->chart, style);
        lv_obj_refresh_style(view_data->chart);
        lv_chart_set_margin(view_data->chart, 30);

        lv_obj_set_pos(view_data->chart, 30, 50);
        lv_obj_set_size(view_data->chart, 420, 210);

        lv_chart_set_y_tick_length(view_data->chart, 5, 8);
        lv_chart_set_secondary_y_tick_length(view_data->chart, 5, 8);
        lv_chart_set_x_tick_length(view_data->chart, 5, 8);
        lv_chart_set_point_count(view_data->chart,
                view_data->x_axis.points_max - view_data->x_axis.points_min);
        lv_chart_set_update_mode(view_data->chart, LV_CHART_UPDATE_MODE_SHIFT);
        lv_chart_set_range(view_data->chart, 0, CHART_VERT_RESOLUTION);

        for (uint32_t i = 0; i < MAX_PLOTS_PER_CHART; i++) {
            view_data->plot_pool[i].reference_range = NULL;
            view_data->plot_pool[i].series = NULL;
        }

        view_to_init->view.draw = &plot_view_draw;
        redraw_plot_legend(view_data);
        return 0;
    } else {
        return -1;
    }

}

int32_t plot_view_set_upper_line_texts(plot_view_t* view, const char** labels, uint8_t labels_count)
{
    plot_view_pvt_data_t* pvt_data = view->private_data;
    //TODO: check if leaks
    lv_obj_clean(pvt_data->top_container);

    for (uint8_t i = 0; i < labels_count; i++) {
        lv_obj_t* label = lv_label_create(pvt_data->top_container, NULL);
        if (!label) {
            //allocation error
            lv_obj_clean(pvt_data->top_container);
            return -1;
        }
        lv_label_set_text(label, labels[i]);
    }
    return 0;
}

int32_t plot_view_set_div_count(plot_view_t* view, uint8_t horizontal, uint8_t vertical)
{
    plot_view_pvt_data_t* pvt_data = view->private_data;
    pvt_data->horiz_div_lines = horizontal;
    pvt_data->vertical_div_lines = vertical;
    redraw_plot_legend(pvt_data);
    return 0;
}


int32_t plot_view_set_axis_range(plot_view_t* view,
                                 enum plot_view_axis which_axis,
                                 plot_view_axis_range_t* range)
{
    plot_view_pvt_data_t* pvt_data = view->private_data;

    switch (which_axis) {
        case AXIS_HORIZONTAL:
            pvt_data->x_axis = *range;
            lv_chart_set_point_count(pvt_data->chart, range->points_max - range->points_min);
            break;
        case AXIS_VERTICAL_PRIMARY:
            pvt_data->primary_y_axis = *range;
            break;
        case AXIS_VERTICAL_SECONDARY:
            pvt_data->secondary_y_axis = *range;
            break;
        default:
            //TODO: invalid argument
            break;
    }

    redraw_plot_legend(pvt_data);
    return 0;
}

plot_view_plot_t* plot_view_create_plot(plot_view_t* view, enum plot_view_axis which_axis, color_t chart_color)
{
    plot_view_pvt_data_t* data = view->private_data;
    plot_view_plot_t* plot = NULL;

    for (uint32_t i = 0; i < MAX_PLOTS_PER_CHART; i++) {
         if (data->plot_pool[i].series == NULL) {
             plot = &(data->plot_pool[i]);
         }
    }

    if (plot) {
        lv_color_t color = LV_COLOR_MAKE((chart_color.red << 3),
                (chart_color.green << 2),
                (chart_color.blue << 3));
        plot->series = lv_chart_add_series(data->chart, color);
        lv_chart_set_series_width(data->chart, 3);
        plot->view = view;
        switch (which_axis) {
        case AXIS_VERTICAL_PRIMARY:
            plot->reference_range = &(data->primary_y_axis);
            break;
        case AXIS_VERTICAL_SECONDARY:
            plot->reference_range = &(data->secondary_y_axis);
            break;
        default:
            //TODO: critical error
            plot->reference_range = NULL;
            break;
        }
    }

    return plot;
}

int32_t plot_view_plot_add_points(plot_view_plot_t* plot, int16_t* points, int16_t points_num)
{
    plot_view_pvt_data_t* view_data = plot->view->private_data;

    float scaling_factor;
    lv_coord_t point_coord;
    scaling_factor = plot->reference_range->points_max - plot->reference_range->points_min;
    scaling_factor = CHART_VERT_RESOLUTION / scaling_factor;

    for (uint8_t i = 0; i < points_num; i++) {
        point_coord = points[i] - plot->reference_range->points_min;
        point_coord = (lv_coord_t)((float)point_coord * scaling_factor);
        lv_chart_set_next(view_data->chart, plot->series, point_coord);

    }

    return 0;
}

void plot_view_plot_destroy(plot_view_plot_t* plot_to_destroy)
{
    (void)plot_to_destroy;
    /*
     * TODO: removing plot is a bit tricky at the moment, because
     * of lvgl limitations (removing series is impossible at the moment).
     * There are two ways to solve the issue:
     * - contribute patch to lvgl which allows removal of the serie
     * - remove chart with all series, create new chart with the same settings
     * and restore all series except one that was deleted
     */
}
