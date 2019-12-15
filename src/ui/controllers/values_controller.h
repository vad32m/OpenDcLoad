#ifndef UI_CONTROLLERS_VALUES_CONTROLLER_H_
#define UI_CONTROLLERS_VALUES_CONTROLLER_H_

#include "ui/controllers/basic_controller.h"

typedef struct values_controller_struct {
    ui_basic_controller_t controller;
    void* pvt_data;
} values_controller_t;

int32_t values_controller_init(values_controller_t* ctl_to_init);

void values_controller_destroy(values_controller_t* ctl_to_destroy);

#endif /* UI_CONTROLLERS_VALUES_CONTROLLER_H_ */
