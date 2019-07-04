/*
 * driver.h
 *
 *  Created on: Jun 30, 2019
 *      Author: vadym
 */

#ifndef ILI9486_DRIVER_H_
#define ILI9486_DRIVER_H_

#include <stdint.h>

struct display_driver
{
	uint16_t width;
	uint16_t height;
};

enum display_orientation
{
	displayOrientationHorizontal,
	displayOrientationVertical
};

int32_t
display_init(struct display_driver* driver, enum display_orientation orient);

void
display_clear(struct display_driver* display, uint16_t color);

void
display_set_window(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd);


#endif /* ILI9486_DRIVER_H_ */
