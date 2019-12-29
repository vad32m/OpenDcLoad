/**
 * @file
 * Work mode facade interface, controls selection of work modes
 */

#ifndef APP_WORK_MODE_FACADE_H_
#define APP_WORK_MODE_FACADE_H_

#include <stdint.h>

#include "app/work_mode.h"

enum wm_event {
	WM_EVENT_MODE_ACTIVATED,
	WM_EVENT_MODE_DEACTIVATED
};

typedef void (*wm_change_listener_fxn)(enum wm_event event);

int32_t wm_facade_init(void);

int32_t wm_facade_get_mode_count(void);

work_mode_t* wm_facade_get_mode(uint8_t idx);

int32_t wm_facade_get_active_mode(void);

void wm_facade_request_mode_deactivation(void);

void wm_facade_request_mode_activate(uint8_t mode_idx);

int32_t wm_facade_subscribe_listener(wm_change_listener_fxn listener);

void wm_facade_deinit(void);

#endif /* APP_WORK_MODE_FACADE_H_ */
