/**
 * @file   rest-client.h
 *
 * Handles SDP exchange using rest calls
 *
 * @author Dario Corna
 */
#ifndef RESTCLIENT_H
#define RESTCLIENT_H

#include <string>
#include <thread>
#include <functional>
#include <condition_variable>

typedef std::function<void (void *arg, int errCode, const char *buf, int len, const char *topic)>
RestReceivedCb;


struct RestParameters
{
	std::string  eps;        ///< request endpoint
	std::string  ccsToken;   ///< ccs authentication token
	std::string  secret;     ///< request secret field
	std::string  duuid;      ///< device uuid of destination
	std::string  authMode;    ///< authentication mode field
	int          timeout;    ///< request timeout [s]
};

class RestClient
{
public:
	RestClient(RestParameters &params);
	~RestClient();

	/**
	 * Start rest async request manager
	 * @return 0 on success
	 */
	int start();

	/**
	 * Stop rest async request manager
	 * @return 0 on success
	 */
	int stop();

	/**
	 * Publish SDP
	 * @param[in] buf
	 * @param[in] len
	 * @return 0 on success
	 */
	int publishService(char *buf, int len);

	/**
	 * Set callback for received data
	 * @param callback
	 * @param arg
	 */
	void setRcvCallback(const RestReceivedCb &callback, void *arg);

private:
	RestParameters parameters;
	std::thread   *loopThread;
	volatile bool  quitThread;  ///< force loopThread to exit
	RestReceivedCb cbk;
	void          *cbkArg;
	volatile int   pendingReqs; ///< number of pending requests
	std::condition_variable reqCondVar; ///< request conditional variable
	std::mutex              reqMutex;   ///< Mutex to synchronize the thread operations

private:
	void runLoop();

	std::string compileRequest(char *buf); ///< create request payload from sdp
	void parseSdpAnswer(std::string &payload, int resCode);
	int errCodeToHttpEvt(const std::string &errStr);
};

#endif // RESTCLIENT_H