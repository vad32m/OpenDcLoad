#ifndef SYS_CONFIG__
#define SYS_CONFIG__

#define SYSCONF_HEAP_SIZE ((size_t) (75 * 1024))

#define SYSCONF_CPU_CLOCK (168000000)

#define SYSCONF_RTOS_AWARE_INTERRUPT_MAX_PRIORITY (0x5)
#define SYSCONF_LOWEST_INTERRUPT_PRIO (0xf)

#define SYSCONF_FAULT_INTERRUPT_PRIORITY (0x1)
#define SYSCONF_LOGGER_INTERRUPT_PRIORITY (0xf)

#endif //SYS_CONFIG__
