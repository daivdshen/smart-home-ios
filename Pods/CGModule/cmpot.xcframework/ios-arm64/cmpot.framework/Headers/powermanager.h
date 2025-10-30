/**
 *
 * @file powermanager.h
 *
 * This class is the common interface to handle power management events.
 *
 * It allows an application to subscribe to pm services, this way it is able to:
 *
 * - Listen for pm events (suspend/resume)
 * - Lock the system awaken for a given period of time, during the locking period
 *   the whole system will wait before going to suspension mode
 *
 * It depends on an already running instance of an IPC server to receive and
 * reply to power management events.
 *
 * Copyright (C) 2017 Comelit Group SpA
 */

#ifndef POWERMANAGER_H
#define POWERMANAGER_H

#include <cmpot/common.h>
#include <cmpot/ipcmanager.h>

namespace Comelit  {
namespace MPot     {
namespace IPC      {

static const string EVENT_SUSPEND       = "suspend";
static const string EVENT_RESUME        = "resume";
static const string EVENT_PM_ENABLED    = "pm-enabled";
static const string EVENT_REBOOT        = "reboot";

static const string REASON_WAKEUP_WAKELOCK = "wakelock"; ///< resume due to an application wakelock
static const string REASON_WAKEUP_HW       = "hw"; ///< resume due to an hardware wakeup

/**
 * The PowerManager class handles PM procedures
 *
 * Usage:
 *
 * - Create a PowerManager class giving an unique app name and ipcServer instance
 * - call registerPMEvent() to register a callback for PM events (resume/suspend),
 *   the callback can refuse suspension by returning false.
 *   In this case it will be continuously notified by the run loop until the callback
 *   returns true.
 * - call start() to start listening IPC PM events and subscribe application
 * - call run() loop to process pm events
 * - optionally use wakelockrequests to lock the system alive for a given
 *   period of time
 */
class PowerManager
{
public:

	/**
	 * Power manager callback
	 *
	 * @param[in] event Possible events:
	 *                  #EVENT_SUSPEND     system is in suspension process.
	 *                  #EVENT_RESUME      system is resuming from a previous suspension.
	 *                  #EVENT_PM_ENABLED  system is reporting that it implements power suspension.
	 *                  #EVENT_REBOOT      system is in reboot process.
	 * @retval    true  If event has been processed and accepted by application
	 *                  On EVENT_SUSPEND application should return true only when it is ready to suspend.
	 * @retval    false If application do not accept event
	 *                  On EVENT_SUSPEND application should return false if it is not yet ready to suspend.
	 *
	 */
	typedef std::function<bool (string event)> PMCbk;
	
	/// State Machine status definitions
	typedef enum
	{
		/// initial state, subscription requests sent waiting for reply
		POWER_STARTING,
		/// subscribed to power management, ready to process events
		POWER_READY,
		/// received suspend request waiting for application response
		POWER_SUSPEND_WAITING,
		/// system suspension accepted
		POWER_SUSPENDING,
		/// system suspension confirmed by servive provider
		POWER_SUSPENDED,
		/// system resuming
		POWER_RESUMING
	} PowerStatus;
	
	/**
	 * Suspend priority currently defined.
	 * The priorities handled by the pm service are in range [0..0xFF]
	 * domanda: oppure possono sforare perchè è un semplice intero?
	 */
	enum SuspendPriority
	{
		PM_PRIO_NORMAL       = 0x00, ///< Minimum priority, accept if application has no pending operations.
		PM_PRIO_SB_BUSY      = 0x80, ///< Simplebus bus is busy, suspend any pending operation as soon as possible.
		PM_PRIO_VIPCALL_BUSY = PM_PRIO_SB_BUSY, ///< Call in progress, suspend any pending operation as soon as possible.
		PM_PRIO_FIRST_BOOT   = 0xC0, ///< First boot, suspend immediately any pending action.
		/** Forced sleep, system is encountering problems on power.
		  * This case is never notified synce a suspension of this priority must be accepted
		  * immediately.
		  * Application can still force system to refuse it by require a wakelock, but in this
		  * case keep in mind that most probably system will be suddenly powered down soon.
		  */
		PM_PRIO_FORCED  = 0xFF
	};
	
public:

	PowerManager(string appName, IPCManager *ipcServer);
	// delete copy constructor and operator=() - this should be a singleton
	PowerManager(const PowerManager &)            = delete;
	PowerManager &operator=(const PowerManager &) = delete;
	
	/**
	 * start register Listener callback and subscribe application to the service
	 */
	void start();
	
	/**
	 * registerPMEvent to be notified on PM events
	 * register a callback to be notified on PM events
	 * @param[in] cbk application callback function
	 */
	void registerPMEvent(PMCbk cbk) { pmCbk = cbk; }
	
	PowerStatus getStatus() { return pmStatus; }
	
