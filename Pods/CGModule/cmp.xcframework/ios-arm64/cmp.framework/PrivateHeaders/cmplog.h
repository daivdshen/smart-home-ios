#ifndef CMPLOG_H
#define CMPLOG_H

#include <functional>

typedef std::function<void (unsigned int level, const char *params)> CmpLogCbk;

/**
 * set log callback
 * @param cbk
 * @param[in] logLvl [0-5] [SUB-ERR]
 * @return true logLvl in range and callback set
 */
bool setCmpLogCbk(CmpLogCbk cbk, unsigned int logLvl = 5);

/**
 * set minimun log level
 * @param[in] logLvl [0-5] [SUB-ERR]
 * @return true logLvl in range
 */
bool setCmpLogMin(unsigned int logLvl);

#endif // CMPLOG_H
