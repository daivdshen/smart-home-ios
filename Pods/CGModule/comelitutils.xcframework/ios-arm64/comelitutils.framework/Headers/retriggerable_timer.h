#ifndef RETRIGGERABLETIMER_H
#define RETRIGGERABLETIMER_H

#include <mutex>
#include <thread>
#include <sys/prctl.h>

#include "utils.h"

namespace Comelit {
namespace Utils   {

/**
 * @brief The RetriggerableTimer class
 * create a ONE SHOT timer which executed a specific timeoutCbk action if the timeout expires.
 * The timer can be stop at any time before expiring (in this case the timeoutCbk
 * is not executed)
 * the timer can be retriggered and the counting restarted.
 */
class RetriggerableTimer
{
public:
	/**
	 * @brief timeoutCbk the callback to be executed if timeout expire
	 */
	typedef std::function<void (void)> timeoutCbk;

	/**
	 * @brief RetriggerableTimer
	 * Default constructor.
	 * @warning The timer is NOT started in constructor
	 * @param cbk callback to be executed when timeout expire
	 */
	explicit RetriggerableTimer(timeoutCbk _cbk) :
		state(STOP)
		, timeout(0)
		, cbk(_cbk)
		, timerthread(nullptr)
		, threadName("thTime")
	{
	}

	virtual ~RetriggerableTimer()
	{
		stop();
	}

	/**
	 * @brief start, start the timer.
	 * @param timeout timeout in milliseconds bofore the timer expires
	 * @retval false start is already called
	 */
	bool start(unsigned int timeout)
	{
		std::unique_lock<std::mutex> mlock(mutex);

		this->timeout = timeout;

		//timer not started yet, start it
		if (timerthread)
		{
			return false;
		}

		state = INIT;
		timerthread = new std::thread([=]() {timerfunction();});
		return true;
	}

	/**
	 * @brief stop the timer
	 * the timeout callback will not be executed
	 * @retval false callback is already called
	 * @retval true callback isn't called yet
	 */
	bool stop()
	{
		{
			std::lock_guard<std::mutex> lk(mutex);

			if (!timerthread)
				return true;

			if (state < STOP)
			{
				state = STOP;
			}

			condvar.notify_one();
		}

		timerthread->join();
		delete (timerthread);
		timerthread = nullptr;

		return (state != EXPIRED);
	}

	/**
	 * @brief retrigger
	 * If timer is running, reset the timeout counting
	 * @retval false #start should be called or timer is already expired
	 */
	bool retrigger()
	{
		std::lock_guard<std::mutex> lk(mutex);

		if (!(timerthread && (state == RUN || state == RETRIGGER)))
		{
			return false;
		}

		state = RETRIGGER;
		condvar.notify_one();

		return true;
	}

	/**
	 * @brief isTimeElapsed
	 * @return true if time is elapsed, false otherwise
	 */
	bool isTimeElapsed(void)
	{
		return (state == EXPIRED);
	}


	/**
	 * @brief setThreadName set the Thread name
	 * @return true if thread name is setted, false if thread is already running
	 */
	bool setThreadName(std::string name)
	{
		std::lock_guard<std::mutex> lk(mutex);

		if (state == RUN)
		{
			return false;
		}

		threadName = name;
		return true;
	}

	/**
	 * @brief setTimeout
	 * @param time time to wait elapsed
	 * @return true if time is setted, false if timer is already running
	 */
	bool setTimeout(unsigned int time)
	{
		std::lock_guard<std::mutex> lk(mutex);

		if (state == RUN)
		{
			return false;
		}

		timeout = time;
		return true;
	}

	/**
	 * @brief getTimeout
	 * @return current time to wait elapsed
	 */
	int getTimeout(void)
	{
		return timeout;
	}

	/**
	 * @brief setCallback
	 * @param callBack function called when the time is elapsed
	 */
	void setCallback(timeoutCbk callBack)
	{
		std::lock_guard<std::mutex> lk(mutex);
		cbk = callBack;
	}

protected:
	/**
	 * @brief elapsedTime method called when the time is elapseds
	 */
	virtual void elapsedTime(void)
	{
		if (cbk != nullptr)
		{
			cbk();
		}
	};

private:
	enum TimerStatus { INIT, RUN, RETRIGGER, STOP, EXPIRED };

	/**
	 * @brief timerfunction
	 * the function which run in the timer thread
	 */
	void timerfunction()
	{
		prctl(PR_SET_NAME, threadName.c_str(), 0, 0, 0);
		std::unique_lock<std::mutex> mlock(mutex);

		if (state == STOP)
			return;

		state = RUN;

		do
		{
			condvar.wait_for(mlock, std::chrono::milliseconds(timeout));

			switch (state)
			{
			case RUN:
				elapsedTime();

				state = EXPIRED;
				break;

			case RETRIGGER:
				state = RUN;
				break;

			case STOP:

			/* FALLTHRU */
			case EXPIRED:

			/* FALLTHRU */
			default:
				break;
			}
		}
		while (state == RUN);
	}

private:
	std::mutex mutex;
	steady_condition_variable condvar;
	TimerStatus state;

	unsigned int timeout;
	timeoutCbk cbk;
	std::thread *timerthread;

	std::string threadName;
};
} // Utils NS
} // Comelit  NS
#endif // RETRIGGERABLETIMER_H
