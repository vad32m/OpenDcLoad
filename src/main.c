#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "debug/logging.h"
#include "debug/assertions.h"
#include "platform/core_setup.h"
#include "platform/display_connection.h"

#include "ui/user_interaction_process.h"
#include "ui/screen.h"

#include "app/work_mode_facade.h"


static void
gpio_setup(void)
{
   /* Enable GPIOC clock. */
   rcc_periph_clock_enable(RCC_GPIOA);
   rcc_periph_clock_enable(RCC_GPIOB);

   /* Set GPIO8 (in GPIO port C) to 'output push-pull'. */
   gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO7);
   gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO3);
   gpio_set_af(GPIOB, GPIO_AF0, GPIO3);
}

struct display_driver display;

#define TEST_NULL_DEREF 0
#define TEST_BUS_FAULT 0
#define TEST_USAGE_FAULT 0
#define TEST_MEMMANAG_FAULT 0
#define TEST_DIV0_FAULT 0
#define TEST_OUT_OF_MEMORY 0

int
main(void)
{
    clock_setup();
    systick_setup();
    gpio_setup();
    irq_setup();
    logger_init();
    display_connection_init();
    display_init(&display, displayOrientationHorizontalFlipped);


#if !TEST_BUS_FAULT
    mpu_setup();
#endif

#if TEST_OUT_OF_MEMORY
    for (int i = 0; true; i++)
    {
        volatile char *example = malloc(2048);

        if (example)
        {
            *example = 10;
        }
        else
        {
            puts(strerror(errno));
        }
    }
#endif


#if TEST_NULL_DEREF
    volatile int* null_ptr = 0;
    volatile int a = *null_ptr;
#endif

#if TEST_BUS_FAULT
    //trigger precise bus fault by accessing memory behind the end of the RAM
    extern int _stack;
    int* behind_the_end = &_stack + 4;
    volatile int a = *behind_the_end;
#endif

#if TEST_USAGE_FAULT
    __asm volatile("udf.w");
#endif

#if TEST_MEMMANAG_FAULT
    void (*exec_never_region)(void) = 0x40000000;
    exec_never_region();
#endif

#if TEST_DIV0_FAULT
    int a = 0;
    volatile int b = 10/a;
#endif

    wm_facade_init(3);
    user_interaction_init(&display);
    user_interaction_start_process(4);
    ui_screen_t* scr = screen_get_instance();
    screen_set_active(scr, UI_SCREEN_MODE_SELECT);

    vTaskStartScheduler();

    for (;;);
    return 0;
}
