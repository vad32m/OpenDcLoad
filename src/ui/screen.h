#ifndef UI_SCREEN_H_
#define UI_SCREEN_H_

typedef struct ui_screen_struct ui_screen_t;

typedef enum ui_screen_enum {
    UI_SCREEN_MODE_SELECT,
    UI_SCREEN_PLOT,
    UI_SCREEN_VALUES,
    UI_SCREEN_PARAMETER_SETUP,

    UI_SCREEN_MAX_MODES
} ui_screen;

ui_screen_t* screen_get_instance(void);

int32_t screen_set_active(ui_screen_t* screen, ui_screen which_screen);

#endif /* UI_SCREEN_H_ */
