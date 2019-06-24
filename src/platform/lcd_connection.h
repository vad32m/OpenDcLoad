#ifndef __LCD_CONNECTION__
#define __LCD_CONNECTION__

#include <stdint.h>

int32_t
lcd_connection_init(void);

void
lcd_connection_write_cmd(uint8_t cmd);

void
lcd_connection_write_data(uint8_t data);

void
lcd_connection_write_data_16(uint16_t data);

uint8_t
lcd_connection_read_data(void);

#endif //__LCD_CONNECTION__
