#include "ui/controllers/mode_select_controller.h"
#include "app/work_mode_facade.h"

static void on_encoder(ui_basic_controller_t* ctl, ui_controller_encoder_action action)
{
	mode_select_controller_t* mode_select_ctl = ctl;
    mode_select_view_t* view = ctl->associated_view;
    //TODO: temporary hack
    int32_t mode_cnt = mode_select_ctl->pvt_data;
    uint8_t selection = mode_select_view_get_selected_mode(view);
    if (action == UI_ENCODER_DOWN) {
        selection--;
    } else {
        selection++;
    }
    selection = selection % mode_cnt;
    mode_select_view_set_selected_mode(view, selection);
}


int32_t mode_select_controller_init(mode_select_controller_t* ctl_to_init,
                                    mode_select_view_t* view)
{
	int32_t mode_cnt =  wm_facade_get_mode_count();

	if (mode_cnt < 0) {
		//forward error code in case of error
		return mode_cnt;
	}

	//TODO: temporary hack
	ctl_to_init->pvt_data = mode_cnt;

    ctl_to_init->controller.associated_view = view;
    mode_select_view_list_item_t* modes = malloc(sizeof(mode_select_view_list_item_t) * mode_cnt);

    for (int32_t i = 0; i < mode_cnt; i++) {
    	modes[i].id = i;
    	work_mode_t* mode = wm_facade_get_mode(i);
    	if (mode) {
			modes[i].text = mode->name;
    	}
    }

    mode_select_view_set_modes_list(view, modes, mode_cnt);
    mode_select_view_set_selected_mode(view, 0);
    ctl_to_init->controller.on_enc_action = on_encoder;

    free(modes);
}
