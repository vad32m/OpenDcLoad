#ifndef UI_CONTROLLERS_PARAM_SETUP_CONTROLLER_H_
#define UI_CONTROLLERS_PARAM_SETUP_CONTROLLER_H_

#include "ui/controllers/basic_controller.h"

typedef struct param_setup_controller_struct {
    ui_basic_controller_t controller;
    void* pvt_data;
} param_setup_controller_t;

int32_t param_setup_controller_init(param_setup_controller_t* ctl_to_init);

void param_setup_controller_destroy(param_setup_controller_t* ctl_to_destroy);

#endif /* UI_CONTROLLERS_PLOT_CONTROLLER_H_ */
