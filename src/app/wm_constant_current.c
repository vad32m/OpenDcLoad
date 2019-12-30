#include <stdlib.h>

#include "app/wm_constant_current.h"
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
		{.decimals = 0, .param = {.type = NUMBER_INVARIANT_INT, .int_repr = 5}, .descr = &resistance },
};

static work_mode_t cc_mode;

static int32_t wm_periodic_cb(work_mode_t* mode) {
	params[1].param.int_repr = 10 / cc_mode.configurable_param.int_repr;
	return ERR_OK;
}

work_mode_t* wm_constant_current_init(void)
{
	work_mode_init(&cc_mode);
	cc_mode.configurable_param.descr = &current;
	cc_mode.configurable_param.int_repr = 2;
	cc_mode.configurable_param.int_step = 1;
	cc_mode.configurable_param.max.type = NUMBER_INVARIANT_INT;
	cc_mode.configurable_param.max.int_repr = 10;
	cc_mode.configurable_param.min.type = NUMBER_INVARIANT_INT;
	cc_mode.configurable_param.min.int_repr = 1;
	cc_mode.configurable_param.type = NUMBER_INVARIANT_INT;
	cc_mode.deinit_fxn = NULL;
	cc_mode.init_fxn = NULL;
	cc_mode.start_fxn = NULL;
	cc_mode.stop_fxn = NULL;
	cc_mode.periodic_callback = &wm_periodic_cb;
	cc_mode.output_params = &params[0];
	cc_mode.output_params_count = 2;
	cc_mode.name = "Constant current";
	return &cc_mode;
}
