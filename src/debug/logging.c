#include "debug/logging.h"
#include <stdio.h>
#include <stdarg.h>

void logger_init(void)
{

}


int32_t logger_start_message(void)
{
    return 1;
}

void logger_put_string(int32_t message_handler, const char *string)
{
    printf(string);
}

void logger_printf(int32_t message_handler, const char *format, ...)
{
    va_list myargs;
    va_start(myargs, format);
    vprintf(format, myargs);
    va_end(myargs);
}

void logger_put_decimal(int32_t message_handler, uint32_t number)
{
    printf("%d", number);
}

void logger_end_message(int32_t message_handler)
{
    puts(" ");
}

void logger_print_byte_array(int32_t message_handler, void *memory, uint32_t _array_size)
{
    
}