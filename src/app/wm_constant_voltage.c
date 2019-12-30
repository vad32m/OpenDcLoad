#include <stdlib.h>

#include "app/wm_constant_voltage.h"
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
		{.decimals = 0, .param = {.type = NUMBER_INVARIANT_INT, .int_repr = 10}, .descr = &current },
		{.decimals = 0, .param = {.type = NUMBER_INVARIANT_INT, .int_repr = 1}, .descr = &resistance },
};

static work_mode_t cv_mode;

static int32_t wm_periodic_cb(work_mode_t* mode) {
	params[1].param.int_repr = 10 / cv_mode.configurable_param.int_repr;
	return ERR_OK;
}

work_mode_t* wm_constant_voltage_init(void)
{
	work_mode_init(&cv_mode);
	cv_mode.configurable_param.descr = &voltage;
	cv_mode.configurable_param.int_repr = 10;
	cv_mode.configurable_param.int_step = 1;
	cv_mode.configurable_param.max.type = NUMBER_INVARIANT_INT;
	cv_mode.configurable_param.max.int_repr = 10;
	cv_mode.configurable_param.min.type = NUMBER_INVARIANT_INT;
	cv_mode.configurable_param.min.int_repr = 1;
	cv_mode.configurable_param.type = NUMBER_INVARIANT_INT;
	cv_mode.deinit_fxn = NULL;
	cv_mode.init_fxn = NULL;
	cv_mode.start_fxn = NULL;
	cv_mode.stop_fxn = NULL;
	cv_mode.periodic_callback = &wm_periodic_cb;
	cv_mode.output_params = &params[0];
	cv_mode.output_params_count = 2;
	cv_mode.name = "Constant voltage";
	return &cv_mode;
}
