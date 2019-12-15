#include "ui/controllers/plot_controller.h"

static const char* labels[] = {"I: 2 A", "U: 4 V", "R: 2 Ohm", "P: 8 W", ""};

int32_t plot_controller_init(plot_controller_t* ctl_to_init, plot_view_t* view)
{
    ctl_to_init->controller.associated_view = view;
    plot_view_set_upper_line_texts(view, labels, 4);
    plot_view_set_div_count(view, 4, 4);

    plot_view_axis_range_t primary_range = {
            .display_min = {.type = NUMBER_INVARIANT_INT, .int_repr = 0},
            .display_max = {.type = NUMBER_INVARIANT_INT, .int_repr = 255},
            .points_min = 0, .points_max = 255
    };

    plot_view_axis_range_t secondary_range = {
            .display_min = {.type = NUMBER_INVARIANT_INT, .int_repr = -10},
            .display_max = {.type = NUMBER_INVARIANT_INT, .int_repr = 10},
            .points_min = -10, .points_max = 10
    };

    plot_view_axis_range_t time_range = {
            .display_min = {.type = NUMBER_INVARIANT_INT, .int_repr = 0},
            .display_max = {.type = NUMBER_INVARIANT_INT, .int_repr = 20},
            .points_min = 0, .points_max = 20
    };

    plot_view_set_axis_range(view, AXIS_VERTICAL_PRIMARY, &primary_range);
    plot_view_set_axis_range(view, AXIS_VERTICAL_SECONDARY, &secondary_range);
    plot_view_set_axis_range(view, AXIS_HORIZONTAL, &time_range);

/*    color_t primary_color = {.red = 0x30, .green = 0x00, .blue = 0x00};
    color_t secondary_color = {.red = 0x00, .green = 0x30, .blue = 0x00};

    plot_view_plot_t* primary = plot_view_create_plot(&plot_view, AXIS_VERTICAL_PRIMARY, primary_color);
    plot_view_plot_t* secondary = plot_view_create_plot(&plot_view, AXIS_VERTICAL_SECONDARY, secondary_color);

    int16_t points[20];

    for (uint8_t i = 0; i < 20; i++) {
        points[i] = sinus(i * 2);
    }
    plot_view_plot_add_points(primary, points, 20);

    for (uint8_t i = 0; i < 20; i++) {
        points[i] = 10 - (int16_t)i;
    }
    plot_view_plot_add_points(secondary, points, 20);*/
}

