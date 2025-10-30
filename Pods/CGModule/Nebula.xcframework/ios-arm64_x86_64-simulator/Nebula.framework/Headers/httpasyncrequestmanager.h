/**
 *
 * @file httpasyncrequestmanager.h
 *
 * Copyright (C) 2017 Comelit Group SpA
 *
 */

#ifndef HTTP_ASYNC_REQUEST_MANAGER_H
#define HTTP_ASYNC_REQUEST_MANAGER_H

#include <mutex>
#include <list>
#include <cloud/httprequest.h>
#include <thread>
#include <atomic>
#include <queue>

namespace Comelit {
namespace Nebula  {

/// Http asynchronus response callback type
typedef void (*HTTPAsyncResponseCbk)(void *obj, HTTPAnswer *response, void *cbkArg);

class HTTPAsyncRequest
{
public:

	HTTPAsyncRequest() : curldata(nullptr), cbk(nullptr), cbkArg(nullptr) {}
	~HTTPAsyncRequest() {}
	
	HTTPCurlData         *curldata;
	HTTPAsyncResponseCbk  cbk;
	void                 *cbkArg;
	HTTPAnswer            answer;
};

/**
 * Manages Asynchronous http requests.
 *
 * Singleton class, call:
 *
 * usage:
 * mode 1) application mode: application is responsable to call init, procedure
 * and then periodically call run()
 *  - init:    to initialize curl multi interface library and internal data
 *  - cleanup: to clean curl multi interface library and abort all the requests
 *  - run:     process requests and read responses, it needs to be called frequently
 *             to process all the requests in time.
 *
 * mode 2) auto mode: application just need to call start and stop methods
 * HTTPRequestAsyncManager will start it's own thread for processing requests.
 *  - start: initialize curl multi interface and start runner thread
 *  - stop:  stop runner thread and cleanup resources
 *
 * for sending http requests (both modes):
 *  - addRequest: to send http request and add it to the list of pending requests
 *  - removeRequest: to remove a pending request
 *
 * Note: application mode and auto mode cannot be used togheter.
 *
 */
class HTTPRequestAsyncManager
{
public:
	static HTTPRequestAsyncManager *getInstance()
	{
		static HTTPRequestAsyncManager instance;
		
		return &instance;
	}
	
	/**
	 * @brief init
	 * initialize curl multi interface
	 */
	void init();

	/**
	 * @brief cleanup
	 * cleanup pending requests, cleanup curl resources
	 * @warning this method is not thread safe against run() method
	 */
	void cleanup();

	/**
	 * @brief run
	 * run curl multi interface, send http requests and process
	 * http replies
	 */
	void run();

	/**
	 * @brief start
	 * initialize curl multi interface and
	 * start a runner thread which send http requests and process http answers
	 * @return true on success, false on failure
	 */
	bool start();

	/**
	 * @brief stop
	 * stop runner tread, cleanup pending requests, cleanup curl resources
	 * @return
	 */
	bool stop();
	
	/**
	 * @brief addRequest
	 * add one http request to request list. http request is processed and
	 * callback is executed when request is completed
	 * @param req http request to send
	 * @param userCbk callback to be executed when request is cmpleted
	 * @param arg callback argument to be returned inside callback
	 * @return a valid pointer if request is taken for processing, null pointer
	 * in case of failure
	 */
	void *addRequest(HTTPRequest *req, HTTPAsyncResponseCbk userCbk, void *arg);
	
	/**
	 * @brief removeRequest
	 * remove a request previously added with addRequest() from the list.
	 * the request will not be executed, and callback not executed
	 * @param reqHandler the pointer to the pending request, that should be
	 * the one returned by addRequest()
	 */
	void  removeRequest(void *reqHandler);
	
	/**
	 * @brief getReqListSize
	 * get the number of requests currently pending
	 * @return the number of pending requests
	 */
	int getReqListSize();

private:
	/**
	 * @brief cleanupInternal
	 * cleanup pending requests and release curl resources
	 */
	void cleanupInternal();

	/**
	 * @brief runInternal
	 * run curl multi interfaces
	 * curl_multi_perform()
	 * curl_multi_read()
	 * and process http replies
	 */
	void runInternal();

	/**
	 * @brief remove_internal
	 * remove pending request from requests list
	 * and cleanup associated curl resources
	 * @param reqHandler
	 */
	void removeInternal(void *reqHandler);

	/**
	 * @brief waitEvent
	 * wait for one event to happen on http sockets, or on the internal pipe
	 * basically a wrapper for select() call, done on curl file descriptors
	 * and internal pipe file descriptors
	 */
	void waitEvent();

	/**
	 * @brief toggleEvent
	 * togle one event on the internal pipe file.
	 * this can be used to unlock waitEvent() call.
	 */
	void toggleEvent();

	/**
	 * @brief getWaitTimeout
	 * calculate the timeout that we should wait for some event to happen
	 * based on curl desired timeout and internal min max values
	 * @return the timeout to be used in waitEvent()
	 */
	timeval getWaitTimeout();

private:
	enum Mode {
		NONE,
		APPLICATION,
		AUTO
	};

	HTTPRequestAsyncManager() : mode(NONE), multi_handle(nullptr), runCloudAsyncManager(false), runnerThread(nullptr), pipeBuffer(10) { }
	~HTTPRequestAsyncManager() {}

	Mode mode;
	
	CURLM *multi_handle;
	
	std::mutex reqListMutex;
	std::list<HTTPAsyncRequest *> reqList;

	std::mutex removeListMutex;
	std::list<void *> toRemoveList;

	std::atomic<bool> runCloudAsyncManager;
	std::thread *runnerThread;

	std::vector<uint8_t> pipeBuffer;
	int pipeFp[2];
	fd_set fdread;
	fd_set fdwrite;
	fd_set fdexcep;
};

} // Nebula  NS
} // Comelit NS

#endif // HTTP_ASYNC_REQUEST_MANAGER_H
