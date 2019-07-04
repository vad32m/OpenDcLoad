#include <stdint.h>

#include <libopencm3/stm32/fsmc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#define LOG_LEVEL LL_DEBUG
#define LOG_DISPLAY_FILE 0
#define LOG_DISPLAY_LINE 0

#include "debug/logging.h"

#define FSMC_BCR_MTYP_OFFSET 2
#define FSMC_BCR_MTYP_NOR (FSMC_BCR_MTYP_OFFSET << 0x02)

static int32_t
fsmc_init()
{
    FSMC_BCR1 = FSMC_BCR_WREN | FSMC_BCR_MBKEN | FSMC_BCR_MTYP_NOR | FSMC_BCR_FACCEN | FSMC_BCR_MUXEN;
    FSMC_BTR1 = FSMC_BTR_BUSTURNx(1) | FSMC_BTR_DATASTx(3) | FSMC_BTR_ADDSETx(3);
    return 0;
}

static int32_t
gpio_ports_init()
{
    const uint32_t gpiod_pins = GPIO0 | GPIO1 | GPIO4 | GPIO5 | GPIO7 | GPIO13 | GPIO14 | GPIO15;
    const uint32_t gpioe_pins = GPIO7 | GPIO8 | GPIO9 | GPIO10;

    rcc_periph_clock_enable(RCC_GPIOD);
    rcc_periph_clock_enable(RCC_GPIOE);

    gpio_mode_setup(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, gpiod_pins);
    gpio_set_af(GPIOD, GPIO_AF12, gpiod_pins);

    gpio_mode_setup(GPIOE, GPIO_MODE_AF, GPIO_PUPD_NONE, gpioe_pins);
    gpio_set_af(GPIOE, GPIO_AF12, gpioe_pins);
    return 0;
}

int32_t
display_connection_init(void)
{
	rcc_periph_clock_enable(RCC_FSMC);
    fsmc_init();
    gpio_ports_init();
    return 0;
}

#define LCD_DATA 0x60040000
#define LCD_REG  0x60000000

void
display_connection_write_cmd(uint8_t cmd)
{
    *(uint8_t*) (LCD_REG) = cmd;
}

void
display_connection_write_data(uint8_t data)
{
    *(uint8_t*) (LCD_DATA) = data;
}

void
display_connection_write_data_16(uint16_t data)
{
    *(uint16_t*) (LCD_DATA) = data;
}

uint8_t
display_connection_read_data(void)
{
	uint8_t result = *(uint8_t*) (LCD_DATA);
    return result;
}
