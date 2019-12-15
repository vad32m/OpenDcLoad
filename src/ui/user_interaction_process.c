#include <libopencm3/stm32/gpio.h>

#include "ui/user_interaction_process.h"
#include "ui/display_lvgl_adapter.h"
#include "ui/screen.h"
#include "error_codes.h"

#include "lv_task.h"

#include "FreeRTOS.h"
#include "task.h"

static void
user_interaction_task(void *args __attribute((unused)))
{
    ui_screen_t* screen = screen_get_instance();
    for (;;) {
       screen_process(screen);
       vTaskDelay(pdMS_TO_TICKS(50));
       gpio_toggle(GPIOA, GPIO7);
       lv_task_handler();
    }
}

int32_t user_interaction_init(struct display_driver* display)
{
    lv_init();
    display_lvgl_adapter_init(display);
    return ERR_OK;
}

int32_t user_interaction_start_process(uint32_t priority)
{
    xTaskCreate(user_interaction_task, "LED", 1024, NULL, priority, NULL);
    return ERR_OK;
}

