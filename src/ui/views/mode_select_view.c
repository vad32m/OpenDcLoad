
#include "src/ui/views/mode_select_view.h"
#include "src/ui/views/lvgl_view.h"
#include "lvgl.h"

#define MODE_SELECT_BUF_SIZE 120
#define MAX_NUMBER_OF_LIST_ITEMS 5

typedef struct mode_select_view_pvt_data_struct {
    lv_obj_t* list;
    char lvgl_list_text[MODE_SELECT_BUF_SIZE];
    uint8_t index_map[MAX_NUMBER_OF_LIST_ITEMS];
} mode_select_view_pvt_data_t;

static int32_t mode_select_view_draw(ui_view_t* view)
{
    lv_obj_t* screen = lvgl_basic_view_get_screen(view);
    lv_disp_load_scr(screen);
    return 0;
}

int32_t mode_select_view_init(mode_select_view_t* view_to_init)
{
    lvgl_basic_view_create(&view_to_init->view);
    lv_obj_t* screen = lvgl_basic_view_get_screen(view_to_init);

    mode_select_view_pvt_data_t* view_data = malloc(sizeof(mode_select_view_pvt_data_t));
    if (view_data) {
        view_to_init->pvt_data = view_data;
        view_data->list = lv_roller_create(screen, NULL);
        lv_obj_set_pos(view_data->list, 30, 30);
        lv_obj_set_size(view_data->list, 420, 250);
        lv_roller_set_fix_width(view_data->list, 420);
        lv_roller_set_visible_row_count(view_data->list, 6);
        view_to_init->view.draw = &mode_select_view_draw;
        return 0;
    }

    return -1;

}

int32_t mode_select_view_set_modes_list(mode_select_view_t* view,
            mode_select_view_list_item_t* modes, uint8_t modes_num)
{
    mode_select_view_pvt_data_t* view_data = view->pvt_data;
    if (modes_num > MAX_NUMBER_OF_LIST_ITEMS) {
        return -1;
    }
    view_data->lvgl_list_text[0] = '\0';

    for (uint8_t i = 0; i < modes_num; i++) {
        //TODO: make secure copy
        strcat(view_data->lvgl_list_text, modes[i].text);
        strcat(view_data->lvgl_list_text, "\n");
        view_data->index_map[i] = modes[i].id;
    }

    lv_roller_set_options(view_data->list, view_data->lvgl_list_text, LV_ROLLER_MODE_NORMAL);
    return 0;
}


int32_t mode_select_view_set_selected_mode(mode_select_view_t* view, uint8_t id)
{
    uint8_t i;
    mode_select_view_pvt_data_t* view_data = view->pvt_data;

    //TODO: check max number
    for (i = 0; i < MAX_NUMBER_OF_LIST_ITEMS; i++) {
        if (view_data->index_map[i] == id) {
            lv_roller_set_selected(view_data->list, i, LV_ANIM_OFF);
            return 0;
        }
    }
    return -1;
}

uint8_t mode_select_view_get_selected_mode(mode_select_view_t* view)
{
    mode_select_view_pvt_data_t* view_data = view->pvt_data;
    return view_data->index_map[lv_roller_get_selected(view_data->list)];
}

void mode_select_view_destroy(mode_select_view_t* view_to_destroy)
{
    mode_select_view_pvt_data_t* view_data = view_to_destroy->pvt_data;
    lv_obj_del(view_data->list);
    free(view_to_destroy);
    lvgl_basic_view_destroy((ui_view_t*)view_to_destroy);
}
