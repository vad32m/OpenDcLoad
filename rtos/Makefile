#******************************************************************************
# This makefile requires the following to be defined before inclusion..
# FREERTOS_CONFIG - catalog, where freertos config file is placed
# FREERTOS_ARCH_FLAGS - eg, -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
#    (ie, the full set of cpu arch flags, _none_ are defined in this file)
# FREERTOS_DIR - relative path from makefile, that includes this makefile
# to direcory with FreeRTOS folder 
# Optional 
# FREERTOS_LIBDIR - directory for installation of output library ()
# FREERTOS_OUTPUTFILE - name for output library (libfreertos.a per default)
# FREERTOS_DEFS - any preprocessor data
# FREERTOS_OPT - full -O flag, defaults to -Os
# CSTD - defaults -std=c99


# Specify the target file and the install directory
FREERTOS_OUTPUTFILE  ?= libfreertos.a
FREERTOS_LIBDIR ?= $(FREERTOS_DIR)/lib/

FREERTOS_OPT ?= -Os
CSTD ?= -std=c99

# Be silent per default, but 'make V=1' will show all compiler calls.
# If you're insane, V=99 will print out all sorts of things.
V?=0
ifeq ($(V),0)
Q	:= @
NULL	:= 2>/dev/null
endif

# Tool paths.
PREFIX	?= arm-none-eabi-
CC	= $(PREFIX)gcc
LD	= $(PREFIX)gcc
OBJCOPY	= $(PREFIX)objcopy
OBJDUMP	= $(PREFIX)objdump

# Inclusion of library header files
FREERTOS_HEADERS_PATH = /Source/include /Source/portable/GCC/ARM_CM4F
FREERTOS_INCLUDES = $(patsubst %,$(FREERTOS_DIR)/%, $(FREERTOS_HEADERS_PATH)) $(FREERTOS_CONFIG_LOC)
FREERTOS_INCLUDES := $(patsubst %,-I%, $(FREERTOS_INCLUDES))

FREERTOS_CFILES = \
	$(shell find $(FREERTOS_DIR)/Source -maxdepth 1 -type f -name "*.c") \
	$(shell find $(FREERTOS_DIR)/Source/portable/GCC/ARM_CM4F -maxdepth 1 -type f -name "*.c") \
	$(FREERTOS_DIR)/Source/portable/MemMang/heap_4.c


FREERTOS_OBJS = $(FREERTOS_CFILES:%.c=$(FREERTOS_OBJDIR)/%.o)

FREERTOS_CPPFLAGS += -MD
FREERTOS_CPPFLAGS += -Wall -Wundef
FREERTOS_CPPFLAGS += $(FREERTOS_INCLUDES) $(FREERTOS_DEFS)

FREERTOS_CFLAGS += $(OPT) $(CSTD) -ggdb3
FREERTOS_CFLAGS += $(FREERTOS_ARCH_FLAGS)
FREERTOS_CFLAGS += -fno-common
FREERTOS_CFLAGS += -ffunction-sections -fdata-sections
FREERTOS_CFLAGS += -Wextra -Wshadow -Wno-unused-variable -Wimplicit-function-declaration
FREERTOS_CFLAGS += -Wredundant-decls -Wstrict-prototypes -Wmissing-prototypes

# Bad make, never *ever* try to get a file out of source control by yourself.
# TODO: why do we need this
%: %,v
%: RCS/%,v
%: RCS/%
%: s.%
%: SCCS/s.%


# Build libfreertos.a from objs
rtos_lib: $(FREERTOS_OBJS)
	$(Q)mkdir -p $(FREERTOS_LIBDIR)
	$(Q)ar ru $(FREERTOS_LIBDIR)/$(FREERTOS_OUTPUTFILE) $(FREERTOS_OBJS)


# Need a special rule to have a bin dir
$(FREERTOS_OBJDIR)/%.o: %.c $(FREERTOS_CONFIG_LOC)/FreeRTOSConfig.h
	$(Q)printf "  CC\t$<\n"
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CC) $(FREERTOS_CFLAGS) $(CFLAGS) $(FREERTOS_CPPFLAGS) $(CPPFLAGS) -o $@ -c $<

rtos_clean:
	$(Q)rm -rf $(FREERTOS_OBJDIR)
	$(Q)rm $(FREERTOS_LIBDIR)/$(FREERTOS_OUTPUTFILE)

.PHONY: rtos_lib rtos_clean
