#ifndef EXPONENTIALBACKOFF_H
#define EXPONENTIALBACKOFF_H

#include "retriggerable_timer.h"

/**********************************************************
 *
 *	1) call start() to enable exponential callback timer
 *
 *	2) Check if time is elapsed with
 *
 *		if (isBackoffElapsed())
 *		{
 *			...
 *			call restartBackoff()
 *		}
 *
 *		or
 *
 *		if (iBackoffElapsedRestart)
 *		{
 *			...
 *			No need to call restartBackoff()
 *		}
 *
 *
 *	3) On succes event
 *
 *		call resetBackoff()
 *
 *	3) On Error event
 *
 *		call incBackoff()
 *
 *
 *	4) call stop() to disable exponential callback timer
 *
 ****************************************************************/

namespace Comelit {
namespace Utils   {

#define EB_DEF_START_TIME 100		//100ms
#define EB_DEF_MAXTIME 1000 * 60 * 10	//10 minutes
#define EB_EXP_INC 2			//inc time * 2


class TimeThreadTime : public RetriggerableTimer
{
public:
	TimeThreadTime(RetriggerableTimer::timeoutCbk cbk);
	~TimeThreadTime();
};

class RestoreThreadTime: public RetriggerableTimer
{
public:
	RestoreThreadTime(RetriggerableTimer::timeoutCbk cbk);
	~RestoreThreadTime();
	void elapsedTime(void) override;
	int getLastTimeout(void);
	void setLasTimeout(int time);
	bool isResetTimeout(void);
private:
	int lastTimeout;		//last backoff time, max back off time reached
	bool resetTimeout;
};

class ExponentialBackoff
{
public:
	struct Timing
	{
		int startTime;
		int maxRestoreTime;
		int maxTime;
	};
	/**
	 * @brief The timeState enum
	 */
	enum timeState
	{
		TIME_IDLE,
		TIME_ACTIVE,
		TIME_END
	};

	/**
	 * @brief ExponentialBackoff
	 *
	 * @param timeElapsedCb
	 */
	ExponentialBackoff(RetriggerableTimer::timeoutCbk timeElapsedCb = nullptr,
	                   Timing time = {EB_DEF_START_TIME, EB_DEF_MAXTIME, EB_DEF_MAXTIME},
	                   int firstjumpTime = EB_DEF_START_TIME);
	~ExponentialBackoff();

public:
	/**
	 * @brief start start the backoff timer management
	 */
	void start(void);

	/**
	 * @brief stop stop the backoff timer management and restore timer
	 */
	void stop(void);

	/**
	 * @brief setCallback set callback function when time is elapsed
	 *
	 * @param timeElapsedCb callback funtion called when time is elapsed
	 */
	void setCallback(RetriggerableTimer::timeoutCbk timeElapsedCb);

	/**
	 * @brief isBackoffElapsedRestart Check if backoff time is elapsed and restart timer in case of time elapsed
	 *
	 * @return true if time is elapsed, false otherwise
	 */
	bool isBackoffElapsedRestart(void);

	/**
	 * @brief isBackoffElapsed Check if backoff time is elapsed
	 *
	 * @return true if time is elapsed, false otherwise
	 */
	bool isBackoffElapsed(void);

	/**
	 * @brief incBackoff Increment backoff time, to use in case of errors
	 */
	void incBackoff(void);

	/**
	 * @brief resetBackoff Reset backoff time, to use in case of success
	 */
	void resetBackoff(void);

	/**
	 * @brief restartBackoffTimer Restart Bacokf timer, to restart timer counter
	 */
	void restartBackoffTimer(void);

	/**
	 * @brief getBackoffTime
	 *
	 * @return current backoff time
	 */
	int getBackoffTime(void);

	/**
	 * @brief getLasBackoffTime
	 *
	 * @return last backoff time, max back off time reached
	 */
	int getLastBackoffTime(void);

	/**
	 * @brief resetRestoreBackoffTime reset the restore timer
	 */
	void forceRestoreBackoffTime(void);

private:

	/**
	 * @brief restartTimeInternal
	 */
	void restartTimeInternal(void);

	std::mutex eBMutex;

	TimeThreadTime timeTh;
	RestoreThreadTime restoreTimeTh;

	Timing time;
	int firstjumpTime;
	int currentTimeout;
};

} // Utils NS
} // Comelit  NS

#endif // EXPONENTIALBACKOFF_H
