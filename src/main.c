#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/systick.h>

#include <libopencm3/stm32/dbgmcu.h>
#include <libopencm3/cm3/scs.h>
#include <libopencm3/cm3/tpiu.h>
#include <libopencm3/cm3/itm.h>

#include "FreeRTOS.h"
#include "task.h"

static void
gpio_setup(void) {

   /* Enable GPIOC clock. */
   rcc_periph_clock_enable(RCC_GPIOA);
   rcc_periph_clock_enable(RCC_GPIOB);

   /* Set GPIO8 (in GPIO port C) to 'output push-pull'. */
   gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO7);
   gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO3);
   gpio_set_af(GPIOB, GPIO_AF0, GPIO3);
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
trace_send_blocking(char c)
{
	while (!(ITM_STIM8(0) & ITM_STIM_FIFOREADY))
		;

	ITM_STIM8(0) = c;
}

static void
task1(void *args __attribute((unused))) {
    for (;;) {
        gpio_toggle(GPIOA,GPIO7);
        vTaskDelay(pdMS_TO_TICKS(200));
        trace_send_blocking('0');
    }
}

static void
trace_setup(void)
{
	/* Enable trace subsystem (we'll use ITM and TPIU). */
	/*SCS_DEMCR |= SCS_DEMCR_TRCENA;

	/* Use Manchester code for asynchronous transmission. */
	TPIU_SPPR = TPIU_SPPR_ASYNC_MANCHESTER;
	TPIU_ACPR = 7;

	/* Formatter and flush control. */
	TPIU_FFCR &= ~TPIU_FFCR_ENFCONT;

	/* Enable TRACESWO pin for async mode. */
	DBGMCU_CR = DBGMCU_CR_TRACE_IOEN | DBGMCU_CR_TRACE_MODE_ASYNC;

	/* Unlock access to ITM registers. */
	/* FIXME: Magic numbers... Is this Cortex-M3 generic? */
	*((volatile uint32_t *)0xE0000FB0) = 0xC5ACCE55;

	/* Enable ITM with ID = 1. */
	ITM_TCR = (1 << 16) | ITM_TCR_ITMENA;
	/* Enable stimulus port 1. */
	ITM_TER[0] = 1;
}



int
main(void) {
    int i;

    clock_setup();
    systick_setup();
    gpio_setup();

    xTaskCreate(task1, "LED", 100, NULL, configMAX_PRIORITIES - 1, NULL);
    vTaskStartScheduler();

    for (;;);
    return 0;
}
