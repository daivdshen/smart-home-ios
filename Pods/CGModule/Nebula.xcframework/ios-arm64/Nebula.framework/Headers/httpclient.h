/**
 *
 * @file httpclient.h
 *
 * HTTPClient class: performs an HTTP/HTTPS request
 * through libcurl library
 *
 * Copyright (C) 2016 Comelit Group SpA
 *
 */

#include <string>
#include <cstring>

#include <stddef.h>
#include <functional>
#include <map>
#include <vector>

#include <curl/curl.h>

#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

namespace Comelit {
namespace Nebula  {

using std::string;
using std::function;
using std::vector;

enum HTTPReqType
{
	HTTP_GET,
	HTTP_POST,
	HTTP_DELETE
};

struct HTTPAnswer
{
	int    retCode; ///< lib curl_easy_perform() result code
	string payload; ///< HTTP response string
	long   httpCode;
};

typedef function<bool (const void *buf, size_t len, void *arg)> HttpDataReceivedCb;

/**
 * @brief The HTTPCurlData struct
 * is a structure to wrap data using for a CURL
 * request.
 */
struct HTTPCurlData
{
	CURL *curl {nullptr};
	curl_slist *headers{nullptr};

	HTTPCurlData();

	~HTTPCurlData();
};

class HTTPClient
{
public:
	HTTPClient() : httpDataRcvCb(0), httpDataRcvCbArg(0), answer( {CURLE_FAILED_INIT, "", 0}),
	m_timeout(30L), m_lowspeedlimit(0), m_lowspeedtime(0), fileoffset(0) {}
	
	~HTTPClient();

	static void       globalInit();
	static void       globalCleanup();

	struct HttpData
	{
		string url;
		string contents;
		string mimeType;
		vector<string> *customHeaders;
		string username;
		string password;

		HttpData():
		        customHeaders(nullptr)
		{}

		HttpData(const string &url_,
		         const string &contents_,
		         const string &mimeType_,
		         vector<string> *hdr):
		        url(url_)
		      , contents(contents_)
		      , mimeType(mimeType_)
		      , customHeaders(hdr)
		{}

		HttpData(const string &url_,
		         const string &contents_,
		         const string &mimeType_,
		         vector<string> *hdr,
		         const string &username_,
		         const string &password_):
		        url(url_)
		      , contents(contents_)
		      , mimeType(mimeType_)
		      , customHeaders(hdr)
		      , username(username_)
		      , password(password_)
		{}
	};


	/**
	 * Send POST request, all fields required
	 * @note blocking call
	 */
	HTTPAnswer sendPostRequest(const string &url, const string contents,
	                           const string mimeType = "", vector<string> *customHeaders = nullptr);
	HTTPAnswer sendPostRequest(const HttpData &data);

	/**
	 * Send GET request, contents and mimeType optional
	 * @note blocking call
	 */
	HTTPAnswer sendGetRequest(const string &url, const string contents = "",
	                          const string mimeType = "", vector<string> *customHeaders = nullptr);
	HTTPAnswer sendGetRequest(const HttpData &data);
	                          

	/**
	 * Send DELETE request, contents and mimeType are not admitted
	 * @note blocking call
	 */
	HTTPAnswer sendDeleteRequest(const HTTPClient::HttpData &data);

	/**
	 * Set the callback for receiving data as soon the module receives it.
	 * If the callback is set the data is no more collected by the module,
	 * it's user responsability the handle data properly.
	 *
	 * @param cb user callback function
	 * @param arg callback correlator
	 */
	void setRecvCallback(HttpDataReceivedCb cb, void *arg)
	{
		httpDataRcvCb    = cb;
		httpDataRcvCbArg = arg;
	}

	/**
	 * Set the callback for receiving header as soon the module receives it.
	 *
	 * @param cb user callback function
	 * @param arg callback correlator
	 */
	void setRecvHdrCallback(HttpDataReceivedCb cb, void *arg)
	{
		httpHdrRcvCb    = cb;
		httpHdrRcvCbArg = arg;
	}
	
	/// Set the timeout for the http client request [s]
	void setTimeout(long timeout) { m_timeout = timeout; }
	/// Set low speed parameters, limit in byte per second, time in seconds
	void setLowSpeed(long limit, long time) { m_lowspeedlimit = limit; m_lowspeedtime = time; }
	/// Set the byte offset where to start file download
	void setFileOffset(unsigned long offset) { fileoffset = offset; }
	
public:

	HttpDataReceivedCb httpDataRcvCb;
	void              *httpDataRcvCbArg;
	HttpDataReceivedCb httpHdrRcvCb;
	void              *httpHdrRcvCbArg;
	HTTPAnswer         answer;
	
private:

	long m_timeout;
	long m_lowspeedlimit;
	long m_lowspeedtime;
	unsigned long fileoffset;
	
	HTTPCurlData   *prepareRequest(const HTTPReqType &reqType, const HttpData &data);

	HTTPAnswer sendRequest(const HTTPReqType &reqType, const HttpData &data);

	/**
	 * Send POST request, all fields required
	 * @note asynchronous call, results are notified via callback
	 */
	HTTPCurlData *sendPostAsyncRequest(const HttpData &data);

	/**
	 * Send GET request, contents and mimeType optional
	 * @note asynchronous call, results are notified via callback
	 */
	HTTPCurlData *sendGetAsyncRequest(const HttpData &data);

	/**
	 * Send DELETE request, all fields required
	 * @note asynchronous call, results are notified via callback
	 */
	HTTPCurlData *sendDeleteAsyncRequest(const HTTPClient::HttpData &data);

	friend class HTTPRequest;
};

} // Nebula  NS
} // Comelit NS

#endif // HTTPCLIENT_H
