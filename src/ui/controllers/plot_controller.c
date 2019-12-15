#include "ui/controllers/plot_controller.h"
#include "error_codes.h"

static const char* labels[] = {"I: 2 A", "U: 4 V", "R: 2 Ohm", "P: 8 W", ""};

static const uint8_t  sine_wave[256] = {
  0x80, 0x83, 0x86, 0x89, 0x8C, 0x90, 0x93, 0x96,
  0x99, 0x9C, 0x9F, 0xA2, 0xA5, 0xA8, 0xAB, 0xAE,
  0xB1, 0xB3, 0xB6, 0xB9, 0xBC, 0xBF, 0xC1, 0xC4,
  0xC7, 0xC9, 0xCC, 0xCE, 0xD1, 0xD3, 0xD5, 0xD8,
  0xDA, 0xDC, 0xDE, 0xE0, 0xE2, 0xE4, 0xE6, 0xE8,
  0xEA, 0xEB, 0xED, 0xEF, 0xF0, 0xF1, 0xF3, 0xF4,
  0xF5, 0xF6, 0xF8, 0xF9, 0xFA, 0xFA, 0xFB, 0xFC,
  0xFD, 0xFD, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFE, 0xFD,
  0xFD, 0xFC, 0xFB, 0xFA, 0xFA, 0xF9, 0xF8, 0xF6,
  0xF5, 0xF4, 0xF3, 0xF1, 0xF0, 0xEF, 0xED, 0xEB,
  0xEA, 0xE8, 0xE6, 0xE4, 0xE2, 0xE0, 0xDE, 0xDC,
  0xDA, 0xD8, 0xD5, 0xD3, 0xD1, 0xCE, 0xCC, 0xC9,
  0xC7, 0xC4, 0xC1, 0xBF, 0xBC, 0xB9, 0xB6, 0xB3,
  0xB1, 0xAE, 0xAB, 0xA8, 0xA5, 0xA2, 0x9F, 0x9C,
  0x99, 0x96, 0x93, 0x90, 0x8C, 0x89, 0x86, 0x83,
  0x80, 0x7D, 0x7A, 0x77, 0x74, 0x70, 0x6D, 0x6A,
  0x67, 0x64, 0x61, 0x5E, 0x5B, 0x58, 0x55, 0x52,
  0x4F, 0x4D, 0x4A, 0x47, 0x44, 0x41, 0x3F, 0x3C,
  0x39, 0x37, 0x34, 0x32, 0x2F, 0x2D, 0x2B, 0x28,
  0x26, 0x24, 0x22, 0x20, 0x1E, 0x1C, 0x1A, 0x18,
  0x16, 0x15, 0x13, 0x11, 0x10, 0x0F, 0x0D, 0x0C,
  0x0B, 0x0A, 0x08, 0x07, 0x06, 0x06, 0x05, 0x04,
  0x03, 0x03, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x03,
  0x03, 0x04, 0x05, 0x06, 0x06, 0x07, 0x08, 0x0A,
  0x0B, 0x0C, 0x0D, 0x0F, 0x10, 0x11, 0x13, 0x15,
  0x16, 0x18, 0x1A, 0x1C, 0x1E, 0x20, 0x22, 0x24,
  0x26, 0x28, 0x2B, 0x2D, 0x2F, 0x32, 0x34, 0x37,
  0x39, 0x3C, 0x3F, 0x41, 0x44, 0x47, 0x4A, 0x4D,
  0x4F, 0x52, 0x55, 0x58, 0x5B, 0x5E, 0x61, 0x64,
  0x67, 0x6A, 0x6D, 0x70, 0x74, 0x77, 0x7A, 0x7D
};

static plot_view_plot_t* plot;

static uint8_t curr_pos;
static uint8_t step = 10;

static int32_t process_fxn(ui_basic_controller_t* ctl)
{
    plot_view_t* view = ctl->associated_view;
    curr_pos += step;
    uint16_t point = sine_wave[curr_pos];
    plot_view_plot_add_points(plot, &point, 1);
    return ERR_OK;
}

static void on_encoder(ui_basic_controller_t* ctl, ui_controller_encoder_action action)
{
    if ((step > 2) && (action == UI_ENCODER_DOWN)) {
        step--;
    } else if ((step < 25) && (action == UI_ENCODER_UP)) {
        step++;
    }
}

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
            .points_min = 0, .points_max = 50
    };

    plot_view_set_axis_range(view, AXIS_VERTICAL_PRIMARY, &primary_range);
    plot_view_set_axis_range(view, AXIS_VERTICAL_SECONDARY, &secondary_range);
    plot_view_set_axis_range(view, AXIS_HORIZONTAL, &time_range);

    color_t primary_color = {.red = 0x30, .green = 0x20, .blue = 0x20};

    plot = plot_view_create_plot(view, AXIS_VERTICAL_PRIMARY, primary_color);
    ctl_to_init->controller.on_enc_action = on_encoder;
    ctl_to_init->controller.process_callback = process_fxn;
}


