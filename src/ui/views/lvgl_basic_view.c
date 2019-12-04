/*
 * lvgl_basic_view.c
 *
 *  Created on: Nov 24, 2019
 *      Author: vadym
 */
#include "lvgl.h"
#include "src/ui/views/lvgl_view.h"

typedef struct lvgl_view_private_data_struct {
    lv_obj_t* screen;
    lv_obj_t* button_matrix;
} lvgl_view_private_data_t;

static const char* dummy_btnmap[] = {LV_SYMBOL_HOME, LV_SYMBOL_UP, LV_SYMBOL_DOWN, LV_SYMBOL_OK, ""};

uint32_t lvgl_basic_view_create(ui_view_t* view_to_init)
{
    lvgl_view_private_data_t* pvt_data = malloc(sizeof(lvgl_view_private_data_t));

    if (pvt_data)
    {
        view_to_init->private_data = pvt_data;
        pvt_data->screen = lv_obj_create(NULL, NULL);
        //disable_indent(pvt_data->screen);

        pvt_data->button_matrix = lv_btnm_create(pvt_data->screen, NULL);
        lv_btnm_set_map(pvt_data->button_matrix, dummy_btnmap);
        lv_obj_set_pos(pvt_data->button_matrix, 0, 285);
        lv_obj_set_size(pvt_data->button_matrix, 480, 35);
        //disable_indent(pvt_data->button_matrix);
    }

    return 0;
}

lv_obj_t* lvgl_basic_view_get_screen(ui_view_t* basic_view)
{
    lvgl_view_private_data_t* pvt_data = basic_view->private_data;
    return pvt_data->screen;
}

void lvgl_basic_view_destroy(ui_view_t* view)
{
    lvgl_view_private_data_t* pvt_data = view->private_data;
    lv_obj_clean(pvt_data->screen);
    lv_obj_del(pvt_data->screen);
}

