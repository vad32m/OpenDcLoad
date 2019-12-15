#include "ui/controllers/values_controller.h"

int32_t values_controller_init(values_controller_t* ctl_to_init, values_view_t* view)
{
    ctl_to_init->controller.associated_view = view;
    values_view_table_entry_t data[] = {{.param_name = "I", .param_value = "2 A"},
                                        {.param_name = "U", .param_value = "10 V"},
                                        {.param_name = "R", .param_value = "5 Ohm"}};
    values_view_set_table_content(view, data, 3);
}


