/**
 * @file
 * Header file with interface for base class controller
 */

#ifndef UI_CONTROLLERS_BASIC_CONTROLLER_H_
#define UI_CONTROLLERS_BASIC_CONTROLLER_H_

#include "ui/views/basic_view.h"


enum ui_controller_btn_action_enum {
    UI_BTN_PRESSED,
    UI_BTN_RELEASED,
};

enum ui_controller_encoder_action_enum {
    UI_ENCODER_UP,
    UI_ENCODER_DOWN,
};

typedef enum ui_controller_encoder_action_enum ui_controller_encoder_action;

typedef enum ui_controller_btn_action_enum ui_controller_btn_action;

typedef struct ui_basic_controller_struct ui_basic_controller_t;

typedef int32_t (*btn_handler_fxn_t)(ui_basic_controller_t* ctl,
                                     uint8_t btn_index,
                                     ui_controller_btn_action action);

typedef int32_t (*enc_handler_fxn_t)(ui_basic_controller_t* ctl,
                                     ui_controller_encoder_action action);

typedef int32_t (*ctl_process_fxn_t)(ui_basic_controller_t* ctl);

struct ui_basic_controller_struct {
    btn_handler_fxn_t on_btn_action;
    enc_handler_fxn_t on_enc_action;
    ctl_process_fxn_t process_callback;
    ui_view_t* associated_view;
    void* pvt_data;
};

int32_t ui_basic_controller_init(ui_basic_controller_t* ctl_to_init);

int32_t ui_controller_activate(ui_basic_controller_t* ctl);

int32_t ui_controller_process(ui_basic_controller_t* ctl);

int32_t ui_controller_deactivate(ui_basic_controller_t* ctl);

int32_t ui_controller_notify_button(ui_basic_controller_t* ctl,
                                    uint8_t btn_index,
                                    ui_controller_btn_action action);

int32_t ui_controller_notify_encoder(ui_basic_controller_t* ctl,
                                     ui_controller_encoder_action action);

void ui_basic_controller_destroy(ui_basic_controller_t* ctl_to_del);

#endif /* UI_CONTROLLERS_BASIC_CONTROLLER_H_ */
