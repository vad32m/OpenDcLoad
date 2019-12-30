BUILD_DIR = bin
ARCH_FLAGS = -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
V = 1

.DEFAULT_GOAL = debug

debug: FREERTOS_DEFS = -DDEBUG
debug: FREERTOS_OPT = -Og

release: FREERTOS_OPT = -Os

FREERTOS_CONFIG_LOC := ./src/
FREERTOS_DIR = ./rtos/FreeRTOS
FREERTOS_OBJDIR = $(BUILD_DIR)/rtosbin
FREERTOS_ARCH_FLAGS = $(ARCH_FLAGS)

include ./rtos/Makefile

LVGL_DIR = ./src/ui

include ./src/ui/lvgl/lvgl.mk

CFILES += $(CSRCS)
CFILES += src/main.c \
	src/fault_handlers/fault_handler.c \
	src/platform/core_setup.c \
	src/platform/syscalls.c \
	src/platform/display_connection.c \
	src/platform/fp_controls_driver.c \
	src/debug/logging.c \
	src/debug/assertions.c \
	src/driver/ili9486/display_driver.c \
	src/ui/display_lvgl_adapter.c \
	src/ui/views/lvgl_basic_view.c \
	src/ui/views/values_view.c \
	src/ui/views/plot_view.c \
	src/ui/views/param_setup_view.c \
	src/ui/views/mode_select_view.c \
	src/ui/controllers/basic_controller.c \
	src/ui/controllers/mode_select_controller.c \
	src/ui/controllers/param_setup_controller.c \
	src/ui/controllers/plot_controller.c \
	src/ui/controllers/values_controller.c \
	src/ui/user_interaction_process.c \
	src/ui/screen.c \
	src/app/work_mode_facade.c \
	src/app/wm_constant_current.c \
	src/app/wm_constant_power.c \
	src/app/wm_constant_resistance.c \
	src/app/wm_constant_voltage.c \
	src/app/work_mode.c

INCLUDES = $(FREERTOS_INCLUDES) \
    -I./src/fault_handlers/inc/ \
	-I./src/ui/lvgl/ \
	-I./src/

OPENCM3_DIR = ./libopencm3/
PROJECT = electronic_load
LDSCRIPT = ./tools/stm32f407.ld
OPENCM3_LIB = opencm3_stm32f4
LIBDEPS = rtos_lib
CSTD = --std=c99
OPENCM3_DEFS = -DSTM32F4

debug: OPENCM3_DEFS += -DDEBUG
debug: OPT = -Og

debug: binary
release: binary

include rules.mk

.PHONY: debug release