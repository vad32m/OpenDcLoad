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
    //TODO: virtual function may be reuired
}

int32_t ui_controller_deactivate(ui_basic_controller_t* ctl)
{
    //No need to deactivate views, lvgl manages it automatically
}
