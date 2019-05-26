#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/itm.h>

#include <stdio.h>
#include <stdarg.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <string.h>

#include "debug/logging.h"
#include "sys_config.h"

struct log_buffer_t
{
    bool is_finished;
    char buffer[LOGGER_MESSAGE_MAX_LEN + 1];
    uint8_t print_pos;
};

static struct log_buffer_t buffers[LOGGER_BUFFER_MESSAGE_CAPACITY];

static volatile _Atomic int32_t write_head;
static volatile int32_t read_tail;

static void logging_print_data(void) {
	if ((read_tail != write_head)
		&& (buffers[read_tail].is_finished)
		&& (ITM_STIM8(0) & ITM_STIM_FIFOREADY)) {

		if (buffers[read_tail].buffer[buffers[read_tail].print_pos] == '\0') {
			ITM_STIM8(0) = '\n';
			//change this property exactly here to prevent race conditions
			buffers[read_tail].is_finished = false;
			read_tail = (read_tail + 1) % LOGGER_BUFFER_MESSAGE_CAPACITY;
		} else {
			ITM_STIM8(0) = buffers[read_tail].buffer[buffers[read_tail].print_pos];
			buffers[read_tail].print_pos++;
		}
	}
}

void logger_init(void)
{
    rcc_periph_clock_enable(RCC_TIM4);
    //TIM4 resides in APB1 domain
    //APB1 clock frequency can't be higher than 42 MHz
    //APB1 timer clock should be 84 Mhz
    timer_set_mode(TIM4, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_disable_preload(TIM4);
    timer_set_prescaler(TIM4, 0);

    const int32_t tim_cnt_frequency = 84000000;

    //we want to set interrupt for 20 khz frequency
    timer_set_period(TIM4, tim_cnt_frequency / 20000);
    timer_enable_counter(TIM4);
    timer_enable_irq(TIM4, TIM_DIER_UIE);

    nvic_enable_irq(NVIC_TIM4_IRQ);
    nvic_set_priority(NVIC_TIM4_IRQ, SYSCONF_LOGGER_INTERRUPT_PRIORITY);
}

void tim4_isr(void)
{
    if (timer_get_flag(TIM4, TIM_SR_UIF)) {
		logging_print_data();
        timer_clear_flag(TIM4, TIM_SR_UIF);
    }
}

int32_t logger_start_message(void)
{
    //TODO: retries
    while (1)
    {
        int32_t old_head = atomic_load(&write_head);
        int32_t new_head = (old_head + 1) % LOGGER_BUFFER_MESSAGE_CAPACITY;

        if (new_head == read_tail)
        {
            //error - buffer overlap
            return -1;
        }

        if (atomic_compare_exchange_strong(&write_head, &old_head, new_head))
        {
            buffers[old_head].buffer[0] = '\0';
            buffers[old_head].print_pos = 0;
            return old_head;
        }
    }
}

void logger_put_string(int32_t message_handler, const char *string)
{
    //TODO: advanced checking (keep length of the string in buffer)
    strncat(buffers[message_handler].buffer, string, LOGGER_MESSAGE_MAX_LEN - strlen(buffers[message_handler].buffer));
}

void logger_printf(int32_t message_handler, const char *format, ...)
{
    size_t length = strlen(buffers[message_handler].buffer);
    va_list myargs;

    va_start(myargs, format);
    vsnprintf(buffers[message_handler].buffer + length, LOGGER_MESSAGE_MAX_LEN - length, format, myargs);
    va_end(myargs);
}

void logger_put_decimal(int32_t message_handler, uint32_t number)
{
	size_t length = strlen(buffers[message_handler].buffer);
    snprintf(buffers[message_handler].buffer + length, LOGGER_MESSAGE_MAX_LEN - length, "%lu", number);
}

void logger_end_message(int32_t message_handler)
{
    buffers[message_handler].is_finished = true;
}

void logger_flush(int32_t message_handler)
{
    while (buffers[message_handler].is_finished != false) {};
}

void logger_print_byte_array(int32_t message_handler, void *memory, uint32_t array_size)
{
    size_t length = strlen(buffers[message_handler].buffer);
    uint8_t *bytes = memory;
    
    for (uint32_t array_index = 0; array_index < array_size; array_index++)
    {
        snprintf(buffers[message_handler].buffer + length, LOGGER_MESSAGE_MAX_LEN - length, " %02X", bytes[array_index]);
        length += 3;
    }
}
