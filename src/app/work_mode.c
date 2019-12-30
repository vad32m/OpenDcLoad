#include <stdlib.h>

#include "app/work_mode.h"
#include "error_codes.h"

int32_t work_mode_init(work_mode_t* this)
{
	this->pvt_data = NULL;
	this->deinit_fxn = NULL;
	this->init_fxn = NULL;
	this->start_fxn = NULL;
	this->stop_fxn = NULL;
	this->periodic_callback = NULL;
	return ERR_OK;
}
