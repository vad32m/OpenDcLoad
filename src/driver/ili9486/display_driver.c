/*
 * driver.c
 *
 *  Created on: Jun 30, 2019
 *      Author: vadym
 */

#include "display_driver.h"

#include <stddef.h>

#include "../../platform/display_connection.h"

#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 320

#define MEMORY_ACCESS_CTL_REG 0x36
#define MEMORY_ACCESS_CTL_MY (1 << 7)
#define MEMORY_ACCESS_CTL_MX (1 << 6)
#define MEMORY_ACCESS_CTL_MV (1 << 5)
#define MEMORY_ACCESS_CTL_ML (1 << 4)
#define MEMORY_ACCESS_CTL_BGR (1 << 3)

#define DISPLAY_INVERSION_CTL_REG 0xB4
#define POWER_CTL_2_REG 0xC1
#define VCOM_CTL_REG 0xC5
#define POSITIVE_GAMMA_CTL_REG 0xE0
#define NEGATIVE_GAMMA_CORRECTION_REG 0xE1
#define SLEEP_OUT_REG 0x11

#define DISPLAY_ON_REG 0x29

#define INTERFACE_PIXEL_FORMAT_REG 0x3A

#define COLUMN_ADRESS_SET 0x2A
#define PAGE_ADRESS_SET 0x2B
#define MEMORY_WRITE_CMD 0x2C

typedef struct
{
    const uint8_t address;
    const uint8_t* data;
    const uint8_t data_len;

} reg_write_cmd;

static const reg_write_cmd init_magic[] = {
        {0xF1, (const uint8_t[]){0x36, 0x04, 0x00, 0x3C, 0x0F, 0x8F}, 6},
        {0xF2, (const uint8_t[]){0x18, 0xA3, 0x12, 0x02, 0xB2, 0x12, 0xFF, 0x10, 0x00}, 9},
        {0XF8, (const uint8_t[]){0x21, 0x04}, 2},
        {0XF9, (const uint8_t[]){0x00, 0x08}, 2}
};

static const reg_write_cmd setup_sequence[] = {
        {DISPLAY_INVERSION_CTL_REG, (const uint8_t[]){0x00}, 1},
        {POWER_CTL_2_REG, (const uint8_t[]){0x41}, 1},
        {VCOM_CTL_REG, (const uint8_t[]){0x00, 0x91, 0x80, 0x00}, 4},
        {POSITIVE_GAMMA_CTL_REG, (const uint8_t[]){0x0F, 0x1F, 0x1C, 0x0C, 0x0F,
        0x08, 0x48, 0x98, 0x37, 0x0A, 0x13, 0x04, 0x11, 0x0D, 0x00}, 15},
        {NEGATIVE_GAMMA_CORRECTION_REG, (const uint8_t[]){0x04, 0x32, 0x2E, 0x0B,
        0x0D, 0x05, 0x47, 0x75, 0x37, 0x06, 0x10, 0x03, 0x24, 0x20, 0x00}, 15},
        {INTERFACE_PIXEL_FORMAT_REG, (const uint8_t[]){0x55}, 1},
        {SLEEP_OUT_REG, NULL, 0},
};

static void
display_reg_write(uint8_t reg_address, const uint8_t* reg_data, uint8_t data_len)
{
    display_connection_write_cmd(reg_address);

    for (uint32_t i = 0; i < data_len; i++) {
        display_connection_write_data(reg_data[i]);
    }
}

static void
display_cmd_seq_write(const reg_write_cmd* commands, uint32_t commands_num)
{
    const reg_write_cmd* cmd;
    for (uint32_t i = 0; i < commands_num; i++)
    {
        cmd = &commands[i];
        display_reg_write(cmd->address, cmd->data, cmd->data_len);
    }
}

int32_t
display_init(struct display_driver* driver, enum display_orientation orient)
{
    display_cmd_seq_write(init_magic, sizeof(init_magic)/sizeof(init_magic[0]));
    display_cmd_seq_write(setup_sequence, sizeof(setup_sequence)/sizeof(setup_sequence[0]));

    switch (orient) {
    case displayOrientationVertical:
        driver->width = DISPLAY_WIDTH;
        driver->height = DISPLAY_HEIGHT;
        display_connection_write_cmd(MEMORY_ACCESS_CTL_REG);
        display_connection_write_data(MEMORY_ACCESS_CTL_MX | MEMORY_ACCESS_CTL_BGR);
        break;
    case displayOrientationHorizontal:
        driver->width = DISPLAY_HEIGHT;
        driver->height = DISPLAY_WIDTH;
        display_connection_write_cmd(MEMORY_ACCESS_CTL_REG);
        display_connection_write_data(MEMORY_ACCESS_CTL_ML | MEMORY_ACCESS_CTL_MV | MEMORY_ACCESS_CTL_BGR);
        break;
    case displayOrientationVerticalFlipped:
        driver->width = DISPLAY_WIDTH;
        driver->height = DISPLAY_HEIGHT;
        display_connection_write_cmd(MEMORY_ACCESS_CTL_REG);
        display_connection_write_data(MEMORY_ACCESS_CTL_MY | MEMORY_ACCESS_CTL_BGR);
        break;
    case displayOrientationHorizontalFlipped:
        driver->width = DISPLAY_HEIGHT;
        driver->height = DISPLAY_WIDTH;
        display_connection_write_cmd(MEMORY_ACCESS_CTL_REG);
        display_connection_write_data(MEMORY_ACCESS_CTL_MX | MEMORY_ACCESS_CTL_MY | MEMORY_ACCESS_CTL_MV | MEMORY_ACCESS_CTL_BGR);
        break;
    default:
        break;
    }

    display_connection_write_cmd(DISPLAY_ON_REG);
    return 0;
}

void
display_set_window(uint16_t xStart, uint16_t yStart,uint16_t xEnd,uint16_t yEnd)
{
    display_connection_write_cmd(COLUMN_ADRESS_SET);
    display_connection_write_data(xStart >> 8);
    display_connection_write_data(0x00FF & xStart);
    display_connection_write_data(xEnd >> 8);
    display_connection_write_data(0x00FF & xEnd);

    display_connection_write_cmd(PAGE_ADRESS_SET);
    display_connection_write_data(yStart >> 8);
    display_connection_write_data(0x00FF & yStart);
    display_connection_write_data(yEnd >> 8);
    display_connection_write_data(0x00FF & yEnd);
}

static void
display_write_ram_prepare(void)
{
    display_connection_write_cmd(MEMORY_WRITE_CMD);
}


void
display_clear(struct display_driver* display, uint16_t color)
{
    display_set_window(0, 0, display->height - 1, display->width - 1);
    display_write_ram_prepare();
    display_connection_duplicate_data(color, 320 * 480);
}

void
display_fill_area(const struct display_area* area, uint16_t color)
{
    uint32_t number_of_pixels = (area->xEnd - area->xStart) * (area->yEnd - area->yStart);
    display_set_window(area->xStart, area->yStart, area->xEnd - 1, area->yEnd - 1);
    display_write_ram_prepare();
    display_connection_duplicate_data(color, number_of_pixels);
}

void
display_write_image(const struct display_area* area, const uint16_t* image)
{
    uint32_t number_of_pixels = (area->xEnd - area->xStart) * (area->yEnd - area->yStart);
    display_set_window(area->xStart, area->yStart, area->xEnd - 1, area->yEnd - 1);
    display_write_ram_prepare();
    display_connection_write_data_bulk(image, number_of_pixels);
}
