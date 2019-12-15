#include "platform/fp_controls_driver.h"
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

typedef struct {;
    uint32_t port;
    uint32_t pin;
} button_pin_t;

button_pin_t buttons[BTN_MAX_INDEX];

void buttons_init()
{
    buttons[BTN_LEFT].port = GPIOE; buttons[BTN_LEFT].pin = GPIO3;
    buttons[BTN_MID_LEFT].port = GPIOE; buttons[BTN_MID_LEFT].pin = GPIO4;
    buttons[BTN_MID_RIGHT].port = GPIOE; buttons[BTN_MID_RIGHT].pin = GPIO5;
    buttons[BTN_RIGHT].port = GPIOE; buttons[BTN_RIGHT].pin = GPIO6;
    buttons[BTN_ENCODER].port = GPIOC; buttons[BTN_ENCODER].pin = GPIO13;

    rcc_periph_clock_enable(RCC_GPIOE);
    rcc_periph_clock_enable(RCC_GPIOC);

    for (uint8_t i = 0; i < BTN_MAX_INDEX; i++)
    {
        gpio_mode_setup(buttons[i].port, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, buttons[i].pin);
    }

}

bool button_is_pressed(button btn)
{
    return !gpio_get(buttons[btn].port, buttons[btn].pin);
}

int32_t encoder_init(void)
{
    rcc_periph_clock_enable(RCC_GPIOB);
    gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO7 | GPIO6);
    gpio_set_af(GPIOB, GPIO_AF2, GPIO7 | GPIO6);

    rcc_periph_clock_enable(RCC_TIM3);
    timer_set_period(TIM4, 1024);
    timer_slave_set_mode(TIM4, 0x3); // encoder
    timer_ic_set_input(TIM4, TIM_IC1, TIM_IC_IN_TI1);
    timer_ic_set_input(TIM4, TIM_IC2, TIM_IC_IN_TI2);
    timer_enable_counter(TIM4);
}

int32_t encoder_get_count_and_dir(uint32_t* count, encoder_dir* dir)
{
    *count = timer_get_counter(TIM4);
    *dir = (TIM14_CCR1 | TIM_CR1_DIR_DOWN) ? ENCODER_DIR_BACKWARD : ENCODER_DIR_FORWARD;
}

