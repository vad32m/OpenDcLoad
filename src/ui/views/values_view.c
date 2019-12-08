/*
 * plot_view.c
 *
 *  Created on: Sep 22, 2019
 *      Author: vadym
 */

#include "src/ui/views/values_view.h"
#include "src/ui/views/values_view_layout.h"
#include "src/ui/views/lvgl_view.h"

#include "lvgl.h"
#include <stdlib.h>


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
        lv_table_set_col_cnt(view_data->table, UI_VALUES_TABLE_COLS);
        lv_table_set_row_cnt(view_data->table, UI_VALUES_TABLE_ROWS);
        lv_obj_set_pos(view_data->table, UI_VALUES_TABLE_X_POS, UI_VALUES_TABLE_Y_POS);
        lv_table_set_col_width(view_data->table, 0, UI_VALUES_TABLE_FIRST_COL_WIDTH);
        lv_table_set_col_width(view_data->table, 1, UI_VALUES_TABLE_SECOND_COL_WIDTH);
    }

    view_to_init->view.draw = &values_view_draw;

    return 0;
}


int32_t values_view_set_table_content(values_view_t* view, values_view_table_entry_t* params, uint8_t params_num)
{
    values_view_pvt_data_t* view_data = view->private_data;
    if (params_num > UI_VALUES_TABLE_ROWS) {
        return -1;
    }

    for (uint8_t i = 0; i < params_num; i++)
    {
        lv_table_set_cell_value(view_data->table, i, 0, params[i].param_name);
        lv_table_set_cell_value(view_data->table, i, 1, params[i].param_value);
    }
    return 0;
}

void values_view_destroy(values_view_t* view_to_destroy)
{
    values_view_pvt_data_t* view_data = view_to_destroy->private_data;
    lv_obj_del(view_data->table);
    free(view_to_destroy->private_data);
    lvgl_basic_view_destroy(view_to_destroy);
}
