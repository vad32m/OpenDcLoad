#ifndef __LCD_CONNECTION__
#define __LCD_CONNECTION__

#include <stdint.h>

int32_t
display_connection_init(void);

void
display_connection_write_cmd(uint8_t cmd);

void
display_connection_write_data(uint8_t data);

void
display_connection_write_data_16(uint16_t data);

uint8_t
display_connection_read_data(void);

#endif //__LCD_CONNECTION__
