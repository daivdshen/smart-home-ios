/**
 *
 * @file cloudrequest.h
 *
 * @todo write here how the CloudRequest class should be used
 *
 * Copyright (C) 2016 Comelit Group SpA
 *
 */

#ifndef CLOUDREQUEST_H
#define CLOUDREQUEST_H

#include <string>
#include <atomic>
#include <mutex>

#include <rapidjson/document.h>

#include <cloud/httpclient.h>
#include <nebula/cloudtypes.h>

using std::string;

namespace Comelit {
namespace Nebula  {


enum CloudSvcLastOpStatus
{
	LAST_OP_UNKNOWN,
	LAST_OP_SUCCESS,
	LAST_OP_FAILURE,
	LAST_OP_SKIPPED
};

struct CloudAnswer : public ICloudAnswer
{
	CloudAnswer() : ICloudAnswer() {}
		
	rapidjson::Document jsonDoc;
	
	bool isOK() const { return errorStep == 0; }

	// ICloudAnswer interface
	const std::string getReqToken();
};

class CloudRequest
{
public:
	CloudRequest() = delete;
	
	CloudRequest(const string &urlBase,
	             const string &service,
	             const string &serviceVersion);
	             
	int setLogin(const string &customLogin); // don't call it when using the token
	int setBody(const string &newBody);  // newBody must include surrounding '{' and '}'
	
	int setToken(string token);
	
	/// Set the timeout for the cloud request [s]
	void setTimeout(long timeout);
	
	/**
	 * Synchronous method to send an HTTP request. It blocks the caller until
	 * the response is received.
	 *
	 * @return the HTTP server response received
	 */
	const CloudAnswer &doRequest();

	static void processResponse(const HTTPAnswer &response, CloudAnswer &answer);

	std::string getUrl() {return  (urlBase + "/" + service);}
	
	string createRequest();

	static void setRequestIdProvider(std::function<int()> func) {requestIdProvider = func;}
private:
	string getNextRequestId();
	string getToken();
	string createLoginSection();
	
private:
	HTTPClient        httpCl;
	
	string            urlBase;
	string            service;
	string            serviceVersion;
	string            customLogin;
	string            body;
	CloudAnswer       answer;
	
	string			token;

	static std::function<uint()> requestIdProvider;

	static std::atomic_uint globalRequestId;
};

} // Nebula  NS
} // Comelit NS

#endif // CLOUDREQUEST_H
