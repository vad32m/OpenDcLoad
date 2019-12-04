/*
 * plot_view.c
 *
 *  Created on: Sep 22, 2019
 *      Author: vadym
 */

#include "src/ui/views/values_view.h"
#include "src/ui/views/lvgl_view.h"

#include "lvgl.h"


typedef struct values_view_pvt_data_struct {
    lv_obj_t* table;
} values_view_pvt_data_t;

static int32_t values_view_draw(ui_view_t* view)
{
    values_view_t* this = view;
    lv_obj_t* screen = lvgl_basic_view_get_screen(view);
    lv_disp_load_scr(screen);
}

int32_t values_view_init(values_view_t* view_to_init)
{
    lvgl_basic_view_create(&view_to_init->view);
    lv_obj_t* screen = lvgl_basic_view_get_screen(view_to_init);

    values_view_pvt_data_t* view_data = malloc(sizeof(values_view_pvt_data_t));

    if (view_data) {
        view_to_init->private_data = view_data;
        view_data->table = lv_table_create(screen, NULL);
        lv_table_set_col_cnt(view_data->table, 2);
        lv_table_set_row_cnt(view_data->table, 6);
        lv_obj_set_pos(view_data->table, 20, 20);
        //lv_obj_set_size(view_data->table, 440, 210);
        lv_table_set_col_width(view_data->table, 0, 100);
        lv_table_set_col_width(view_data->table, 1, 340);
    }

    view_to_init->view.draw = &values_view_draw;

    return 0;
}


int32_t values_view_set_table_content(values_view_t* view, values_view_table_entry_t* params, uint8_t params_num)
{
    values_view_pvt_data_t* view_data = view->private_data;
    //TODO: check if params num is OK
    for (uint8_t i = 0; i < params_num; i++)
    {
        lv_table_set_cell_value(view_data->table, i, 0, params[i].param_name);
        lv_table_set_cell_value(view_data->table, i, 1, params[i].param_value);
    }
}

void values_view_destroy(values_view_t* view_to_destroy)
{
    free(view_to_destroy->private_data);
    lvgl_basic_view_destroy(view_to_destroy);
}
