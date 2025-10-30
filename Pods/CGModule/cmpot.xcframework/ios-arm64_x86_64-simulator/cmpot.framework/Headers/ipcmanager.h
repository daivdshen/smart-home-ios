/**
 *
 * @file ipcmanager.h
 *
 * Manage Interprocessing Communication data
 * throught an mqtt client based on mosquitto library
 *
 * Copyright (C) 2016 Comelit Group SpA
 *
 */

#ifndef IPCMANAGER_H
#define IPCMANAGER_H

#include <mosquittopp.h>
#include <functional>
#include <thread>
#include <mutex>
#include <ctime>
#include <ratio>
#include <chrono>
#include <deque>
#include <atomic>
#include <set>

#include <cmpot/ipccommon.h>

class IPCRPCMessage;
class IPCRequest;
class IPCService;
class IPCMosquittoMessage;

namespace Comelit  {
namespace MPot     {
namespace IPC	   {

#define MPOT_IPC_MESSAGE_TIMEOUT  1000 ///< default request timeout in [ms]

/**
 * The IPCManager class handles ipc engine and communication to the mqtt broker.
 * Basic running mode is:
 *
 * - IPCManager constructor providing an unique identifier of the application
 * - optionally init providing \ref ConnectionParameters
 * - start() initialize mqtt connection
 * - run() call run on a loop, call it frequently means the engine is going to be more responsive
 *         if timeout is set loop exits on new event or on timeout
 *
 * Client usage:
 * - doAsyncRequest() or doRequest()
 *
 * Service provider usage:
 * - registerService() providing callback
 * - Call sendAnswer() from callback context
 *
 * Listener usage:
 * - listenService() providing callback
 */
class IPCManager : private mosqpp::mosquittopp
{
public:

	/**
	 * Mqtt connection paramers
	 * by default it connects to the a local broker
	 * without authentication on a LOCAL deviceId root topic
	 */
	struct ConnectionParameters
	{
		/**
		 * ConnectionParameters default constructor
		 */
		ConnectionParameters();
		
		/**
		 * Validate parameters
		 * @return true if all required parameters are set, false otherwise
		 */
		bool validate(void);
		
		/// unique identifier of the physical device, this has to be the same for all ipc connections
		/// it can be left at default value if engine is used locally
		string deviceId;
		string host; ///< host address of the mqtt broker
		int port;    ///< port of the mqtt broker
		bool auth;   ///< true if the broker require authentication
		string username;  ///< username
		string password;  ///< password
		int	   keepalive; ///< keepalive time to send on the server [s]
	};
	
	/**
	 * IPCManager default constructor
	 */
	IPCManager(string clientId);
	
	~IPCManager();
	
	/**
	 * Set connection parameters,
	 * Note that this function doesn't perform the connection
	 * @param[in] params params to be set
	 * @return set result
	 * @retval 0 correctly set
	 * @retval any error setting parameters
	 */
	int init(ConnectionParameters params);
	
	/**
	 * Start ipc engine
	 *
	 * @return IPCErrCode
	 */
	IPCErrCode	start(void);
	
	/**
	 * Stop ipc engine by perform mqtt disconnection
	 */
	void stop(void);
	
	/**
	 * Run main cycle, this function must be continously called to let ipc engine work
	 * @param[in] execution timeout [ms] [-1..100], 0 -> exit immediately, -1 -> exit on default timeout (100)
	 *
	 * @return IPCErrCode
	 */
	IPCErrCode run(int timeout = 0);
	
	/**
	 * Add a request to messages queue
	 * Note that request is sent immediately and enqueued
	 * to messages list only if user define a callback
	 * @param[in] serviceName service name the request is directed to
	 * @param[in] method method to be called
	 * @param[in] params json parameters of the request
	 * @param[in] cbk userdefined callback
	 * @param[in] arg userdefined callback argument
	 * @param[out] IPCErrCode error code (on transport layer)
	 * @param[in] timeout of the request, by omitting that default is set \ref MPOT_IPC_MESSAGE_TIMEOUT
	 * @return IPCRequestHandler pointer to the request, NULL on error
	 */
	IPCRequestHandler	doAsyncRequest(const std::string serviceName, const string method,
	                                       rapidjson::Document &params,
	                                       IPCAsyncResponseCbk cbk, void *arg, IPCErrCode &err, int timeout = MPOT_IPC_MESSAGE_TIMEOUT);
	                                       
