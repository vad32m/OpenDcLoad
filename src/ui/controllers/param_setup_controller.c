#include "ui/controllers/param_setup_controller.h"

static uint8_t param = 0;

static void on_encoder(ui_basic_controller_t* ctl, ui_controller_encoder_action action)
{
    if (action == UI_ENCODER_DOWN) {
        param--;
    } else {
        param++;
    }

    param_setup_view_t* view = ctl->associated_view;
    static char value_buf[5];
    sprintf(value_buf, "%d", param);
    param_setup_view_set_parameter_data(view, "Param", value_buf);
    param_setup_view_set_bar_position(view, param);
}

int32_t param_setup_controller_init(param_setup_controller_t* ctl_to_init,
                                    param_setup_view_t* view)
{
    ctl_to_init->controller.associated_view = view;
    param_setup_view_set_parameter_data(view, "Param", "0");
    param_setup_view_set_bar_position(view, 0);
    ctl_to_init->controller.on_enc_action = on_encoder;
}


