#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/dbgmcu.h>

#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/itm.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/mpu.h>

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#include "fault_handler.h"
#include "FreeRTOS.h"
#include "task.h"

#define CONFIG_BUSFAULT_INTERRUPT_PRIORITY 0x1

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

/* Set up a timer to create 1mS ticks. */
static void
systick_setup(void)
{
    /* clock rate / 1000 to get 1mS interrupt rate */
    systick_set_reload(168000);
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    systick_counter_enable(); 
}

/* Set STM32 to 168 MHz. */
static void
clock_setup(void)
{
    rcc_clock_setup_hse_3v3(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
}

void
mem_manage_handler(void)
{
    REPORT_STACK_FRAME
    ReportMemanageFault();
}

void
hard_fault_handler(void)
{
    REPORT_STACK_FRAME
    ReportHardFault();
}

void
bus_fault_handler(void)
{
    REPORT_STACK_FRAME
    ReportBusFault();
}

void
usage_fault_handler(void)
{
    REPORT_STACK_FRAME
    ReportUsageFault();
}

static void
irq_setup(void)
{
    SCB_AIRCR = SCB_AIRCR_VECTKEYSTAT
                    | SCB_AIRCR_PRIGROUP_GROUP16_NOSUB;

    SCB_SHCSR |= SCB_SHCSR_MEMFAULTENA
                    | SCB_SHCSR_USGFAULTENA
                    | SCB_SHCSR_BUSFAULTENA;

    SCB_CCR |= SCB_CCR_DIV_0_TRP;
    //No need to set priority for systick and pendsv, it is configured by FreeRTOS
    nvic_enable_irq(NVIC_HARD_FAULT_IRQ);
    nvic_enable_irq(NVIC_BUS_FAULT_IRQ);
    nvic_enable_irq(NVIC_MEM_MANAGE_IRQ);
    nvic_enable_irq(NVIC_USAGE_FAULT_IRQ);

}

static void
task1(void *args __attribute((unused)))
{
    for (;;) {
        gpio_toggle(GPIOA,GPIO7);
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
#define MPU_RASR_SIZE_32B (0x04 << MPU_RASR_SIZE_LSB)
#define MPU_RASR_SIZE_64KB (0x0F << MPU_RASR_SIZE_LSB)
#define MPU_RASR_SIZE_128KB (0x10 << MPU_RASR_SIZE_LSB)
#define MPU_RASR_SIZE_512KB (0x12 << MPU_RASR_SIZE_LSB)
#define MPU_RASR_SIZE_1MB (0x13 << MPU_RASR_SIZE_LSB)
#define MPU_RASR_SIZE_32MB (0x18 << MPU_RASR_SIZE_LSB)
#define MPU_RASR_SIZE_64MB (0x19 << MPU_RASR_SIZE_LSB)

static void
configure_mpu(void)
{
    if (MPU_TYPE && MPU_TYPE_DREGION) {
        MPU_CTRL = 0; //MPU_CTRL_PRIVDEFENA is disabled

        /**
         * 0x0800 0000 - 512K RX
         */
        MPU_RNR = 0;
        MPU_RBAR = 0x08000000;
        MPU_RASR = MPU_RASR_ATTR_AP_PRO_URO | MPU_RASR_ATTR_C 
                    | MPU_RASR_SIZE_512KB | MPU_RASR_ENABLE;

        /**
         * 0x2000 0000 - 128k RW region
         */
        MPU_RNR = 1;
        MPU_RBAR = 0x20000000;
        MPU_RASR = MPU_RASR_ATTR_AP_PRW_URW | MPU_RASR_ATTR_XN | MPU_RASR_ATTR_C 
                    | MPU_RASR_ATTR_S | MPU_RASR_SIZE_128KB | MPU_RASR_ENABLE;


        /**
         * 0x2200 0000 32Mb RW bitband alias (8MB)
         */
        MPU_RNR = 2;
        MPU_RBAR = 0x22000000;
        MPU_RASR = MPU_RASR_ATTR_AP_PRW_URW | MPU_RASR_ATTR_XN | MPU_RASR_ATTR_C
                    | MPU_RASR_ATTR_S | MPU_RASR_SIZE_32MB | MPU_RASR_ENABLE;

        /**
         * 0xE000 0000 1Mb RW - PPB (SCB SCS) (XN by default)
         * There is no need to set up memory regions for Private Peripheral Bus (PPB)
         */
        MPU_RNR = 3;
        MPU_RBAR = 0xE0000000;
        MPU_RASR = MPU_RASR_ATTR_AP_PRW_URW | MPU_RASR_ATTR_XN | MPU_RASR_ATTR_B
                    | MPU_RASR_ATTR_S | MPU_RASR_SIZE_1MB | MPU_RASR_ENABLE;

        /**
         * 0x4000 0000 32Mb RW - Peripherals + bitband (XN by default)
         */
        MPU_RNR = 4;
        MPU_RBAR = 0x40000000;
        MPU_RASR = MPU_RASR_ATTR_AP_PRW_URW | MPU_RASR_ATTR_XN | MPU_RASR_ATTR_B
                    | MPU_RASR_ATTR_S | MPU_RASR_SIZE_32MB | MPU_RASR_ENABLE;

        /**
         * 0x0000 0000 - RO (FREERTOS fetches initial stack pointer from this address)
         */
        MPU_RNR = 5;
        MPU_RBAR = 0x00000000;
        MPU_RASR = MPU_RASR_ATTR_AP_PRO_UNO | MPU_RASR_ATTR_XN | MPU_RASR_ATTR_B
                     | MPU_RASR_ATTR_S | MPU_RASR_SIZE_32B | MPU_RASR_ENABLE;

		/**
		 * 0x1000 0000 - 64k CCM RAM RW region
		 */
		MPU_RNR = 6;
		MPU_RBAR = 0x10000000;
		MPU_RASR = MPU_RASR_ATTR_AP_PRW_URW | MPU_RASR_ATTR_XN | MPU_RASR_ATTR_C
				| MPU_RASR_ATTR_S | MPU_RASR_SIZE_64KB | MPU_RASR_ENABLE;
        /**
         * 0xA000 0000 32Mb RW - Peripheral (AHB3??
         */


		for (int i = 7; i < 8; i++) { // Disabled unused regions
			MPU_RNR = i;
			MPU_RBAR = 0;
			MPU_RASR = 0;
		}

        MPU_CTRL |= MPU_CTRL_ENABLE;
    }
}

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
#if !TEST_BUS_FAULT
    configure_mpu();
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

    #if TEST_BUS_FAULT
    //trigger precise bus fault by accessing memory behind the end of the RAM
    extern int _stack;
    int* behind_the_end = &_stack + 4;
    volatile int a = *behind_the_end;
    #endif

    #if TEST_USAGE_FAULT
    asm volatile("udf.w");
    #endif

    #if TEST_MEMMANAG_FAULT
    void (*exec_never_region)(void) = 0x40000000;
    exec_never_region();
    #endif

    #if TEST_DIV0_FAULT
    int a = 0;
    volatile int b = 10/a;
    #endif

    xTaskCreate(task1, "LED", 100, NULL, configMAX_PRIORITIES - 1, NULL);
    vTaskStartScheduler();

    for (;;);
    return 0;
}
