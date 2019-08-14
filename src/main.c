#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "platform/core_setup.h"
#include "debug/logging.h"
#include "debug/assertions.h"
#include "driver/ili9486/display_driver.h"
#include "platform/display_connection.h"

#include "ui/display_lvgl_adapter.h"
#include "lvgl.h"
#include <math.h>

uint8_t  sine_wave[256] = {
  0x80, 0x83, 0x86, 0x89, 0x8C, 0x90, 0x93, 0x96,
  0x99, 0x9C, 0x9F, 0xA2, 0xA5, 0xA8, 0xAB, 0xAE,
  0xB1, 0xB3, 0xB6, 0xB9, 0xBC, 0xBF, 0xC1, 0xC4,
  0xC7, 0xC9, 0xCC, 0xCE, 0xD1, 0xD3, 0xD5, 0xD8,
  0xDA, 0xDC, 0xDE, 0xE0, 0xE2, 0xE4, 0xE6, 0xE8,
  0xEA, 0xEB, 0xED, 0xEF, 0xF0, 0xF1, 0xF3, 0xF4,
  0xF5, 0xF6, 0xF8, 0xF9, 0xFA, 0xFA, 0xFB, 0xFC,
  0xFD, 0xFD, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFE, 0xFD,
  0xFD, 0xFC, 0xFB, 0xFA, 0xFA, 0xF9, 0xF8, 0xF6,
  0xF5, 0xF4, 0xF3, 0xF1, 0xF0, 0xEF, 0xED, 0xEB,
  0xEA, 0xE8, 0xE6, 0xE4, 0xE2, 0xE0, 0xDE, 0xDC,
  0xDA, 0xD8, 0xD5, 0xD3, 0xD1, 0xCE, 0xCC, 0xC9,
  0xC7, 0xC4, 0xC1, 0xBF, 0xBC, 0xB9, 0xB6, 0xB3,
  0xB1, 0xAE, 0xAB, 0xA8, 0xA5, 0xA2, 0x9F, 0x9C,
  0x99, 0x96, 0x93, 0x90, 0x8C, 0x89, 0x86, 0x83,
  0x80, 0x7D, 0x7A, 0x77, 0x74, 0x70, 0x6D, 0x6A,
  0x67, 0x64, 0x61, 0x5E, 0x5B, 0x58, 0x55, 0x52,
  0x4F, 0x4D, 0x4A, 0x47, 0x44, 0x41, 0x3F, 0x3C,
  0x39, 0x37, 0x34, 0x32, 0x2F, 0x2D, 0x2B, 0x28,
  0x26, 0x24, 0x22, 0x20, 0x1E, 0x1C, 0x1A, 0x18,
  0x16, 0x15, 0x13, 0x11, 0x10, 0x0F, 0x0D, 0x0C,
  0x0B, 0x0A, 0x08, 0x07, 0x06, 0x06, 0x05, 0x04,
  0x03, 0x03, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x03,
  0x03, 0x04, 0x05, 0x06, 0x06, 0x07, 0x08, 0x0A,
  0x0B, 0x0C, 0x0D, 0x0F, 0x10, 0x11, 0x13, 0x15,
  0x16, 0x18, 0x1A, 0x1C, 0x1E, 0x20, 0x22, 0x24,
  0x26, 0x28, 0x2B, 0x2D, 0x2F, 0x32, 0x34, 0x37,
  0x39, 0x3C, 0x3F, 0x41, 0x44, 0x47, 0x4A, 0x4D,
  0x4F, 0x52, 0x55, 0x58, 0x5B, 0x5E, 0x61, 0x64,
  0x67, 0x6A, 0x6D, 0x70, 0x74, 0x77, 0x7A, 0x7D
};

uint8_t sinus(uint8_t arg)
{
    return sine_wave[arg];
}

static void
gpio_setup(void)
{
   /* Enable GPIOC clock. */
   rcc_periph_clock_enable(RCC_GPIOA);
   rcc_periph_clock_enable(RCC_GPIOB);

   /* Set GPIO8 (in GPIO port C) to 'output push-pull'. */
   gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO7);
   gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO3);
   gpio_set_af(GPIOB, GPIO_AF0, GPIO3);
}

struct display_driver display;

static void
disable_indent(lv_obj_t *object)
{
    lv_style_t* style = lv_obj_get_style(object);
    style->body.padding.bottom = 0;
    style->body.padding.inner = 0;
    style->body.padding.left = 0;
    style->body.padding.right = 0;
    style->body.padding.top = 0;
    lv_obj_set_style(object, style);
}

static const char* btnmap[] = {"First", "Second", LV_SYMBOL_OK, "Fourth", ""};
static const char* labels[] = {"I: 2 A", "U: 4 V", "R: 2 Ohm", "P: 8 W", ""};

lv_obj_t * chart;
lv_chart_series_t * series_a;
lv_chart_series_t * series_b;