	/**
	 * Perform a sync request to a service.
	 * It blocks until an answer is received or timeout occured
	 *
	 * @param[in]  serviceName service name the request is directed to
	 * @param[in]  method method to be called
	 * @param[in]  params json parameters of the request
	 * @param[out] answer
	 * @param[in]  timeout of the request [\ref MPOT_IPC_MESSAGE_TIMEOUT]
	 * @return IPCErrCode
	 */
	IPCErrCode	doRequest(const std::string serviceName, const string method,
	                          rapidjson::Document &params,
	                          IPCAnswer &ans, int timeout = MPOT_IPC_MESSAGE_TIMEOUT);
	                          
	/**
	 * Send a broadcast notification
	 *
	 * @param serviceName service name
	 * @param event       event name
	 * @param params      json parameters
	 * @param waitPublish do not exit until message is sent
	 * @return IPCErrCode
	 */
	IPCErrCode	sendEvent(const std::string serviceName, const string event,
	                          rapidjson::Document &params, bool waitPublish = false);
	                          
	/**
	 * Send an answer to a message from a registered service.
	 * Fill response according to JSONRpc 2.0 specifications http://www.jsonrpc.org/specification#response_object
	 * fill result json object in case of message correctly handled,
	 * othewise compile rpcError with code, message and, optionally, data.
	 * Do not fill both result and rpcError, otherwise answer is interpreted as an error
	 * and result field ignored accordingly.
	 * Note that this function must be called from the callback
	 * generated by the request by passing as argument the same
	 * pointer obtained in the service callback.
	 *
	 * @param response jsonrpc2.0 response
	 * @param[in] arg
	 * @return IPCErrCode
	 */
	IPCErrCode	sendAnswer(IPCRPCResponse &response, void *arg);
	
	/**
	 * Register a service to be provided
	 * call \ref unRegisterService to remove it
	 * @param[in] name service name
	 * @param[in] cbk userdefined callback on new message
	 * @return IPCErrCode
	 */
	IPCErrCode  registerService(const std::string name, CallbackOnMessageProvider cbk);
	
	/**
	 * unregister a service
	 * @param[in] name service name
	 */
	void        unRegisterService(const std::string name);
	
	/**
	 * Listen broadcasted events of a service
	 * @param[in] name service name
	 * @param[in] cbk userdefined callback on new messages
	 * call \ref unListenService to remove it
	 * @return IPCErrCode
	 */
	IPCErrCode	listenService(const std::string name, CallbackOnMessageListener	cbk);
	
	/**
	 * Delete a listen subscription
	 * @param[in] name service name
	 */
	void        unListenService(const std::string name);

	/**
	 * Sniff events of a service
	 * @param[in] name service name
	 * @param[in] cbk userdefined callback on new messages
	 * call \ref unListenService to remove it
	 * @return IPCErrCode
	 */
	IPCErrCode	sniffServiceOn(const std::string name, CallbackOnMessageSniffed	cbk);

	/**
	 * Delete a sniff subscription
	 * @param[in] name service name
	 */
	void        sniffServiceOff(const std::string name);
	
	/**
	 * Remove a request from pending messages list
	 * @param req
	 * @return false in case of error
	 */
	bool        removeRequest(IPCRequestHandler req);
	
	/**
	 * threadedSet set true if your application handles ipc in different threads
	 * @param threaded
	 * @return IPCErrCode
	 */
	IPCErrCode threadedSet(bool threaded);
	
	/**
	 * isReady return services registered and listened status
	 * @return true if services are up and ready to receive requests
	 */
	bool       isReady();
	
private:

	/**
	 * Subscrive a service to the mqtt broker
	 */
	IPCErrCode subscribeService(IPCService *service);
	
	/**
	 * Subscrive a listener to the mqtt broker
	 */
	IPCErrCode subscribeListenService(IPCService *service);

	/**
	 * Subscrive a sniffer to the mqtt broker
	 */
	IPCErrCode subscribeSniffService(IPCService &service);
	
	/**
	 * Subscribe standard message respose's topic to the mqtt broker
	 */
	IPCErrCode subscribeResponses();
	
	/**
	 * Send answer on mqtt bus
	 */
	IPCErrCode sendAnswerOnBus(IPCRPCResponse &response, IPCRPCMessage *arg);
	
	// on_message private handlers
	/**
	 * Handles message answer to a sync request from mqtt layer
	 * @param[in] message
	 * @param[in] tokens
	 */
	void	handlerMsgSyncAnswer(const mosquitto_message *message, vector<string> &tokens);
	
