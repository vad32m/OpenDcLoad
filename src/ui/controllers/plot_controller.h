#ifndef UI_CONTROLLERS_PLOT_CONTROLLER_H_
#define UI_CONTROLLERS_PLOT_CONTROLLER_H_

#include "ui/controllers/basic_controller.h"
#include "ui/views/plot_view.h"

typedef struct plot_controller_struct {
    ui_basic_controller_t controller;
    void* pvt_data;
} plot_controller_t;

int32_t plot_controller_init(plot_controller_t* ctl_to_init, plot_view_t* view);

void plot_controller_destroy(plot_controller_t* ctl_to_destroy);

#endif /* UI_CONTROLLERS_PLOT_CONTROLLER_H_ */
