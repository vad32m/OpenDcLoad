#include <stdbool.h>

#include "app/work_mode_facade.h"
#include "FreeRTOS.h"
#include "task.h"

#include "debug/assertions.h"
#include "error_codes.h"

#include "wm_constant_current.h"
#include "wm_constant_voltage.h"
#include "wm_constant_resistance.h"
#include "wm_constant_power.h"

enum work_mode_index {
	WM_CONSTANT_CURRENT,
	WM_CONSTANT_VOLTAGE,
	WM_CONSTANT_RESISTANCE,
	WM_CONSTANT_POWER,

	WM_MODE_COUNT
};

work_mode_t* modes[WM_MODE_COUNT];

static void
wm_periodic_task(void *args __attribute((unused)))
{
    for (;;) {
       vTaskDelay(pdMS_TO_TICKS(50));
    }
}

int32_t wm_facade_init(uint32_t wm_thread_priority)
{
	modes[WM_CONSTANT_CURRENT] = wm_constant_current_init();
	modes[WM_CONSTANT_VOLTAGE] = wm_constant_voltage_init();
	modes[WM_CONSTANT_RESISTANCE] = wm_constant_resistance_init();
	modes[WM_CONSTANT_POWER] = wm_constant_power_init();

	xTaskCreate(wm_periodic_task, "wm_thread", 1024, NULL, wm_thread_priority, NULL);
	return ERR_OK;
}

int32_t wm_facade_get_mode_count(void)
{
	return WM_MODE_COUNT;
}

work_mode_t* wm_facade_get_mode(uint8_t idx)
{
	if (idx < WM_MODE_COUNT) {
		return modes[idx];
	} else {
		ASSERT_TRUE(false);
		return NULL;
	}
}


