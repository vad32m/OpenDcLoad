BUILD_DIR = bin
ARCH_FLAGS = -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
V = 1

.DEFAULT_GOAL = all

FREERTOS_DEFS = -DDEBUG
FREERTOS_CONFIG_LOC := ./src/
FREERTOS_DIR = ./rtos/FreeRTOS
FREERTOS_OBJDIR = $(BUILD_DIR)/rtosbin
FREERTOS_ARCH_FLAGS = $(ARCH_FLAGS)

include ./rtos/Makefile

CFILES = src/main.c
INCLUDES = $(FREERTOS_INCLUDES)

OPENCM3_DIR = ./libopencm3/
PROJECT = electronic_load
LDSCRIPT = ./tools/stm32f407.ld
OPENCM3_LIB = opencm3_stm32f4
OPENCM3_DEFS = -DSTM32F4
LIBDEPS = rtos_lib

include rules.mk