void lv_tutorial_objects(void)
{
    lv_theme_t* th = lv_theme_night_init(20, NULL);
    lv_theme_set_current(th);
    /********************
     * CREATE A SCREEN
     *******************/
    /* Create a new screen and load it
     * Screen can be created from any type object type
     **/
    lv_obj_t* scr = lv_obj_create(NULL, NULL);
    disable_indent(scr);
    lv_disp_load_scr(scr);

    /****************
     * ADD A TITLE
     ****************/
    lv_obj_t* buttons = lv_btnm_create(scr, NULL);
    lv_btnm_set_map(buttons, btnmap);
    lv_obj_set_pos(buttons, 0, 285);
    lv_obj_set_size(buttons, 480, 35);
    disable_indent(buttons);

    //Create informational ribbon on top

    lv_obj_t* top_container = lv_cont_create(scr, NULL);
    lv_cont_set_fit(top_container, LV_FIT_NONE);
    lv_cont_set_layout(top_container, LV_LAYOUT_PRETTY);
    lv_obj_set_pos(top_container, 0, 0);
    lv_obj_set_size(top_container, 480, 35);

    int32_t i = 0;

    while (*(labels[i]))
    {
        lv_obj_t* label = lv_label_create(top_container, NULL);


        lv_label_set_text(label, labels[i]);
        i++;
    }

    chart = lv_chart_create(scr, NULL);

    lv_style_t* style = malloc(sizeof(lv_style_t));
    lv_style_copy(style, lv_obj_get_style(chart));
    
    style->text.font = &lv_font_roboto_12;
    lv_obj_set_style(chart, style);
    lv_obj_refresh_style(chart);

    lv_obj_set_pos(chart, 20, 50);
    lv_obj_set_size(chart, 440, 210);
    lv_chart_set_x_tick_texts(chart, "1\n2\n3\n4\n5\n6\n", 2, LV_CHART_AXIS_DRAW_LAST_TICK);
    lv_chart_set_x_tick_length(chart, 5, 8);
    lv_chart_set_y_tick_texts(chart, "1\n2\n3\n4\n", 2, LV_CHART_AXIS_DRAW_LAST_TICK & LV_CHART_AXIS_INVERSE_LABELS_ORDER);
    lv_chart_set_y_tick_length(chart, 5, 8);
    lv_chart_set_secondary_y_tick_texts(chart, "10\n20\n30\n40\n", 2, LV_CHART_AXIS_DRAW_LAST_TICK);
    lv_chart_set_secondary_y_tick_length(chart, 5, 8);
    lv_chart_set_margin(chart, 30);
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(chart, 30);
    lv_chart_set_range(chart, 0, UINT8_MAX);
    lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_SHIFT);
    lv_chart_set_series_width(chart, 6);

    series_a = lv_chart_add_series(chart, LV_COLOR_AQUA);
    series_b = lv_chart_add_series(chart, LV_COLOR_YELLOW);

    for (i = 0; i < 30; i++)
    {
        lv_chart_set_next(chart, series_b, i * 2);
    }
}



static void
task1(void *args __attribute((unused)))
{
    uint8_t angle = 0;
    lv_tutorial_objects();
    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(200));
        angle += 10;
        lv_chart_set_next(chart, series_a, sinus(angle));
        lv_task_handler();
        gpio_toggle(GPIOA, GPIO7);
    }

}



#define TEST_NULL_DEREF 0
#define TEST_BUS_FAULT 0
#define TEST_USAGE_FAULT 0
#define TEST_MEMMANAG_FAULT 0
#define TEST_DIV0_FAULT 0
#define TEST_OUT_OF_MEMORY 0

int
main(void)
{

    clock_setup();
    systick_setup();
    gpio_setup();
    irq_setup();
    logger_init();
    display_connection_init();
    display_init(&display, displayOrientationHorizontalFlipped);
    lv_init();
    display_lvgl_adapter_init(&display);


#if !TEST_BUS_FAULT
    mpu_setup();
#endif

#if TEST_OUT_OF_MEMORY
    for (int i = 0; true; i++)
    {
        volatile char *example = malloc(2048);

        if (example)
        {
            *example = 10;
        }
        else
        {
            puts(strerror(errno));
        }
    }
#endif


#if TEST_NULL_DEREF
    volatile int* null_ptr = 0;
    volatile int a = *null_ptr;
#endif

#if TEST_BUS_FAULT
    //trigger precise bus fault by accessing memory behind the end of the RAM
    extern int _stack;
    int* behind_the_end = &_stack + 4;
    volatile int a = *behind_the_end;
#endif

#if TEST_USAGE_FAULT
    __asm volatile("udf.w");
#endif

#if TEST_MEMMANAG_FAULT
    void (*exec_never_region)(void) = 0x40000000;
    exec_never_region();
#endif

#if TEST_DIV0_FAULT
    int a = 0;
    volatile int b = 10/a;
#endif

    xTaskCreate(task1, "LED", 256, NULL, configMAX_PRIORITIES - 1, NULL);
    vTaskStartScheduler();

    for (;;);
    return 0;
}
