#include "ui/controllers/basic_controller.h"
#include "ui/views/lvgl_view.h"
#include "error_codes.h"

int32_t ui_basic_controller_init(ui_basic_controller_t* ctl_to_init)
{
    ctl_to_init->associated_view = NULL;
    ctl_to_init->on_btn_action = NULL;
    ctl_to_init->on_enc_action = NULL;
    ctl_to_init->process_callback = NULL;
    ctl_to_init->process_callback = NULL;
    return ERR_OK;
}

int32_t ui_controller_activate(ui_basic_controller_t* ctl)
{
    if (ctl->associated_view) {
        lvgl_basic_view_activate(ctl->associated_view);
    }
}

int32_t ui_controller_process(ui_basic_controller_t* ctl)
{
    if (ctl->process_callback) {
        ctl->process_callback(ctl);
    }
}

int32_t ui_controller_deactivate(ui_basic_controller_t* ctl)
{
    //No need to deactivate anything, lvgl manages it automatically
}

int32_t ui_controller_notify_button(ui_basic_controller_t* ctl,
                                    uint8_t btn_index,
                                    ui_controller_btn_action action)
{
    if (btn_index < UI_VIEW_BTN_COUNT) {
        switch (action) {
        case UI_BTN_PRESSED:
            view_set_button_pressed(ctl->associated_view, btn_index);
            break;
        case UI_BTN_RELEASED:
            view_release_all_buttons(ctl->associated_view);
            break;
        }
    }

    if (ctl->on_btn_action) {
        ctl->on_btn_action(ctl, btn_index, action);
    }
}

int32_t ui_controller_notify_encoder(ui_basic_controller_t* ctl,
                                     ui_controller_encoder_action action)
{
    if (ctl->on_enc_action) {
        ctl->on_enc_action(ctl, action);
    }
}
