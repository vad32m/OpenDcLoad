#include <libopencm3/cm3/mpu.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/scb.h>

#include <libopencm3/stm32/rcc.h>

#include "platform/core_setup.h"
#include "sys_config.h"

#define MPU_RASR_SIZE_32B (0x04 << MPU_RASR_SIZE_LSB)
#define MPU_RASR_SIZE_64KB (0x0F << MPU_RASR_SIZE_LSB)
#define MPU_RASR_SIZE_128KB (0x10 << MPU_RASR_SIZE_LSB)
#define MPU_RASR_SIZE_256KB (0x11 << MPU_RASR_SIZE_LSB)
#define MPU_RASR_SIZE_512KB (0x12 << MPU_RASR_SIZE_LSB)
#define MPU_RASR_SIZE_1MB (0x13 << MPU_RASR_SIZE_LSB)
#define MPU_RASR_SIZE_32MB (0x18 << MPU_RASR_SIZE_LSB)
#define MPU_RASR_SIZE_64MB (0x19 << MPU_RASR_SIZE_LSB)

typedef struct {
    uint32_t start_address;
    uint32_t region_size;
    uint32_t permissions;
} mpu_region_t;

static mpu_region_t mpu_regions[] = {
    /**
     * 0x0800 0000 - 512K RX
     */
    {.start_address = 0x08000000, .region_size = MPU_RASR_SIZE_512KB, 
    .permissions = MPU_RASR_ATTR_AP_PRO_URO | MPU_RASR_ATTR_C },
    /**
     * 0x2000 0000 - 128k RW region
     */
    {.start_address = 0x20000000, .region_size = MPU_RASR_SIZE_128KB,
    .permissions = MPU_RASR_ATTR_AP_PRW_URW | MPU_RASR_ATTR_XN | MPU_RASR_ATTR_C | MPU_RASR_ATTR_S },
    /**
     * 0x2200 0000 32Mb RW bitband alias (8MB)
     */
    {.start_address = 0x22000000, .region_size = MPU_RASR_SIZE_32MB,
    .permissions = MPU_RASR_ATTR_AP_PRW_URW | MPU_RASR_ATTR_XN | MPU_RASR_ATTR_C | MPU_RASR_ATTR_S },
    /**
     * 0xE000 0000 1Mb RW - PPB (SCB SCS) (XN by default)
     * There is no need to set up memory regions for Private Peripheral Bus (PPB)
     */
    {.start_address = 0xE0000000, .region_size = MPU_RASR_SIZE_1MB,
    .permissions = MPU_RASR_ATTR_AP_PRW_URW | MPU_RASR_ATTR_XN | MPU_RASR_ATTR_B | MPU_RASR_ATTR_S },
    /**
     * 0x4000 0000 32Mb RW - Peripherals + bitband (XN by default)
     */
    {.start_address = 0x40000000, .region_size = MPU_RASR_SIZE_32MB,
    .permissions = MPU_RASR_ATTR_AP_PRW_URW | MPU_RASR_ATTR_XN | MPU_RASR_ATTR_B | MPU_RASR_ATTR_S },
    /**
	 * 0x1000 0000 - 64k CCM RAM RW region
	 */
    {.start_address = 0x10000000, .region_size = MPU_RASR_SIZE_64KB,
    .permissions = MPU_RASR_ATTR_AP_PRW_URW | MPU_RASR_ATTR_XN | MPU_RASR_ATTR_C | MPU_RASR_ATTR_S },
	/**
	 * 0x60040000 - 0x60040000 - FSMC
	 */
    {.start_address = 0x60000000, .region_size = MPU_RASR_SIZE_512KB,
     .permissions = MPU_RASR_ATTR_AP_PRW_URW | MPU_RASR_ATTR_XN}
    /**
     * 0xA000 0000 32Mb RW - Peripheral (AHB3??
     */
};

static void
mpu_setup_regions(mpu_region_t *config, uint8_t num_regions)
{
    if (MPU_TYPE && MPU_TYPE_DREGION) {
        MPU_CTRL = 0; //MPU_CTRL_PRIVDEFENA is disabled

        for (uint8_t i = 0; i < num_regions; i++) {
            MPU_RNR = i;
            MPU_RBAR = config[i].start_address;
            MPU_RASR = config[i].permissions | config[i].region_size | MPU_RASR_ENABLE;
        }

        // Disabled unused regions
		for (uint8_t i = num_regions; i < 8; i++) {
			MPU_RNR = i;
			MPU_RBAR = 0;
			MPU_RASR = 0;
		}

        MPU_CTRL |= MPU_CTRL_ENABLE;
    }
}

void
mpu_setup(void)
{
    mpu_setup_regions(mpu_regions, sizeof(mpu_regions)/sizeof(mpu_regions[0]));
}

/* Set up a timer to create 1mS ticks. */
void
systick_setup(void)
{
    /* clock rate / 1000 to get 1mS interrupt rate */
    systick_set_reload(168000);
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    systick_counter_enable(); 
}

/* Set STM32 to 168 MHz. */
void
clock_setup(void)
{
    rcc_clock_setup_hse_3v3(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
}

void
irq_setup(void)
{
    //16 bits for priority, no sub-priorities
    SCB_AIRCR = SCB_AIRCR_VECTKEYSTAT
                    | SCB_AIRCR_PRIGROUP_GROUP16_NOSUB;

    //enable specific fault interrupts
    SCB_SHCSR |= SCB_SHCSR_MEMFAULTENA
                    | SCB_SHCSR_USGFAULTENA
                    | SCB_SHCSR_BUSFAULTENA;

    SCB_CCR |= SCB_CCR_DIV_0_TRP;
    //No need to config systick and pendsv, they are configured by FreeRTOS
    nvic_enable_irq(NVIC_HARD_FAULT_IRQ);

    nvic_enable_irq(NVIC_BUS_FAULT_IRQ);
    nvic_set_priority(NVIC_BUS_FAULT_IRQ, SYSCONF_FAULT_INTERRUPT_PRIORITY);

    nvic_enable_irq(NVIC_MEM_MANAGE_IRQ);
    nvic_set_priority(NVIC_MEM_MANAGE_IRQ, SYSCONF_FAULT_INTERRUPT_PRIORITY);

    nvic_enable_irq(NVIC_USAGE_FAULT_IRQ);
    nvic_set_priority(NVIC_USAGE_FAULT_IRQ, SYSCONF_FAULT_INTERRUPT_PRIORITY);

}
