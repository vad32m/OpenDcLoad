#include "ui/views/param_setup_view.h"
#include "ui/views/param_setup_view_layout.h"
#include "ui/views/lvgl_view.h"

#include "error_codes.h"

#include "lvgl.h"
#include <stdlib.h>

typedef struct param_setup_view_pvt_data {
    lv_obj_t* top_container;
    lv_obj_t* param_table;
    lv_obj_t* slider;
    lv_style_t* table_style;
} param_setup_view_pvt_data_t;


static int32_t param_setup_view_draw(ui_view_t* view)
{
    lv_obj_t* screen = lvgl_basic_view_get_screen(view);
    lv_disp_load_scr(screen);
    return ERR_OK;
}


int32_t param_setup_view_init(param_setup_view_t* view_to_init)
{
    lvgl_basic_view_create(&view_to_init->view);
    lv_obj_t* screen = lvgl_basic_view_get_screen(view_to_init);

    param_setup_view_pvt_data_t* view_data = malloc(sizeof(param_setup_view_pvt_data_t));
    if (view_data)
    {
        view_to_init->private_data = view_data;

        view_data->top_container = lv_cont_create(screen, NULL);
        lv_cont_set_fit(view_data->top_container, LV_FIT_NONE);
        lv_cont_set_layout(view_data->top_container, LV_LAYOUT_PRETTY);
        lv_obj_set_pos(view_data->top_container, UI_TOP_RIBBON_X_POS, UI_TOP_RIBBON_Y_POS);
        lv_obj_set_size(view_data->top_container, UI_VIEW_WIDTH, UI_TOP_RIBBON_HEIGHT);

        view_data->param_table = lv_table_create(screen, NULL);
        lv_obj_set_pos(view_data->param_table, UI_PARAM_TABLE_X_POS, UI_PARAM_TABLE_Y_POS);
        lv_table_set_col_cnt(view_data->param_table, UI_PARAM_TABLE_COL_COUNT);
        lv_table_set_row_cnt(view_data->param_table, UI_PARAM_TABLE_ROW_COUNT);
        lv_table_set_cell_align(view_data->param_table, 0, 0, LV_LABEL_ALIGN_RIGHT);
        lv_table_set_cell_align(view_data->param_table, 0, 1, LV_LABEL_ALIGN_LEFT);
        lv_table_set_col_width(view_data->param_table, 0, UI_PARAM_TABLE_FIRST_COL_WIDTH);
        lv_table_set_col_width(view_data->param_table, 1, UI_PARAM_TABLE_SECOND_COL_WIDTH);

        view_data->table_style = malloc(sizeof(lv_style_t));
        if (view_data->table_style) {
            lv_style_copy(view_data->table_style,
                          lv_table_get_style(view_data->param_table, LV_TABLE_STYLE_CELL1));
            view_data->table_style->body.border.width = 3;
            view_data->table_style->text.font = &lv_font_roboto_28;
            lv_table_set_style(view_data->param_table, LV_TABLE_STYLE_CELL1, view_data->table_style);
            lv_table_set_cell_type(view_data->param_table, 0, 0, 1);
            lv_table_set_cell_type(view_data->param_table, 0, 1, 1);
        }

        view_data->slider = lv_bar_create(screen, NULL);
        lv_obj_set_pos(view_data->slider, UI_PARAM_BAR_X_POS, UI_PARAM_BAR_Y_POS);
        lv_obj_set_size(view_data->slider, UI_PARAM_BAR_WIDTH, UI_PARAM_BAR_HEIGHT);
        lv_bar_set_range(view_data->slider, 0, UINT8_MAX);

        view_to_init->view.draw = &param_setup_view_draw;
        return ERR_OK;
    }

    return ERR_NOMEM;
}


int32_t param_setup_set_upper_line_texts(param_setup_view_t* view, const char** labels, uint8_t labels_count)
{
    param_setup_view_pvt_data_t* pvt_data = view->private_data;
    //TODO: check if leaks
    lv_obj_clean(pvt_data->top_container);

    for (uint8_t i = 0; i < labels_count; i++) {
        lv_obj_t* label = lv_label_create(pvt_data->top_container, NULL);
        if (!label) {
            //allocation error
            lv_obj_clean(pvt_data->top_container);
            return ERR_NOMEM;
        }
        lv_label_set_text(label, labels[i]);
    }
    return ERR_OK;
}


int32_t param_setup_view_set_parameter_data(param_setup_view_t* view, const char* label, const char *value)
{
    param_setup_view_pvt_data_t* view_data = view->private_data;
    lv_table_set_cell_value(view_data->param_table, 0, 0, label);
    lv_table_set_cell_value(view_data->param_table, 0, 1, value);
    return ERR_OK;
}


int32_t param_setup_view_set_bar_position(param_setup_view_t* view, uint8_t val)
{
    param_setup_view_pvt_data_t* view_data = view->private_data;
    lv_bar_set_value(view_data->slider, val, LV_ANIM_OFF);
    return ERR_OK;
}


void param_setup_view_destroy(param_setup_view_t* view_to_destroy)
{
    param_setup_view_pvt_data_t* view_data = view_to_destroy->private_data;
    lv_obj_del(view_data->param_table);
    lv_obj_del(view_data->slider);
    lv_obj_del(view_data->top_container);
    free(view_data->table_style);
    lvgl_basic_view_destroy((ui_view_t*)view_to_destroy);
}
