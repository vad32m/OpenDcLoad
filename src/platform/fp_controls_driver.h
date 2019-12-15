/**
 * @file
 * Header file with driver for buttons and encoder that gives abstraction over
 * specific hardware implementation details and chip internals
 */

#ifndef PLATFORM_FP_CONTROLS_DRIVER_H_
#define PLATFORM_FP_CONTROLS_DRIVER_H_

#include <stdbool.h>
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/timer.h"

typedef enum {
    BTN_LEFT,
    BTN_MID_LEFT,
    BTN_MID_RIGHT,
    BTN_RIGHT,
    BTN_ENCODER,
    BTN_MAX_INDEX
} button;

typedef enum encoder_dir_enum {
    ENCODER_DIR_FORWARD,
    ENCODER_DIR_BACKWARD
} encoder_dir;

void buttons_init(void);

bool button_is_pressed(button btn);

void buttons_deinit(button btn);

int32_t encoder_init(void);

int32_t encoder_get_count_and_dir(uint32_t* count, encoder_dir* dir);

void encoder_deinit(void);

#endif /* PLATFORM_FP_CONTROLS_DRIVER_H_ */
