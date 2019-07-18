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

#include "lvgl.h"

lv_disp_t* lvgl_disp;
lv_disp_drv_t lvgl_drv;
lv_disp_buf_t lvgl_buf;

static lv_color_t buf_1[480 * 20];
static lv_color_t buf_2[480 * 20];

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
lv_display_flush_cb(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p)
{
    struct display_area disp_area;
    disp_area.xStart = area->x1;
    disp_area.xEnd = area->x2 + 1;
    disp_area.yStart = area->y1;
    disp_area.yEnd = area->y2 + 1;
    display_write_image(&display, &disp_area, color_p);
}

static void
lv_display_cb_glue(void)
{
    lv_disp_flush_ready(&lvgl_drv);
}

static void btn_event_cb(lv_obj_t * btn, lv_event_t event);
static void ddlist_event_cb(lv_obj_t * ddlist, lv_event_t event);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_obj_t * slider;

/**
 * Create some objects
 */
void lv_tutorial_objects(void)
{
    static int i = 0;
    /********************
     * CREATE A SCREEN
     *******************/
    /* Create a new screen and load it
     * Screen can be created from any type object type
     * Now a Page is used which is an objects with scrollable content*/
    lv_obj_t * scr = lv_page_create(NULL, NULL);
    lv_disp_load_scr(scr);

    /****************
     * ADD A TITLE
     ****************/
    lv_obj_t * label = lv_label_create(scr, NULL); /*First parameters (scr) is the parent*/
    lv_label_set_text(label, "Object usage demo");  /*Set the text*/
    lv_obj_set_x(label, 50);                        /*Set the x coordinate*/

    /****************
     * ADD A SLIDER
     ****************/
    slider = lv_slider_create(scr, NULL);                            /*Create a slider*/
    lv_obj_set_size(slider, lv_obj_get_width(scr)  / 3, LV_DPI / 3);            /*Set the size*/
    lv_obj_align(slider, label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);                /*Align below the first button*/
    lv_slider_set_value(slider, 30, false);                                            /*Set the current value*/

    /***********************
     * ADD A DROP DOWN LIST
     ************************/
    lv_obj_t * ddlist = lv_ddlist_create(scr, NULL);                     /*Create a drop down list*/
    lv_obj_align(ddlist, slider, LV_ALIGN_OUT_RIGHT_TOP, 50, 0);         /*Align next to the slider*/
    lv_obj_set_top(ddlist, true);                                        /*Enable to be on the top when clicked*/
    lv_ddlist_set_options(ddlist, "None\nLittle\nHalf\nA lot\nAll");     /*Set the options*/
    lv_obj_set_event_cb(ddlist, ddlist_event_cb);                        /*Set function to call on new option is chosen*/

    /****************
     * CREATE A CHART
     ****************/
    lv_obj_t * chart = lv_chart_create(scr, NULL);                         /*Create the chart*/
    lv_obj_set_size(chart, lv_obj_get_width(scr) / 2, lv_obj_get_width(scr) / 4);   /*Set the size*/
    lv_obj_align(chart, slider, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 50);                   /*Align below the slider*/
    lv_chart_set_series_width(chart, 3);                                            /*Set the line width*/

    /*Add a RED data series and set some points*/
    lv_chart_series_t * dl1 = lv_chart_add_series(chart, LV_COLOR_RED);
    lv_chart_set_next(chart, dl1, i++);
    lv_chart_set_next(chart, dl1, i++);
    lv_chart_set_next(chart, dl1, i++);
    lv_chart_set_next(chart, dl1, i++);

    /*Add a BLUE data series and set some points*/
    lv_chart_series_t * dl2 = lv_chart_add_series(chart, lv_color_make(0x40, 0x70, 0xC0));
    lv_chart_set_next(chart, dl2, i++);
    lv_chart_set_next(chart, dl2, i++);
    lv_chart_set_next(chart, dl2, i++);
    lv_chart_set_next(chart, dl2, i++);
    lv_chart_set_next(chart, dl2, i++);
    lv_chart_set_next(chart, dl2, i++);
    if (i > 25) {
        i = 0;
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Called when a button is released
 * @param btn pointer to the released button
 * @param event the triggering event
 * @return LV_RES_OK because the object is not deleted in this function
 */
static void btn_event_cb(lv_obj_t * btn, lv_event_t event)
{
    if(event == LV_EVENT_RELEASED) {
        /*Increase the button width*/
        lv_coord_t width = lv_obj_get_width(btn);
        lv_obj_set_width(btn, width + 20);
    }
}

/**
 * Called when a new option is chosen in the drop down list
 * @param ddlist pointer to the drop down list
 * @param event the triggering event
 * @return LV_RES_OK because the object is not deleted in this function
 */
static  void ddlist_event_cb(lv_obj_t * ddlist, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) {
        uint16_t opt = lv_ddlist_get_selected(ddlist);            /*Get the id of selected option*/

        lv_slider_set_value(slider, (opt * 100) / 4, true);       /*Modify the slider value according to the selection*/
    }

}

static void
task1(void *args __attribute((unused)))
{
    uint16_t color = 1;
    display_init(&display, displayOrientationHorizontalFlipped);
	display_clear(&display, 0x0220);

    lv_disp_buf_init(&lvgl_buf, buf_1, buf_2, 480 * 20);
    lv_disp_drv_init(&lvgl_drv);

    lvgl_drv.buffer = &lvgl_buf;
    lvgl_drv.flush_cb = &lv_display_flush_cb;
    lvgl_disp = lv_disp_drv_register(&lvgl_drv);

    display_set_callback(&display, &lv_display_cb_glue);

    for (;;) {
        lv_tutorial_objects();
        vTaskDelay(pdMS_TO_TICKS(600));
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

    lv_init();
    xTaskCreate(task1, "LED", 256, NULL, configMAX_PRIORITIES - 1, NULL);
    vTaskStartScheduler();

    for (;;);
    return 0;
}
