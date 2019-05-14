/**
 * @file
 * @brief macro definitions for config of the logger
 * and interface function definitions
*/

#ifndef __LOGGING_H
#define __LOGGING_H

#include <stdint.h>
#include "logging_config.h"

/**
 * @addtogroup logger
 * Highly configurable module for writing debug output to the ITM trace
 * @{
 */

/**
 * @brief Defines maximum number of characters(including null terminator)
 * that may be put with debug in single line
 */
#define LOGGER_MESSAGE_MAX_LEN 128
/**
 * @brief Defines maximum number of messages, stored in loggers buffer
 */
#define LOGGER_BUFFER_MESSAGE_CAPACITY 5

/**
 * @brief No logs will be printed
 */
#define LL_NO_LOGS 0
/**
 * @brief Only error messages will be printed
 */
#define LL_ERROR   1
/**
 * @brief Error and warning messages will be printed
 */
#define LL_WARNING 2
/**
 * @brief Error, info and warning messages will be printed
 */
#define LL_INFO    3
/**
 * @brief Most verbose log output - everything will be printed
 */
#define LL_DEBUG   4

/**
 * @cond INTERNAL
 */
#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_GLOBAL_LEVEL
#endif

#ifndef LOG_DISPLAY_LINE
#define LOG_DISPLAY_LINE LOG_GLOBAL_DISPLAY_LINE
#endif

#ifndef LOG_DISPLAY_FILE
#define LOG_DISPLAY_FILE LOG_GLOBAL_DISPLAY_FILE
#endif

#ifndef LOG_DISPLAY_LEVEL
#define LOG_DISPLAY_LEVEL LOG_GLOBAL_DISPLAY_LEVEL
#endif

#define STRINGIZE(x) STRINGIZE_SIMPLE(x)
#define STRINGIZE_SIMPLE(x) #x

#if LOG_DISPLAY_LINE != 0
#define LOG_LINE_STR(LIN) STRINGIZE(LIN) " "
#else
#define LOG_LINE_STR(LIN)
#endif

#if LOG_DISPLAY_FILE != 0
#define LOG_FILE_STR(FIL) STRINGIZE(FIL) " "
#else
#define LOG_FILE_STR(FIL)
#endif

#if LOG_DISPLAY_LEVEL != 0
#define LOG_LEVEL_STR(LEVEL) LEVEL " "
#else
#define LOG_LEVEL_STR(LEVEL)
#endif

#define LOG_PRINTF(LEVEL, ...) {\
    int32_t log_handler = logger_start_message();\
    if (log_handler >= 0) {\
        logger_printf(log_handler, LOG_LEVEL_STR(LEVEL) LOG_FILE_STR(__FILE__) LOG_LINE_STR(__LINE__) __VA_ARGS__);\
        logger_end_message(log_handler);\
    }}

#define LOG_BYTE_ARRAY(LEVEL, POINTER, SIZE) {\
    int32_t log_handler = logger_start_message();\
    if (log_handler >= 0) {\
        logger_put_string(log_handler, LOG_LEVEL_STR(LEVEL) LOG_FILE_STR(__FILE__) LOG_LINE_STR(__LINE__));\
        logger_print_byte_array(log_handler, POINTER, SIZE);\
        logger_end_message(log_handler);\
    }}
/**
 * @endcond
 */

/**
* @def LOG_DBG(...)
* @brief Log debug-level ouput, used just to trace program execution path
*/
#if LOG_LEVEL < LL_DEBUG
#define LOG_DBG(...)
#define LOG_DBG_BYTES(POINTER, SIZE)
#else
#define LOG_DBG(...) LOG_PRINTF("Debug", __VA_ARGS__)
#define LOG_DBG_BYTES(POINTER, SIZE) LOG_BYTE_ARRAY("Debug", POINTER, SIZE)
#endif
/**
 * @def LOG_INF(...)
 * @brief Log program execution information, should be used during normal execution path
 */
#if LOG_LEVEL < LL_INFO
#define LOG_INF(...)
#define LOG_INF_BYTES(POINTER, SIZE)
#else
#define LOG_INF(...) LOG_PRINTF("Info", __VA_ARGS__)
#define LOG_INF_BYTES(POINTER, SIZE) LOG_BYTE_ARRAY("Info", POINTER, SIZE)
#endif

/**
 * @def LOG_WRN(...)
 * @brief Log errors, which are easily handled and don't generally affect program functions
 */
#if LOG_LEVEL < LL_WARNING
#define LOG_WRN(...)
#define LOG_WRN_BYTES(POINTER, SIZE)
#else
#define LOG_WRN(...) LOG_PRINTF("Warning", __VA_ARGS__)
#define LOG_WRN_BYTES(POINTER, SIZE) LOG_BYTE_ARRAY("Warning", POINTER, SIZE)
#endif

/**
 * @def LOG_ERR(...)
 * @brief Log serious errors, that cause serious changes to program behavior
 */
#if LOG_LEVEL < LL_ERROR
#define LOG_ERR(...)
#define LOG_ERR_BYTES(POINTER, SIZE)
#else
#define LOG_ERR(...) LOG_PRINTF("Error", __VA_ARGS__)
#define LOG_ERR_BYTES(POINTER, SIZE) LOG_BYTE_ARRAY("Error", POINTER, SIZE)
#endif

/**
 * @brief Performs necessary initialization and creates logger thread
 */
void logger_init(void);
/**
 * @}
 */

int32_t logger_start_message(void);
void logger_put_string(int32_t message_handler, const char *string);
void logger_printf(int32_t message_handler, const char *format, ...);
void logger_put_decimal(int32_t message_handler, uint32_t number);
void logger_end_message(int32_t message_handler);
void logger_print_byte_array(int32_t message_handler, void *memory, uint32_t _array_size);


#endif

