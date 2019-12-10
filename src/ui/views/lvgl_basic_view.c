#include "lvgl.h"
#include "ui/views/lvgl_view.h"
#include "ui/views/basic_view_layout.h"
#include "error_codes.h"
#include "debug/assertions.h"

#include <stdlib.h>

typedef struct lvgl_view_private_data_struct {
    lv_obj_t* screen;
    lv_obj_t* button_matrix;
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

void lvgl_basic_view_destroy(ui_view_t* view)
{
    ASSERT_PTR_VALID(view);

    lvgl_view_private_data_t* pvt_data = view->private_data;
    lv_obj_clean(pvt_data->screen);
    lv_obj_del(pvt_data->screen);
}

