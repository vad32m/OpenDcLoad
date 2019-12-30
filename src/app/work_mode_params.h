/**
 * @file
 * Header file containing interface for structure describing
 * work mode input and output parameters
 */
#ifndef APP_WORK_MODE_PARAMS_H_
#define APP_WORK_MODE_PARAMS_H_

#include "common/number_invariant.h"

/**
 * @brief
 * Human readable description of parameter
 */
typedef struct {
    const char *param_name;
    const char *param_shorth_name;
    const char *unit;
} wm_param_descr_t;

/**
 * @brief
 * Structure describing input parameter of work mode
 */
typedef struct {
    enum num_invariant_type type;

    union {
        float float_repr;
        int16_t int_repr;
    };

    union {
        float float_step;
        int16_t int_step;
    };

    number_invariant_t min;

    number_invariant_t max;

    const wm_param_descr_t* descr;
} wm_config_param_t;

/**
 * @brief
 * Structure describing output parameter of work mode
 */
typedef struct {
    number_invariant_t param;
    const wm_param_descr_t* descr;
    uint8_t decimals;
} wm_process_param_t;

#endif /* APP_WORK_MODE_PARAMS_H_ */
