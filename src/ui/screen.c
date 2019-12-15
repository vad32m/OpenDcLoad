#include "lvgl.h"
#include "ui/screen.h"

#include "ui/controllers/mode_select_controller.h"
#include "ui/controllers/param_setup_controller.h"
#include "ui/controllers/plot_controller.h"
#include "ui/controllers/values_controller.h"

#include "ui/views/mode_select_view.h"
#include "ui/views/param_setup_view.h"
#include "ui/views/plot_view.h"
#include "ui/views/values_view.h"

#include "platform/fp_controls_driver.h"

#include "error_codes.h"

struct ui_screen_struct {
    mode_select_view_t mode_select_view;
    param_setup_view_t param_setup_view;
    plot_view_t plot_view;
    values_view_t values_view;

    mode_select_controller_t mode_select_ctl;
    param_setup_controller_t param_setup_ctl;
    plot_controller_t plot_ctl;
    values_controller_t values_ctl;

    ui_basic_controller_t* controllers[UI_SCREEN_MAX_SCREENS];
    int8_t current_controller;
    int8_t pend_controller_set;

    uint8_t button_states[BTN_MAX_INDEX];
    uint32_t prev_enc_count;
    encoder_dir encoder_dir;
};

static struct ui_screen_struct scr_instance;
static uint8_t is_initialized = 0;

static int32_t screen_init(ui_screen_t* screen)
{
    lv_theme_t* th = lv_theme_night_init(20, NULL);
    lv_theme_set_current(th);

    mode_select_view_init(&screen->mode_select_view);
    param_setup_view_init(&screen->param_setup_view);
    plot_view_init(&screen->plot_view);
    values_view_init(&screen->values_view);

    mode_select_controller_init(&screen->mode_select_ctl, &screen->mode_select_view);
    param_setup_controller_init(&screen->param_setup_ctl, &screen->param_setup_view);
    plot_controller_init(&screen->plot_ctl, &screen->plot_view);
    values_controller_init(&screen->values_ctl, &screen->values_view);

    screen->controllers[UI_SCREEN_MODE_SELECT] = &screen->mode_select_ctl;
    screen->controllers[UI_SCREEN_PARAMETER_SETUP] = &screen->param_setup_ctl;
    screen->controllers[UI_SCREEN_PLOT] = &screen->plot_ctl;
    screen->controllers[UI_SCREEN_VALUES] = &screen->values_ctl;

    buttons_init();
    for (uint8_t i = 0; i < BTN_MAX_INDEX; i++) {
        screen->button_states[i] = button_is_pressed(i);
    }
    encoder_init();
    encoder_get_count_and_dir(&screen->prev_enc_count, &screen->encoder_dir);

    screen->current_controller = UI_SCREEN_PLOT;
    screen->pend_controller_set = UI_SCREEN_PLOT;
    ui_controller_activate(screen->controllers[UI_SCREEN_PLOT]);
}

ui_screen_t* screen_get_instance(void)
{
    if (!is_initialized) {
        screen_init(&scr_instance);
        is_initialized = 1;
    }

    return &scr_instance;
}

int32_t screen_process(ui_screen_t* screen)
{
    uint32_t enc_count;
    ui_basic_controller_t* ctl = screen->controllers[screen->current_controller];

    if (screen->current_controller != screen->pend_controller_set) {
        ui_controller_deactivate(ctl);
        screen->current_controller = screen->pend_controller_set;
        ctl = screen->controllers[screen->current_controller];
        ui_controller_activate(ctl);
    }

    //TODO: shall be put to separate module (front panel handler)
    for (uint8_t i = 0; i < BTN_MAX_INDEX; i++) {
        uint8_t current_state = button_is_pressed(i);
        if (current_state != screen->button_states[i]) {
            screen->button_states[i] = current_state;
            ui_controller_notify_button(ctl, i, current_state ? UI_BTN_PRESSED : UI_BTN_RELEASED);
            //TODO: dummy functionality for demo, shall be removed
            if (!current_state && (i == BTN_ENCODER)) {
                screen->pend_controller_set++;
                screen->pend_controller_set %= UI_SCREEN_MAX_SCREENS;
            }
        }
    }

    encoder_get_count_and_dir(&enc_count, &screen->encoder_dir);

    //TODO: take direction into account
    if (enc_count > screen->prev_enc_count) {
        ui_controller_notify_encoder(ctl, UI_ENCODER_DOWN);
        screen->prev_enc_count += 2;
    } else if (enc_count < screen->prev_enc_count) {
        ui_controller_notify_encoder(ctl, UI_ENCODER_UP);
        screen->prev_enc_count -= 2;
    }

    ui_controller_process(screen->controllers[screen->current_controller]);

    return ERR_OK;
}
