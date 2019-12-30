#include <stdlib.h>

#include "app/wm_constant_power.h"
#include "error_codes.h"

static wm_param_descr_t current = {
		.param_name = "Current",
		.param_shorth_name = "I",
		.unit = "A"
};

static wm_param_descr_t voltage = {
		.param_name = "Voltage",
		.param_shorth_name = "U",
		.unit = "V"
};

static wm_param_descr_t resistance = {
		.param_name = "Resistance",
		.param_shorth_name = "R",
		.unit = "Ohm"
};

static wm_param_descr_t power = {
		.param_name = "Power",
		.param_shorth_name = "P",
		.unit = "W"
};

static wm_process_param_t params[] = {
		{.decimals = 0, .param = {.type = NUMBER_INVARIANT_INT, .int_repr = 10}, .descr = &voltage },
		{.decimals = 0, .param = {.type = NUMBER_INVARIANT_INT, .int_repr = 5}, .descr = &current },
};

static work_mode_t cp_mode;

static int32_t wm_periodic_cb(work_mode_t* mode) {
	params[1].param.int_repr = cp_mode.configurable_param.int_repr / 10;
	return ERR_OK;
}

work_mode_t* wm_constant_power_init(void)
{
	work_mode_init(&cp_mode);
	cp_mode.configurable_param.descr = &power;
	cp_mode.configurable_param.int_repr = 50;
	cp_mode.configurable_param.int_step = 1;
	cp_mode.configurable_param.max.type = NUMBER_INVARIANT_INT;
	cp_mode.configurable_param.max.int_repr = 100;
	cp_mode.configurable_param.min.type = NUMBER_INVARIANT_INT;
	cp_mode.configurable_param.min.int_repr = 10;
	cp_mode.configurable_param.type = NUMBER_INVARIANT_INT;
	cp_mode.deinit_fxn = NULL;
	cp_mode.init_fxn = NULL;
	cp_mode.start_fxn = NULL;
	cp_mode.stop_fxn = NULL;
	cp_mode.periodic_callback = &wm_periodic_cb;
	cp_mode.output_params = &params[0];
	cp_mode.output_params_count = 2;
	cp_mode.name = "Constant power";
	return &cp_mode;
}