	/**
	 * run loop to manage suspension status
	 * Call it frequently (ideally when you call ipcserver run)
	 * The more frequent loop is called the more reactive
	 * will be application on pm events.
	 */
	void run();
	
	/**
	 * Return the time elapsed since the system requested the suspension [ms]
	 */
	int getSuspendElapsedTime();
	
	/**
	 * Return the time elapsed since the system resumed [ms]
	 */
	int getReadyElapsedTime();
	
	/**
	 * Get the timeout [s] the system is scheduled to wakeup after suspension
	 * Note that system can wakeup earlier for an incoming call event
	 * @return time [s]
	 */
	int getNextWakeupTime();
	
	/**
	 * Return the priority of the current suspend request
	 */
	int getCurrentSuspendPriority();
	
	/**
	 * Return the reason of the current wakeup request
	 */
	string getCurrentWakeupReason();
	
	/**
	 * Return the time [s] the system is going to reboot
	 */
	int getRebootDelay() { return rebootDelay; }
	
	/**
	 * Return the status of pm support in the system.
	 * Some devices may not support / not need pm support
	 * this way application can know if, for example, it needs
	 * to use wakelocks.
	 */
	bool getPMSupport() { return pmEnabled == PM_ENABLED; }
	
	/**
	 * Request system to stay awaken for a given period of time
	 * Wakelock is accepted by the system if the ongoing suspension
	 * procedure (if any) has a lower or equal priority.
	 *
	 * Wakelock can be requested even when there is an
	 * ongoing suspension procedure (even in the suspension callback).
	 * However a good practice is to request the wakelock when the
	 * application knows that can't accept a suspension in order to don't
	 * harms other applications.
	 *
	 * Application can have only one wakelock active at time!
	 *
	 * @param[in] priority request priority ref #SuspendPriority
	 * @param[in] timeout  time the system is required to stay awake [s]
	 *
	 * @return err IPC_ERR_OK if request has been correctly sent
	 */
	IPCErrCode wakeLockRequest(int priority, int timeout);
	
	/**
	 * Release a wakelock previously acquired.
	 * Wakelock is released after timeout but if it's no more needed
	 * it should be released calling this function.
	 *
	 * @return err IPC_ERR_OK if request has been correctly sent
	 */
	IPCErrCode wakeLockRelease();
	
	/**
	 * Issue a reboot request
	 * @param[in] priority
	 * @return
	 */
	IPCErrCode rebootRequest(int priority);
	
	/**
	 * Send a generic wakeup request
	 * This is handled by powermanager server
	 * as a generic event on the system that is currently alive.
	 * If a suspension procedure is in progress it is withdrawn
	 * by the server.
	 * @return
	 */
	IPCErrCode wakeUpRequest();
	
private:

	typedef enum
	{
		PM_UNKNOWN,
		PM_REQUESTING,
		PM_ENABLED,
		PM_DISABLED
	} PMEnableSt;
	
	void handleIPCPMNotification(const string method, const rapidjson::Value &params);
	/// @return err IPC_ERR_OK if request has been correctly sent
	IPCErrCode sendSuspendedNotification();
	/// @return err IPC_ERR_OK if request has been correctly sent
	IPCErrCode subscribePM();
	void changeStatus(PowerStatus newstatus);
	int  getElapsedTime(std::chrono::steady_clock::time_point &timeRef);
	
	const string appName;  ///< unique application identifier
	IPCManager *ipcServer; ///< instance of a running ipc server
	PMCbk       pmCbk;     ///< Events callback, notify application of suspend requests and resume event
	PowerStatus pmStatus;  ///< state machine status
	PMEnableSt  pmEnabled; ///< PM_ENABLED if powermanager is enabled in the system
	/// timeref of the last ready status
	std::chrono::steady_clock::time_point timeRefReady;
	/// timeref of POWER_SUSPENDWAITING status
	std::chrono::steady_clock::time_point timeRefSuspend;
	int         wakeupTime; ///< on suspend request the time the system will be awaken again [s]
	int         priority;   ///< on suspend request the priority of the request
	string      wakeupReason; ///< on resume the reason of the wakeup
	int         rebootDelay;///< on reboot request the time the system will reboot [s]
};


/**
 * The PowerManagerImpl interface handles a client implementation of powermanager
 *
 * Modules that uses Power Management should implement suspend and resume methods.
 * This is an interfaces for a generic implementation
 */
class PowerManagerImpl
{
public:
	/// default destructor
	virtual ~PowerManagerImpl() {}
	/**
	 * suspend handles a suspension request
	 * @param[in] pm a pointer to the PowerManager object
	 * @return true if module is ready to suspend
	 */
	virtual bool suspend(PowerManager *pm) = 0;
	
	/**
	 * resume handles a resume notification
	 * Calling this methods module ideally perform
	 * the reverse it did on suspend.
	 * @param[in] pm
	 */
	virtual void resume(PowerManager *pm) = 0;
};

}
}
}
#endif // POWERMANAGER_H
