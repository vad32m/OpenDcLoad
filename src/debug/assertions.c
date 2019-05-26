#include <stddef.h>

#include "assertions.h"
#include "logging.h"


#ifdef DEBUG


bool
_assert_is_ptr_valid(void* ptr)
{
    //TODO: additional checks
    return (ptr != NULL);
}

void
_assert_die(const char* file, const char* line)
{
    int32_t log_msg_handle = logger_start_message();

    if (log_msg_handle >= 0) {
        logger_printf(log_msg_handle, "Assertion failed: %s line %s", file, line);
        logger_end_message(log_msg_handle);
        logger_flush(log_msg_handle);
    }

    __asm volatile("BKPT #0");
}

#endif //DEBUG
