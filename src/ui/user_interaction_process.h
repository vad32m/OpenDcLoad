#include "driver/ili9486/display_driver.h"

int32_t user_interaction_init(struct display_driver* display);

int32_t user_interaction_start_process(uint32_t priority);

void user_interaction_stop_process(void);

void user_interaction_deinit(void);
