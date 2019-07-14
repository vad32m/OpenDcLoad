/*
 * driver.h
 *
 *  Created on: Jun 30, 2019
 *      Author: vadym
 */

#ifndef ILI9486_DRIVER_H_
#define ILI9486_DRIVER_H_

#include <stdint.h>

typedef void (*display_not_busy_cb)(void);

struct display_driver
{
	uint16_t width;
	uint16_t height;
    display_not_busy_cb not_busy_cb;
};

enum display_orientation
{
	displayOrientationHorizontal,
	displayOrientationVertical,
	displayOrientationHorizontalFlipped,
	displayOrientationVerticalFlipped
};

struct display_area
{
    uint16_t xStart;
    uint16_t xEnd;
    uint16_t yStart;
    uint16_t yEnd;
};


int32_t
display_init(struct display_driver* driver, enum display_orientation orient);

void
display_set_callback(struct display_driver* driver, display_not_busy_cb callback);

void
display_clear(struct display_driver* display, uint16_t color);

void
display_set_window(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd);

void
display_fill_area(struct display_driver* display, const struct display_area* area, uint16_t color);

void
display_write_image(struct display_driver* display, const struct display_area* area, const uint16_t* image);

#endif /* ILI9486_DRIVER_H_ */
