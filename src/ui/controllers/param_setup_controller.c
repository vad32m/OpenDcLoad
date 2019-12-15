#include "ui/controllers/param_setup_controller.h"

int32_t param_setup_controller_init(param_setup_controller_t* ctl_to_init,
                                    param_setup_view_t* view)
{
    ctl_to_init->controller.associated_view = view;
    param_setup_view_set_parameter_data(view, "Current", "2 A");
    param_setup_view_set_bar_position(view, 100);
}


