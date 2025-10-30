#ifndef LOG_H
#define LOG_H

/**
 * @file
 * @author  Milo Spadacini
**/

#define SUB 0
#define VRB 1
#define DBG 2
#define INF 3
#define WRN 4
#define ERR 5

#ifdef ENABLE_DEBUG
#define DEBUG_PRINT(...) p2pLog(__VA_ARGS__ )

#if !defined(OS_ANDROID) && !defined(OS_iOS)
#include <sys/prctl.h>
#define SET_THREAD_NAME(x) prctl(PR_SET_NAME, (x), 0, 0, 0);
#else
#define SET_THREAD_NAME(x) {}
#endif
#else
#define DEBUG_PRINT(...)   { }
#define SET_THREAD_NAME(x) { }
#endif

void p2pLog(unsigned int level, const char *format, ...);

#endif // LOG_H
