
#include "display_lvgl_adapter.h"
#include "debug/assertions.h"
#include "lvgl.h"

#define LVGL_FB_SIZE (480 * 20)

static lv_disp_t* lvgl_disp;
static lv_disp_drv_t lvgl_drv;
static lv_disp_buf_t lvgl_buf;

static struct display_driver* display_drv;

static lv_color_t buf_1[LVGL_FB_SIZE];
static lv_color_t buf_2[LVGL_FB_SIZE];


static void
lv_display_flush_cb(lv_disp_drv_t* lv_disp_drv, const lv_area_t* area, lv_color_t* color_p)
{
    struct display_area disp_area;
    disp_area.xStart = area->x1;
    disp_area.xEnd = area->x2 + 1;
    disp_area.yStart = area->y1;
    disp_area.yEnd = area->y2 + 1;
    display_write_image(display_drv, &disp_area, color_p);
}

static void
lv_display_cb_glue(void)
{
    lv_disp_flush_ready(&lvgl_drv);
}

void
display_lvgl_adapter_init(struct display_driver* display)
{
    //ASSERT_PTR_VALID(display);

    lv_disp_buf_init(&lvgl_buf, buf_1, buf_2, LVGL_FB_SIZE);
    lv_disp_drv_init(&lvgl_drv);

    lvgl_drv.buffer = &lvgl_buf;
    lvgl_drv.flush_cb = &lv_display_flush_cb;
    lvgl_disp = lv_disp_drv_register(&lvgl_drv);

    display_drv = display;

    display_set_callback(display, &lv_display_cb_glue);
}
