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

CFILES = src/main.c \
	src/fault_handlers/fault_handler.c \
	src/platform/core_setup.c \
	src/platform/syscalls.c

INCLUDES = $(FREERTOS_INCLUDES) \
    -I./src/fault_handlers/inc/ \
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