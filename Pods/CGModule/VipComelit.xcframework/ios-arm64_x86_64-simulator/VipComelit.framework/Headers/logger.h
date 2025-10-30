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

#ifndef LIBCLOGGER_H
#define LIBCLOGGER_H

#include <unistd.h>
#include <functional>
#include <string>

#include <comelitutils/logger.h>

using namespace Comelit;

namespace LibComelit {

enum LibFlags
{
  LIB_LIBC = 1 << 0,
  LIB_CMPS = 1 << 1,
  LIB_ERM  = 1 << 2,
  LIB_FMPG = 1 << 3,
  LIB_P2P  = 1 << 4,
  LIB_ALL  = 0xFFFFFFFF, //32 libraries at most
};

namespace SetLogger {

/**
 * Initialize libraries log callbacks
 */
void setLibsLog();

/**
 * Set the minimum event type to be printed (default LOG_INF and above)
 * @param minLevel the minimum event type ("level") to be printed
 * @param libFlags indicates which library should apply the new logging minLevel (use LibFlags enum)
 */
void setMinEventType(LogTypeLevel minLevel, unsigned int libFlags = LIB_LIBC);


} // End Logger
} // End LibComelit
#endif // LIBCLOGGER_H
