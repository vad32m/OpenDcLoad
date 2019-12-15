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
};

static struct ui_screen_struct scr_instance;
static uint8_t is_initialized;

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

    screen->current_controller = UI_SCREEN_MODE_SELECT;
    screen->pend_controller_set = UI_SCREEN_MODE_SELECT;
    ui_controller_activate(screen->controllers[UI_SCREEN_MODE_SELECT]);
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
    if (screen->current_controller != screen->pend_controller_set) {
        ui_controller_deactivate(screen->controllers[screen->current_controller]);
        screen->current_controller = screen->pend_controller_set;
        ui_controller_activate(screen->controllers[screen->current_controller]);
    }

    ui_controller_process(screen->controllers[screen->current_controller]);

    return ERR_OK;
}
