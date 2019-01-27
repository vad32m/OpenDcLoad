OPENCM3_DIR = ./libopencm3/
PROJECT = electronic_load
CFILES = src/main.c
LDSCRIPT = ./tools/stm32f407.ld
OPENCM3_LIB = opencm3_stm32f4
OPENCM3_DEFS = -DSTM32F4
ARCH_FLAGS = -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16

BUILD_DIR = bin

include rules.mk
