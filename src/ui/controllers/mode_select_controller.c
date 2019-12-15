#include "ui/controllers/mode_select_controller.h"

static void on_encoder(ui_basic_controller_t* ctl, ui_controller_encoder_action action)
{
    mode_select_view_t* view = ctl->associated_view;
    uint8_t selection = mode_select_view_get_selected_mode(view);
    if (action == UI_ENCODER_DOWN) {
        selection--;
    } else {
        selection++;
    }
    selection = selection % 3;
    mode_select_view_set_selected_mode(view, selection);
}


int32_t mode_select_controller_init(mode_select_controller_t* ctl_to_init,
                                    mode_select_view_t* view)
{
    ctl_to_init->controller.associated_view = view;
    mode_select_view_list_item_t modes[] = {
            { .id = 0, .text = "Constant current" },
            { .id = 1, .text = "Constant voltage" },
            { .id = 2, .text = "Constant resistance" } };

    mode_select_view_set_modes_list(view, &modes, 3);
    mode_select_view_set_selected_mode(view, 1);
    ctl_to_init->controller.on_enc_action = on_encoder;
}
