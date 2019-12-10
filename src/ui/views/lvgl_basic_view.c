#include "lvgl.h"
#include "ui/views/lvgl_view.h"
#include "ui/views/basic_view_layout.h"
#include "error_codes.h"
#include "debug/assertions.h"

#include <stdlib.h>

typedef struct lvgl_view_private_data_struct {
    lv_obj_t* screen;
    lv_obj_t* button_matrix;
    char const * btn_labels[UI_BUTTONS_COUNT + 1];
} lvgl_view_private_data_t;

static const char* dummy_btnmap[] = {LV_SYMBOL_HOME, LV_SYMBOL_UP, LV_SYMBOL_DOWN, LV_SYMBOL_OK, ""};

uint32_t lvgl_basic_view_create(ui_view_t* view_to_init)
{
    ASSERT_PTR_VALID(view_to_init);

    lvgl_view_private_data_t* pvt_data = malloc(sizeof(lvgl_view_private_data_t));
    ASSERT_NON_NULL(pvt_data);

    if (pvt_data) {
        view_to_init->private_data = pvt_data;
        pvt_data->screen = lv_obj_create(NULL, NULL);

        pvt_data->button_matrix = lv_btnm_create(pvt_data->screen, NULL);
        lv_btnm_set_map(pvt_data->button_matrix, dummy_btnmap);
        lv_obj_set_pos(pvt_data->button_matrix, UI_BUTTONS_X_POS, UI_BUTTONS_Y_POS);
        lv_obj_set_size(pvt_data->button_matrix, UI_VIEW_WIDTH, UI_BUTTONS_HEIGHT);
        return ERR_NOMEM;
    }

    return ERR_OK;
}

lv_obj_t* lvgl_basic_view_get_screen(ui_view_t* basic_view)
{
    ASSERT_PTR_VALID(basic_view);

    lvgl_view_private_data_t* pvt_data = basic_view->private_data;
    return pvt_data->screen;
}

void lvgl_basic_view_activate(ui_view_t* view)
{
    lv_obj_t* screen = lvgl_basic_view_get_screen(view);
    lv_disp_load_scr(screen);
}

int32_t view_set_button_labels(ui_view_t* basic_view, char const * const labels[])
{
    ASSERT_PTR_VALID(basic_view);
    ASSERT_PTR_VALID(labels);

    lvgl_view_private_data_t* pvt_data = basic_view->private_data;

    for (uint8_t i = 0; i < UI_BUTTONS_COUNT; i++) {
        pvt_data->btn_labels[i] = labels[i];
    }
    pvt_data->btn_labels[UI_BUTTONS_COUNT] = "";

    lv_btnm_set_map(pvt_data->button_matrix, pvt_data->btn_labels);
    return ERR_OK;
}

void view_set_button_pressed(ui_view_t* view, uint8_t btn_index)
{
    ASSERT_PTR_VALID(view);
    ASSERT_TRUE(btn_index < UI_BUTTONS_COUNT);

    lvgl_view_private_data_t* pvt_data = view->private_data;
    lv_btnm_set_pressed(pvt_data->button_matrix, btn_index);
}

void view_release_all_buttons(ui_view_t* view)
{
    ASSERT_PTR_VALID(view);

    lvgl_view_private_data_t* pvt_data = view->private_data;
    lv_btnm_set_pressed(pvt_data->button_matrix, LV_BTNM_BTN_NONE);
}

void lvgl_basic_view_destroy(ui_view_t* view)
{
    ASSERT_PTR_VALID(view);

    lvgl_view_private_data_t* pvt_data = view->private_data;
    lv_obj_clean(pvt_data->screen);
    lv_obj_del(pvt_data->screen);
    free(pvt_data);
}

