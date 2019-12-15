/**
 * @file
 * Header file with interface for mode select view controller
 */

#ifndef SRC_UI_CONTROLLERS_MODE_SELECT_CONTROLLER_H_
#define SRC_UI_CONTROLLERS_MODE_SELECT_CONTROLLER_H_

#include "ui/controllers/basic_controller.h"
#include "ui/views/mode_select_view.h"

struct mode_select_controller_struct {
    ui_basic_controller controller;
    void* pvt_data;
};

typedef struct mode_select_controller_struct mode_select_controller_t;

int32_t mode_select_controller_init(mode_select_controller_t* ctl_to_init, view_);

void mode_select_controller_destroy(mode_select_controller_t* ctl_to_del);


#endif /* SRC_UI_CONTROLLERS_MODE_SELECT_CONTROLLER_H_ */
