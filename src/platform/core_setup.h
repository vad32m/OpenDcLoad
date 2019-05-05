#ifndef _CORE_SETUP_H__
#define _CORE_SETUP_H__

#include <stdint.h>

void
mpu_setup(void);

/* Set up a timer to create 1mS ticks. */
void
systick_setup(void);

/* Set STM32 to 168 MHz. */
void
clock_setup(void);

void
irq_setup(void);

#endif //_CORE_SETUP_H__