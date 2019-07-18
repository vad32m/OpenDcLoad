#include <stdint.h>
#include <stdbool.h>

#include <libopencm3/stm32/fsmc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/f4/nvic.h>

#include "display_connection.h"

#define LOG_LEVEL LL_DEBUG
#define LOG_DISPLAY_FILE 0
#define LOG_DISPLAY_LINE 0

#include "debug/logging.h"

#define LCD_DATA 0x60040000
#define LCD_REG  0x60000000

#define FSMC_BCR_MTYP_OFFSET 2
#define FSMC_BCR_MTYP_NOR (FSMC_BCR_MTYP_OFFSET << 0x02)

#define DMA_TRANSACTION_MAX_ITEMS 65535

static struct {
    uint16_t* pending_address;
    uint32_t data_left;
    bool data_increment;
    bool in_progress;
    xfer_completed_cb xfer_completed;
} dma_ongoing_transaction;

static int32_t
fsmc_init()
{
	rcc_periph_clock_enable(RCC_FSMC);
    FSMC_BCR1 = FSMC_BCR_WREN | FSMC_BCR_MBKEN | FSMC_BCR_MTYP_NOR | FSMC_BCR_FACCEN | FSMC_BCR_MUXEN;
    FSMC_BTR1 = FSMC_BTR_BUSTURNx(3) | FSMC_BTR_DATASTx(4) | FSMC_BTR_ADDSETx(3);
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

void dma2_stream0_isr(void)
{
    if (dma_get_interrupt_flag(DMA2, DMA_STREAM0, DMA_TCIF)) {

        dma_disable_stream(DMA2, DMA_STREAM0);
		dma_clear_interrupt_flags(DMA2, DMA_STREAM0, DMA_TCIF);

        if (dma_ongoing_transaction.data_left) {
            uint32_t data_xfer_count;
            if (dma_ongoing_transaction.data_left > DMA_TRANSACTION_MAX_ITEMS) {
                data_xfer_count = DMA_TRANSACTION_MAX_ITEMS;
                dma_ongoing_transaction.data_left -= DMA_TRANSACTION_MAX_ITEMS;
            } else {
                data_xfer_count = dma_ongoing_transaction.data_left;
                dma_ongoing_transaction.data_left = 0;
            }

            dma_set_peripheral_address(DMA2, DMA_STREAM0, dma_ongoing_transaction.pending_address);
            dma_set_number_of_data(DMA2, DMA_STREAM0, data_xfer_count);


            if (dma_ongoing_transaction.data_increment) {
                dma_ongoing_transaction.pending_address += data_xfer_count;
                dma_enable_peripheral_increment_mode(DMA2, DMA_STREAM0);
            } else {
                dma_disable_peripheral_increment_mode(DMA2, DMA_STREAM0);
            }
            dma_enable_stream(DMA2, DMA_STREAM0);
        } else {

            //callback can be null
            if (dma_ongoing_transaction.xfer_completed) {
                dma_ongoing_transaction.xfer_completed();
            }

            dma_ongoing_transaction.in_progress = false;
        }
    }

    if (dma_get_interrupt_flag(DMA2, DMA_STREAM0, DMA_HTIF)) {
		dma_clear_interrupt_flags(DMA2, DMA_STREAM0, DMA_HTIF);
    }
}

static void
dma_init()
{
	rcc_periph_clock_enable(RCC_DMA2);
    dma_stream_reset(DMA2, DMA_STREAM0);
    dma_set_transfer_mode(DMA2, DMA_STREAM0, DMA_SxCR_DIR_MEM_TO_MEM);
    dma_set_priority(DMA2, DMA_STREAM0, DMA_SxCR_PL_LOW);
    dma_set_memory_size(DMA2, DMA_STREAM0, DMA_SxCR_MSIZE_16BIT);
    dma_set_memory_address(DMA2, DMA_STREAM0, LCD_DATA);
    dma_set_peripheral_size(DMA2, DMA_STREAM0, DMA_SxCR_PSIZE_16BIT);
	nvic_enable_irq(NVIC_DMA2_STREAM0_IRQ);
    dma_enable_transfer_complete_interrupt(DMA2, DMA_STREAM0);
    dma_disable_half_transfer_interrupt(DMA2, DMA_STREAM0);
}

void
display_connection_duplicate_data(uint16_t number, uint32_t repetitions, xfer_completed_cb callback)
{
    static uint16_t data;
    
    while (dma_ongoing_transaction.in_progress) {}
    dma_ongoing_transaction.in_progress = true;

    dma_disable_stream(DMA2, DMA_STREAM0);
    data = number;
    dma_ongoing_transaction.xfer_completed = callback;

    if (repetitions > DMA_TRANSACTION_MAX_ITEMS) {
        dma_ongoing_transaction.data_left = repetitions - DMA_TRANSACTION_MAX_ITEMS;
        dma_ongoing_transaction.pending_address = &data;
        dma_ongoing_transaction.data_increment = false;
        repetitions = DMA_TRANSACTION_MAX_ITEMS;
    } else {
        dma_ongoing_transaction.data_left = 0;
        dma_ongoing_transaction.pending_address = 0;
        dma_ongoing_transaction.data_increment = false;
    }

    dma_disable_peripheral_increment_mode(DMA2, DMA_STREAM0);
    dma_set_peripheral_address(DMA2, DMA_STREAM0, &data);
    dma_set_number_of_data(DMA2, DMA_STREAM0, repetitions);
    dma_enable_stream(DMA2, DMA_STREAM0);
}

void
display_connection_write_data_bulk(uint16_t* data, uint32_t data_count, xfer_completed_cb callback)
{

    while (dma_ongoing_transaction.in_progress) {}
    dma_ongoing_transaction.in_progress = true;

    dma_disable_stream(DMA2, DMA_STREAM0);
    dma_ongoing_transaction.xfer_completed = callback;

    if (data_count > DMA_TRANSACTION_MAX_ITEMS) {
        dma_ongoing_transaction.data_left = data_count - DMA_TRANSACTION_MAX_ITEMS;
        dma_ongoing_transaction.pending_address = data + DMA_TRANSACTION_MAX_ITEMS;
        dma_ongoing_transaction.data_increment = true;
        data_count = DMA_TRANSACTION_MAX_ITEMS;
    } else {
        dma_ongoing_transaction.data_left = 0;
        dma_ongoing_transaction.pending_address = 0;
        dma_ongoing_transaction.data_increment = false;
    }

    dma_enable_peripheral_increment_mode(DMA2, DMA_STREAM0);
    dma_set_peripheral_address(DMA2, DMA_STREAM0, data);
    dma_set_number_of_data(DMA2, DMA_STREAM0, data_count);
    dma_enable_stream(DMA2, DMA_STREAM0);
}

int32_t
display_connection_init(void)
{
    fsmc_init();
    gpio_ports_init();
    dma_init();
    return 0;
}

void
display_connection_write_cmd(uint8_t cmd)
{
    while (dma_ongoing_transaction.in_progress) {}
    *(volatile uint8_t*) (LCD_REG) = cmd;
}

void
display_connection_write_data(uint8_t data)
{
    while (dma_ongoing_transaction.in_progress) {}
    *(volatile uint8_t*) (LCD_DATA) = data;
}

void
display_connection_write_data_16(uint16_t data)
{
    while (dma_ongoing_transaction.in_progress) {}
    *(volatile uint16_t*) (LCD_DATA) = data;
}

uint8_t
display_connection_read_data(void)
{
    while (dma_ongoing_transaction.in_progress) {}
	uint8_t result = *(volatile uint8_t*) (LCD_DATA);
    return result;
}
