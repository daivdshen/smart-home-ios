/**
 *
 * @file httprequest.h
 *
 * Copyright (C) 2016 Comelit Group SpA
 *
 */

#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <string>
#include <vector>
#include <cloud/httpclient.h>
#include <atomic>

using std::string;
using std::vector;

namespace Comelit {
namespace Nebula  {

class HTTPRequest
{
public:

	HTTPRequest() = delete;
	HTTPRequest(HTTPReqType reqType, string url, string contents = "", string mimeType = "");
	HTTPRequest(HTTPReqType reqType, string url, string contents, string mimeType, vector<string>& headers);
	HTTPRequest(HTTPReqType reqType,
	            const string &url,
	            const string &contents,
	            const string &mimeType,
	            vector<string>& headers,
	            const string &username,
	            const string &password);

		
public:
	/// Set the timeout for the cloud request [s]
	void setTimeout(long timeout) __attribute__ ((deprecated));
	
	/**
	 * Synchronous method to send an HTTP request. It blocks the caller until
	 * the response is received.
	 *
	 * @return the HTTP server response received
	 */
	const HTTPAnswer &doRequest();
	
	/**
	 * Asynchronous method to send an HTTP request. It returns immediately
	 * the unique identifier of the request and set the CURL easy object
	 * that should be used to send the request.
	 */
	void doAsyncRequest(HTTPCurlData **curl);

	HTTPClient &getHttpClient();

	/**
	 * @brief Convert a map of key and content in a single string where all
	 * all letters consider illegal in URLs are converted to their %XX versions
	 * @note only content fields are converted
	 * @param items map of key and content
	 * @return encoded string
	 */
	static string getUrlEncodedContent(const std::map<string, string> &items);
	
private:
	string getNextRequestId();
	
private:
	HTTPClient  httpCl;
	HTTPReqType reqType;
	string      url;
	string      contents;
	string      mimeType;
	HTTPAnswer  answer; ///< Answer to the sync request
	vector<string> customHeaders;
	string username;
	string password;
};

} // Nebula  NS
} // Comelit NS

#endif // HTTPREQUEST_H
