#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

static void
gpio_setup(void) {

   /* Enable GPIOC clock. */
   rcc_periph_clock_enable(RCC_GPIOA);

   /* Set GPIO8 (in GPIO port C) to 'output push-pull'. */
   gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO7);
}

int
main(void) {
   int i;

   gpio_setup();

   for (;;) {
       gpio_clear(GPIOA, GPIO7);      
       for (i = 0; i < 1500000; i++)  
           __asm__("nop");

       gpio_set(GPIOA, GPIO7);        
       for (i = 0; i < 500000; i++)
           __asm__("nop");
   }

   return 0;
}