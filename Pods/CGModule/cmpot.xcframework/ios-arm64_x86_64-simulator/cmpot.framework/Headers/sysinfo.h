/**
 *
 * @file sysinfo.h
 * Copyright (C) 2017 Comelit Group SpA
 *
 */

#ifndef CMPOT_SYSINFO_H
#define CMPOT_SYSINFO_H

#include <string>

namespace Comelit {
namespace MPot    {

using std::string;

/**
 * @brief The MemStat struct stores info about current memory
 *        used by this program in a given moment
 */
struct MemStat
{
	string time; ///< current time (string representation)
	long   rss;  ///< RSS memory used by this program
	long   shm;  ///< shared memory used by this program
	long   pvt;  ///< private memory used by this program
	long   vsize;
	unsigned long  utimetks;
	long   cutimetks;
	unsigned long  stimetks;
	long   cstimetks;
	unsigned long  cputotaltime;
};

namespace SysInfo {
string getCurrTime();

MemStat getProcessUsage();

void getCPUUsagePct(const MemStat &currUsage,
                    const MemStat &lastUsage,
                    double &ucpuUsage, double &scpuUsage);

string getUptime();

/**
 * @brief getMemoryUsage
 * return a string representation of system memory usage
 * @return memory usage string
 */
string getMemoryUsage();

/**
 * @brief getTotalFreeMem
 * Retrieve system total memory and free memory, intended
 * as free + buffers + cached memory
 * @param[out] totalMem total system memory in Mb
 * @param[out] totalFreeMem total system free memory in Mb
 * @return true if memory usage is retrieved with success
 */
bool getTotalFreeMem(unsigned long &totalMem, unsigned long &totalFreeMem);

/**
 * @brief getCachedMem
 * retrieve the amount of system cached memory
 * @return system cached memory in kB
 */
unsigned long getCachedMem();

/**
 * @brief getMemoryUsagePct
 * get the percentage of system used memory
 * @return used memory percentage
 */
double getMemoryUsagePct();

string getSysInfo();
}

} // MPot    NS
} // Comelit NS

#endif // CMPOT_SYSINFO_H