	/**
	 * Handles message received on rx channel from mqtt layer
	 *
	 * Call this function to handles messages for a service provider
	 * @param[in] message
	 * @param[in] tokens
	 */
	void	handlerMsgProvider(const mosquitto_message *message, vector<string> &tokens);
	
	/**
	 * Handles responses from service provider of requests sent previously
	 *
	 * @param[in] message
	 * @param[in] tokens
	 */
	void	handlerMsgResponse(const mosquitto_message *message, vector<string> &tokens);
	
	/**
	 * Handles broadcast message received on tx channel from mqtt layer
	 * @param[in] message
	 * @param[in] tokens
	 */
	void	handlerMsgBroadcast(const mosquitto_message *message, vector<string> &tokens);

	/**
	 * Handles sniffeg message received on rx/tx/broadcast channels from mqtt layer
	 * @param[in] message
	 * @param[in] tokens
	 */
	void	handlerMsgSniffed(const mosquitto_message *message, vector<string> &tokens);
	
	/**
	 * updateReadyFlag check services and update isReady flag
	 * @return isServiceReady flag status
	 */
	bool	updateReadyFlag();
	
	/**
	 * Clear list elements and delete pointers
	 * @param[in,out] list
	 */
	template <typename T>
	void	clearList(std::list<T *> &list)
	{
		for (auto elem : list)
		{
			delete elem;
		}
		
		// resources are now free, clear list
		list.clear();
	}
	
	template <typename T>
	void	clearDeque(std::deque<T *> &list)
	{
		for (auto elem : list)
		{
			delete elem;
		}
		
		// resources are now free, clear list
		list.clear();
	}
	
	/**
	 * Process pending received message
	 * @param[in] message
	 */
	void	processMessage(const mosquitto_message *message);
	
	// mosquittopp interface
	
	void	on_connect(int rc);
	
	void	on_message(const mosquitto_message *message);
	
	void	on_error();
	
	void	on_disconnect(int rc);
	
	void	on_publish(int mid);
	
	void	on_subscribe(int mid, int qos_count, const int *granted_qos);
	
	void	on_unsubscribe(int mid);
	
	void	on_log(int level, const char *str);
	
private:

	typedef std::set<IPCService> ServicesList;

	string						clientId;   ///< unique ID of the process running this instance
	ConnectionParameters		connParams; ///< mqtt connections parameters
	bool						running;	///< engine status
	bool						snifferMode = false; ///< flag to identify if class is used as sniffer
	
	// following mutex are defined to protect concurrent access to the lists
	std::mutex					messagesMutex; ///< protects access to messages list
	std::mutex					servicesMutex; ///< protects access to services list
	std::mutex					listenMutex;   ///< protects access to listen services
	std::mutex					sniffedMutex;  ///< protects access to sniffed services
	std::mutex					runMutex;     ///< protects run loop and pendingMessages
	std::mutex					requestsMutex; ///< protects access to requests list
	std::mutex					cMessagesMutex; ///< protects access to completed messages list
	std::mutex					loopMutex;      ///< protect access to mosquitto loop
	
	std::list<IPCService *>		services;	    ///< services list
	std::list<IPCService *>		listenServices; ///< listened services list
	std::list<IPCRequest *>		messages;		 ///< messages list
	std::list<IPCRequest *>		completedMessages;	 ///< completed messages list
	/// list of pending requests waiting for an async answer from the server
	std::list<IPCRPCMessage *>	pendingRequests;
	ServicesList				sniffedServices; ///< sniffed services list

	std::chrono::steady_clock::time_point timeRef;    ///< used for internal timer
	
	/// There should be only one sync request waiting for answer.
	/// While waiting for answer all the other received messages will be added to
	/// ::pendingMessages queue to be processed later.
	IPCRequest  *pendingSyncRequest;
	
	/// There should be only one send request waiting for publish.
	/// While waiting for publish all the other received messages will be added to
	/// ::pendingMessages queue to be processed later.
	IPCRequest  *pendingSendRequest;
	
	/// This queue stores all the messages which comes in the middle of a sync request
	/// waiting for answer
	std::deque<IPCMosquittoMessage *> pendingMessages;
	
	std::atomic_bool
	isServiceReady; ///< flag set to true when all services are ready to receive messages
	
	std::atomic_bool
	syncRequestRequired; ///< flag set to true when a pending request lock is required
	
	struct
	{
		bool        subscribed; ///< subscription status
		int         lastMid;    ///< mid of the last subscription request on this topic
	} rxTopic; ///< topic for message's responses
};

} // namespace IPC
} // namespace MPot
} // namespace Comelit

#endif // IPCMANAGER_H
