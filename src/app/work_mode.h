/**
 * @file
 * Header file with interface work mode base class
 */
#ifndef APP_WORK_MODE_H_
#define APP_WORK_MODE_H_

#include <stdint.h>
#include "app/work_mode_params.h"
#include "common/number_invariant.h"

typedef struct work_mode_struct work_mode_t;

/**
 * @brief
 * Typedef for work mode initialization and start functions
 */
typedef int32_t (*work_mode_acquire_fxn)(work_mode_t* this);

/**
 * @brief
 * Typedef for work mode deinit and stop functions
 */
typedef void (*work_mode_release_fxn)(work_mode_t* this);

/**
 * @brief
 * Struct describing basic work mode
 */
struct work_mode_struct {
	work_mode_acquire_fxn init_fxn;
	work_mode_acquire_fxn start_fxn;
	work_mode_acquire_fxn periodic_callback;
	work_mode_release_fxn stop_fxn;
	work_mode_release_fxn deinit_fxn;
    wm_config_param_t configurable_param;
    wm_process_param_t* output_params;
    uint8_t output_params_count;
    const char* name;
    void* pvt_data;
};

/**
 * Work mode initialization function
 * @param this pointer to work mode to be initialized
 * @return zero on successful completion, error code otherwise
 */
int32_t work_mode_init(work_mode_t* this);

/**
 * Work mode start function
 * @param this pointer to work mode that shall start functioning
 * @return zero on successful completion, error code otherwise
 */
int32_t work_mode_start(work_mode_t* this);

/**
 * Work mode periodic function. This function shall be called from work mode thread.
 * @param this pointer to active work mode
 * @return zero on successful completion, error code otherwise
 */
int32_t work_mode_process(work_mode_t* this);

/**
 * Work mode stop function
 * @param this pointer to work mode that shall suspend it's functioning
 * @return zero on successful completion, error code otherwise
 */
void work_mode_stop(work_mode_t* this);

/**
 * Work mode deinit function
 * @param this pointer to work mode that shall be deinitialized
 * @return zero on successful completion, error code otherwise
 */
void work_mode_deinit(work_mode_t* this);

/**
 * Get parameters describing current work mode
 * @param this pointer to active work mode
 * @param params pointer to array with parameters to be filled by the function
 * @param param_cnt size of the array, passed as previous argument
 * @return number of work mode parameters, returned by the function or negative error code
 */
int32_t work_mode_get_parameters(work_mode_t* this, wm_process_param_t* params, uint8_t param_cnt);

/**
 * Get configurable parameter
 * @param this pointer to mode, to get configurable parameter for
 * @return pointer to configurable parameter structure
 */
const wm_config_param_t* work_mode_get_config_param(work_mode_t* this);

/**
 * Set configurable parameter
 * @param this pointer to mode, to set configurable parameter to
 * @param value value of the configurable parameter to set
 * @return zero on successful completion, negative error code otherwise
 */
int32_t work_mode_apply_config_param(work_mode_t* this, number_invariant_t value);

#endif /* APP_WORK_MODE_H_ */
