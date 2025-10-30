/**
 *
 * @file overwatch.h
 *
 * This class is the common interface to handle overwatch management events.
 *
 * It allows an application to subscribe to overatch service.
 * Once subscribed application has to send a keepalive message before
 * the timeout expires, otherwise it is restarted by the server.
 *
 * It depends on an already running instance of an IPC server to receive and
 * reply to overwatch management events.
 *
 * Copyright (C) 2018 Comelit Group SpA
 */

#ifndef OVERWATCH_H
#define OVERWATCH_H

#include <cmpot/common.h>
#include <cmpot/ipcmanager.h>

namespace Comelit  {
namespace MPot     {
namespace IPC      {

/**
 * The OverWatch class handles OW procedures
 *
 * Usage:
 *
 * - Create a OverWatch class giving an unique app name and ipcServer instance
 * - optionally call registerPeriodCallback() to register a period callback,
 *   the callback can signal an application failure returning false.
 *   In this case watchdog won't be refreshed anymore waiting the supervisor
 *   to kill process pid.
 * - call init() giving approriate OverWatchParameters
 *   be sure that parameters are valid by calling validate() method.
 * - call start() to start engine and init time references
 * - call run() loop on seconds basis to process overwatch actions
 *   minimum period to call run loop is 5s.
 */
class OverWatch
{
public:
	/**
	 * The OverWatchParameters class
	 * contain parameters definition to configure OverWatch service
	 */
	class OverWatchParameters
	{
	public:
		/// minimum timeout (watchdog is refreshed every REFRESH_PERIOD seconds)
		static const int MIN_TIMEOUT    = 20;
		/// period of system watchdog refresh
		static const int REFRESH_PERIOD = 10;
		/// default netstat interval [s]
		static const int NETSTAT_DEFAULT_INTERVAL = 60;
		/// default netstat max failures
		static const int NETSTAT_DEFAULT_MAX_FAILURES = 10;
		
		/**
		* OverWatchParameters default constructor
		*/
		OverWatchParameters()
		{
			comm = "";
			timeout    = 20;
			processPid = -1; // init to invalid
			rebootOnFailure = false;
			maxFailures = 0;
			enableNetStatCheck = false;
			netStatInterval = NETSTAT_DEFAULT_INTERVAL;
			netStatMaxFailures = NETSTAT_DEFAULT_MAX_FAILURES;
			
		}
		
		bool validate()
		{
			return (timeout >= MIN_TIMEOUT &&
			        processPid >= 0 &&
			        !comm.empty() &&
			        netStatInterval >= NETSTAT_DEFAULT_INTERVAL);
		}
		
		string comm; ///< Process name (output of /proc/$pid/comm)
		int timeout; ///< watchdog timeout [s]
		int processPid; ///< pid of the process to be killed
		bool rebootOnFailure; ///< if true reboot after #maxFailures restarts
		unsigned maxFailures; ///< maximum number of restart in 1h before to issue a system failure
		bool enableNetStatCheck; ///< if true check socket pending queue of the process
		int netStatInterval; ///< netstat check interval [s]
		int netStatMaxFailures; ///< max consecutives netstat checks failures
	};
	
	/**
	 * OverWatch callback
	 *
	 * @retval    true  If application is working correctly
	 * @retval    false On application failure
	 *
	 */
	typedef std::function<bool (void)> OWCbk;
	
	/// State Machine status definitions
	typedef enum
	{
		/// initial state, subscription requests sent waiting for reply
		OVERWATCH_STARTING,
		/// subscribed to watchdog management, ready to process events
		OVERWATCH_READY,
		/// system watchdog to be refreshed
		OVERWATCH_WDOG_TOREFRESH,
		/// system watchdog refreshing
		OVERWATCH_WDOG_REFRESHING,
		/// system failure wait server reset
		OVERWATCH_WDOG_FAILURE
	} OverWatchStatus;
	
public:

	OverWatch(string appName, IPCManager *ipcServer);
	// delete copy constructor and operator=() - this should be a singleton
	OverWatch(const OverWatch &)            = delete;
	OverWatch &operator=(const OverWatch &) = delete;
	
	/**
	 * Init Overwatch Parameters
	 * @param[in] params
	 * @return true if parameters are succesfully set
	 */
	bool init(OverWatchParameters &params);
	
	/**
	 * start register Listener callback and subscribe application to the service
	 * @return true on success
	 */
	bool start();
	
	/**
	 * stop unregister Listener callback and unsubscribe application to the service
	 * After calling stop it is possible to init again with different parameters
	 * @return true on success
	 */
	bool stop();
	
	/**
	 * registerOWCallback to enable timer poll on application side
	 *
	 * @param[in] cbk application callback function
	 * @param[in] period time [s] for the callback to be issued
	 */
	void registerPeriodCallback(OWCbk cbk, int period) { ovCbk = cbk; ovCbkPeriod = period; }
	
	/**
	 * run loop to manage suspension status
	 * Call it frequently (ideally when you call ipcserver run)
	 * The more frequent loop is called the more reactive
	 * will be application on pm events.
	 */
	void run();
	
	/**
	 * Return the status of ow support in the system.
	 * Some devices may not support / not need ow support.
	 */
	bool getOWSupport() { return owEnabled == OW_ENABLED; }
	
private:

	typedef enum
	{
		OW_UNKNOWN,
		OW_REQUESTING,
		OW_ENABLED,
		OW_DISABLED
	} OWEnableSt;
	
	void handleIPCOWNotification(const string method, const rapidjson::Value &params);
	
	IPCErrCode sendWatchDogRefresh();
	
	/// @return err IPC_ERR_OK if request has been correctly sent
	IPCErrCode subscribeOW();
	
	/// @return err IPC_ERR_OK if request has been correctly processed (sync request)
	IPCErrCode unsubscribeOW();
	
	void changeStatus(OverWatchStatus newstatus);
	int  getElapsedTime(std::chrono::steady_clock::time_point &timeRef);
	
	bool running;
	OverWatchParameters parameters;
	const string appName;  ///< unique application identifier
	IPCManager *ipcServer; ///< instance of a running ipc server
	OWCbk       ovCbk;     ///< Timer callback
	int         ovCbkPeriod; ///< Period [s] of ovCbk
	OverWatchStatus owStatus;  ///< state machine status
	OWEnableSt  owEnabled; ///< OW_ENABLED if overwatch is enabled in the system
	/// timeref of the last ready status
	std::chrono::steady_clock::time_point timeRefReady;
	/// timeref of the last callback issued
	std::chrono::steady_clock::time_point timeRefCbk;
	IPCRequestHandler owEnableReqHandle; ///< pointer to the actual ow enable ipc request
};


/**
 * The OverWatchImpl interface handles a client implementation of OverWatch
 *
 * Modules that uses OverWatch Management should implement watchdog method.
 * This is an interfaces for a generic implementation
 */
class OverWatchImpl
{
public:
	/// default destructor
	virtual ~OverWatchImpl() {}
	/**
	 * suspend handles a watchdog refresh request
	 * @param[in] ow a pointer to the OverWatch object
	 * @return true if module is ready to suspend
	 */
	virtual bool watchdog(OverWatch *ow) = 0;
};

}
}
}
#endif // OVERWATCH_H
