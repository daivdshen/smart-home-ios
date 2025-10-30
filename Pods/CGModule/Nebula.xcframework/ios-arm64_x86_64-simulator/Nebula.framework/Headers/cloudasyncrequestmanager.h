/**
 *
 * @file cloudasyncrequestmanager.h
 *
 * Copyright (C) 2016 Comelit Group SpA
 *
 */

#ifndef CLOUD_ASYNC_REQUEST_MANAGER_H
#define CLOUD_ASYNC_REQUEST_MANAGER_H

#include <string>
#include <list>
#include <mutex>

#include <cloud/cloudrequest.h>
#include <cloud/httpasyncrequestmanager.h>

using std::string;
using std::list;

namespace Comelit {
namespace Nebula  {


#define MAX_ASYNC_REQUEST 30

/// Cloud asynchronus response callback type
typedef std::function<void (void *obj, CloudAnswer *response, void* cbkArg)>
CloudAsyncResponseCbk;

class CloudAsyncRequest
{
public:

	CloudAsyncRequest() : cbk(0), cbkArg(0) {}
	~CloudAsyncRequest() {}

	CloudAsyncResponseCbk cbk;
	void                 *cbkArg;
	void                 *httpReq;
};

/**
 * Manages Asynchronous cloud requests.
 * This is a convenient wrapper to send http request to Comelit Cloud using
 * HTTPRequestAsyncManager.
 *
 * make sure HTTPRequestAsyncManager is initialized and running when using
 * this class.
 *
 *  - addRequest: to send cloud request and add it to the list of pending requests
 *  - removeRequest: to remove a pending request
 */
class CloudRequestAsyncManager
{
public:

	static CloudRequestAsyncManager *getInstance()
	{
		static CloudRequestAsyncManager instance;

		return &instance;
	}

	void *addRequest(CloudRequest *req, CloudAsyncResponseCbk userCbk, void *arg);

	void  removeRequest(void *reqHandler);
private:

	CloudRequestAsyncManager() {}
	~CloudRequestAsyncManager() {}

};

} // Nebula  NS
} // Comelit NS

#endif // CLOUD_ASYNC_REQUEST_MANAGER_H
