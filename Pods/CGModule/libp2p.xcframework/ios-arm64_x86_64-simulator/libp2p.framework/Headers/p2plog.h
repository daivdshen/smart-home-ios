#ifndef P2PLOG_H
#define P2PLOG_H

/**
 * @file
 * @author  Fabio Cornelli
**/
#include <functional>

typedef std::function<void (unsigned int level, const char *params)> P2pLogCbk;

/**
 * set log callback
 * @param cbk
 * @param[in] logLvl [0-5] [SUB-ERR]
 * @return true logLvl in range and callback set
 */
bool setP2pLogCbk(P2pLogCbk cbk, unsigned int logLvl = 5);

/**
 * set minimun log level
 * @param[in] logLvl [0-5] [SUB-ERR]
 * @return true logLvl in range
 */
bool setP2pLogMin(unsigned int logLvl);

#endif // P2PLOG_H
