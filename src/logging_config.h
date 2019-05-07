#ifndef __LOGGER_CONF_H
#define __LOGGER_CONF_H

/**
 * @file
 * @brief Default configurations for logger, may be overriden by file-scope logger config
 */

/**
* @addtogroup logger
* @{
*/
/**
* @brief Global level of logging
*
* Ranging from LL_NO_LOGS - most quiet to LL_DEBUG - most verbose
* May be overriden for specific source file with #define LOG_LEVEL
* directive before #include "logger.h"
*/
#define LOG_GLOBAL_LEVEL LL_ERROR
/**
 * @brief Include line numbers into log output or not
 *
 * May be overriden for specific source file with #define LOG_DISPLAY_LINE
 * directive before #include "logger.h"
 */
#define LOG_GLOBAL_DISPLAY_LINE 1
/**
 * @brief Include file names into log output or not
 *
 * May be overriden for specific source file with #define LOG_DISPLAY_FILE
 * directive before #include "logger.h"
 */
#define LOG_GLOBAL_DISPLAY_FILE 1

/**
 * @brief Global verbosity level of trace output
 *
 * May be overriden for specific source file with #define LOG_DISPLAY_LEVEL
 * directive before #include "logger.h"
 */
#define LOG_GLOBAL_DISPLAY_LEVEL 1

/**
 *@}
 */

#endif
