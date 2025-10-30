/**
 *
 * @file log.h
 * Copyright (C) 2013 Comelit Group SpA
 *
 */

#ifndef LOG_H
#define LOG_H

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <pthread.h>
#ifndef __WIN32__
#include <net/if.h>
#endif
#include <sys/time.h>
#include <unistd.h>

#include <functional>
#include <map>
#include <string>
#include <vector>

#include <cmpot/common.h>

#define CMPOT_TRACE_ERR     fprintf(stderr, "\e[00;31m[TRACE] %s:%s:%d: here!\e[00m\n\n", __FILE__, __FUNCTION__, __LINE__);
#define CMPOT_TRACE         printf("\e[00;34m[TRACE] %s:%s:%d: here!\e[00m\n", __FILE__, __FUNCTION__, __LINE__);

namespace Comelit {
namespace MPot    {

using std::map;
using std::string;
using std::vector;

enum EventType
{
	SUB, VRB, DBG, INF, WRN, ERR
};

typedef std::function<void (EventType level, const char *emitter, const char *params, const char *datetime, const char *line)>
OnNewLogEvent;

namespace Log {
/**
 * @brief setMinEventType set the minimum event type to be printed (default DBG and above)
 * @param[in] minLevel the minimum event type ("level") to be printed
 */
void setMinEventType(EventType minLevel);

/**
 * @brief append printf wrapper for debug (console) messages
 * @param[in] type event type
 * @param[in] emitter identifies who (e.g., the function/subsystem) that's emitting the message
 * @param[in] format printf-style string format
 * @param[in] ... printf-style variadic number of parameters
 *
 */
void append(EventType type, const char *emitter, const char *format, ...);

/**
 * @brief setInMemoryLogging enable/disable the in memory logging
 * @param[in] enabled enable/disable in-memory logging
 *
 * @note: by default in-memory logging is disabled
 */
void setInMemoryLogging(bool enabled);

/**
 * @brief setAppendLogToFile enable logging on file (append mode)
 * @param[in] enabled enable/disable logging on file
 * @param[in] filePath path to file to write log to
 */
void setAppendLogToFile(bool enabled, const string &filePath);

/**
 * @brief addSubscriber add a subscriber that will be informed on any new log entry added
 * @param onNewEvent callback to be invoked to inform the subscriber
 * @param id subscriber identifier, used for removal purposes
 * @return true if the operation succeeded; fa
 */
bool addSubscriber(OnNewLogEvent &&onNewEvent, int &id);

/**
 * @brief removeSubscriber remove a registered subscriber
 * @param id subscriber id used to dinf and remove the subscriber
 * @return true if removal was successfully completed; false otherwise
 */
bool removeSubscriber(int id);

/**
 * @brief clearInMemoryLogging clear events log
 */
void clearInMemoryLogging();

/**
 * @brief getInMemoryLog get the current in-memory-logged events (if any)
 * @return the in-memory-logged events (if any)
 */
const vector<string> &getInMemoryLog();

/**
 * @brief connectSysLog connect log to system log (unix only)
 * @param appName syslog tag
 * @param facility syslog channel
 */
void connectSysLog(string appName, int facility = 0);

/**
 * @brief disconnectSysLog disconnect previously connected syslog
 */
void disconnectSysLog();
}


} // MPot    NS
} // Comelit NS

#endif // LOG_H
