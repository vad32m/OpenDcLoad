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
    case displayOrientationHorizontal:
        driver->width = DISPLAY_WIDTH;
        driver->height = DISPLAY_HEIGHT;
        //0 degree MY=0,MX=0,MV=0,ML=0,BGR=1,MH=0
        display_connection_write_cmd(MEMORY_ACCESS_CTL_REG);
        display_connection_write_data(MEMORY_ACCESS_CTL_MX | MEMORY_ACCESS_CTL_BGR);
        break;
    case displayOrientationVertical:
        driver->width = DISPLAY_WIDTH;
        driver->height = DISPLAY_HEIGHT;
        //90 degree MY=0,MX=1,MV=1,ML=1,BGR=1,MH=0
        display_connection_write_cmd(MEMORY_ACCESS_CTL_REG);
        display_connection_write_data(MEMORY_ACCESS_CTL_ML | MEMORY_ACCESS_CTL_MV | MEMORY_ACCESS_CTL_BGR);
        break;
    default:
        break;
    }

    display_connection_write_cmd(DISPLAY_ON_REG);
    return 0;
}

void
display_set_window(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd)
{
    display_connection_write_cmd(0x2A);
    display_connection_write_data(xStar>>8);
    display_connection_write_data(0x00FF&xStar);
    display_connection_write_data(xEnd>>8);
    display_connection_write_data(0x00FF&xEnd);

    display_connection_write_cmd(0x2B);
    display_connection_write_data(yStar>>8);
    display_connection_write_data(0x00FF&yStar);
    display_connection_write_data(yEnd>>8);
}

static void
display_write_ram_prepare(void)
{
    display_connection_write_cmd(0x2C);
}


void
display_clear(struct display_driver* display, uint16_t color)
{
    display_set_window(0, 0, 479, 319);
    display_write_ram_prepare();

    for (uint32_t i = 0; i < 320 * 480; i++) {
        display_connection_write_data_16(color);
    }
}
