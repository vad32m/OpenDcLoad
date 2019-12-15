#include "ui/controllers/mode_select_controller.h"

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
}
