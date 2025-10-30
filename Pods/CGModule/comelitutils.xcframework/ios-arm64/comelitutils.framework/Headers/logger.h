/**
 *
 * @file liblogger.h
 *
 * LibComelitLogger class. Object for the management of log. Allow set a external callback to send log messages.
 *
 * Usage example to set callback from application (using lambda):
 *
 *  Logger::setCallback(pointerToCallerObject, [] (void* obj, const std::string& pString) { // What you want do with log string });
 *  Logger::setMinEventType(wantedMinimumLevel);
 *  (or Logger::setMinEventType(wantedMinimumLevel, LIB_LIBC | LIB_FMPG) to set granularity of logs for each library)
 *
 * @author	Fabio Bigoni.
 */

#ifndef LIBCOMUTILSLOGGER_H
#define LIBCOMUTILSLOGGER_H

#include <unistd.h>
#include <functional>
#include <string>

namespace Comelit {

enum LogTypeLevel
{
	LOG_SUB, LOG_VRB, LOG_DBG, LOG_INF, LOG_WRN, LOG_ERR
};

namespace Logger {

/**
 * Logger callback structure definition.
 * The first parameter is the pointer to object caller (set by setCallback)
 * The second parameter is the log level
 * The third parameter is the log string
 */
typedef std::function<void (void *, unsigned int, const std::string &)> CbkFunction;

/**
 * Append string to log. Inside callback called (if set) or printf.
 * @param level the level of the log (The log is discarded if the type is less than the current minimum level)
 * @param pString the message to log (strings with optional format specifiers)
 */
void appendCheckTypeLevel(LogTypeLevel level, const char *pString, ...);

/**
 * Append string to log. Inside callback called (if set) or printf
 * @param level the level of the log
 * @param pString the message to log (strings with optional format specifiers)
 */
void append(LogTypeLevel level, const char *pString);

/**
 * Function to set logger callback
 * @param cb callback function
 * @param obj callback object
 */
void setCallback(void *obj, CbkFunction &&cb);

/**
 * Function to get the string for log level given as parameter
 * @param level the type to get string
 * @return match string for type level given
 */
std::string getLevelStr(LogTypeLevel level);

/**
 * Function to set min level of log to print
 * @param minLevel[in]: new min level
 */
void setMinPrintedEventType(LogTypeLevel minLevel);


} // End Logger
} // End LibComelit
#endif // LIBCOMUTILSLOGGER_H
