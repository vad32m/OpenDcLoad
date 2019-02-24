#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/systick.h>

#include "FreeRTOS.h"
#include "task.h"

static void
gpio_setup(void) {

   /* Enable GPIOC clock. */
   rcc_periph_clock_enable(RCC_GPIOA);

   /* Set GPIO8 (in GPIO port C) to 'output push-pull'. */
   gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO7);
}

/* Set up a timer to create 1mS ticks. */
static void
systick_setup(void)
{
    /* clock rate / 1000 to get 1mS interrupt rate */
    systick_set_reload(168000);
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    systick_counter_enable();
    /* this done last */
    systick_interrupt_enable();
}

/* Set STM32 to 168 MHz. */
static void
clock_setup(void)
{
    rcc_clock_setup_hse_3v3(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
}


static void
task1(void *args __attribute((unused))) {
    for (;;) {
        gpio_toggle(GPIOA,GPIO7);
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}


int
main(void) {
    int i;

    clock_setup();
    systick_setup();
    gpio_setup();

    xTaskCreate(task1,"LED",100,NULL,configMAX_PRIORITIES-1,NULL);
    vTaskStartScheduler();

    for (;;);
    return 0;
}