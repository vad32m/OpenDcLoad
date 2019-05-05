#ifndef FAULT_CONFIG_H_
#define FAULT_CONFIG_H_

#include <stdio.h>

#define MEMMANAGE_FAULT_SYMBOL mem_manage_handler

#define HARD_FAULT_SYMBOL hard_fault_handler

#define BUS_FAULT_SYMBOL bus_fault_handler

#define USAGE_FAULT_SYMBOL usage_fault_handler

#define FAULT_PRINTLN(VAR)           puts(VAR)
#define FAULT_PRINT(VAR)             printf(VAR)
#define FAULT_PRINT_HEX(VAR)         printf("0x%08lX", VAR)
#define FAULT_NEWLINE()              puts(" ")

#define FAULT_BREAKPOINT
#define FAULT_STOP

#endif /* FAULT_CONFIG_H_ */