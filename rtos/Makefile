#******************************************************************************
# This makefile requires the following to be defined before inclusion..
# FREERTOS_CONFIG - catalog, where freertos config file is placed
# FREERTOS_ARCH_FLAGS - eg, -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
#    (ie, the full set of cpu arch flags, _none_ are defined in this file)
# FREERTOS_OBJDIR - directory, where object files should be put
# Optional 
# FREERTOS_DEFS - any preprocessor data
# FREERTOS_OPT - full -O flag, defaults to -Os
# CSTD - defaults -std=c99

FREERTOS_OBJDIR = ./bin
FREERTOS_ARCH_FLAGS = -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
FREERTOS_DEFS := -DDEBUG
FREERTOS_CONFIG_LOC := ../src/


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
FREERTOS_HEADERS_PATH = FreeRTOS/Source/include FreeRTOS/Source/portable/GCC/ARM_CM4F $(FREERTOS_CONFIG_LOC)
FREERTOS_INCLUDES += $(patsubst %,-I%, . $(FREERTOS_HEADERS_PATH) )

FREERTOS_CFILES = \
	$(shell find FreeRTOS/Source -maxdepth 1 -type f -name "*.c") \
	$(shell find FreeRTOS/Source/portable/GCC/ARM_CM4F -maxdepth 1 -type f -name "*.c") \
	FreeRTOS/Source/portable/MemMang/heap_4.c


FREERTOS_OBJS = $(FREERTOS_CFILES:%.c=$(FREERTOS_OBJDIR)/%.o)

TGT_CPPFLAGS += -MD
TGT_CPPFLAGS += -Wall -Wundef
TGT_CPPFLAGS += $(FREERTOS_INCLUDES) $(FREERTOS_DEFS)

TGT_CFLAGS += $(OPT) $(CSTD) -ggdb3
TGT_CFLAGS += $(FREERTOS_ARCH_FLAGS)
TGT_CFLAGS += -fno-common
TGT_CFLAGS += -ffunction-sections -fdata-sections
TGT_CFLAGS += -Wextra -Wshadow -Wno-unused-variable -Wimplicit-function-declaration
TGT_CFLAGS += -Wredundant-decls -Wstrict-prototypes -Wmissing-prototypes

# Bad make, never *ever* try to get a file out of source control by yourself.
# TODO: why do we need this
%: %,v
%: RCS/%,v
%: RCS/%
%: s.%
%: SCCS/s.%


# Specify the target file and the install directory
OUTPUTFILE  = libfreertos.a

# Build libfreertos.a from objs
freertos: $(FREERTOS_OBJS)
	ar ru $(OUTPUTFILE) $(FREERTOS_OBJS)



# Need a special rule to have a bin dir
$(FREERTOS_OBJDIR)/%.o: %.c
	@printf "  CC\t$<\n"
	@mkdir -p $(dir $@)
	$(Q)$(CC) $(TGT_CFLAGS) $(CFLAGS) $(TGT_CPPFLAGS) $(CPPFLAGS) -o $@ -c $<

$(FREERTOS_OBJDIR)/%.o: %.S
	@printf "  AS\t$<\n"
	@mkdir -p $(dir $@)
	$(Q)$(CC) $(TGT_ASFLAGS) $(ASFLAGS) $(TGT_CPPFLAGS) $(CPPFLAGS) -o $@ -c $<

clean:
	rm -rf $(FREERTOS_OBJDIR)
	rm libfreertos.a

.PHONY: freertos clean
