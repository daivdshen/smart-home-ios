#ifndef PSEUDOTCPSCK_H
#define PSEUDOTCPSCK_H

#include <thread>
#include <functional>
#include <map>
#include <mutex>
#include <chrono>

#include "pseudotcp.h"

using std::function;

typedef std::function<void (const char *, int len)>	pseudo_tcp_send_cbk;

typedef std::function<void (char *, int len)>	pseudo_tcp_rx_cbk;

typedef std::function<void ()>	pseudo_tcp_open_cbk;

typedef std::function<void ()>	pseudo_tcp_close_cbk;

class PseudoTcpSck
{
public:
	typedef enum
	{
		STATUS_IDLE,
		STATUS_CONNECTING,
		STATUS_CONNECTED,
		STATUS_DISCONNECTING,
		STATUS_DISCONNECTED
	} SocketStatus;

public:
	PseudoTcpSck();

	PseudoTcpSck(pseudo_tcp_send_cbk sendCbk,
	             pseudo_tcp_rx_cbk rxCbk,
	             pseudo_tcp_open_cbk openCbk,
	             pseudo_tcp_close_cbk closeCbk);

	~PseudoTcpSck();

	void requestDisconnect();
	void requestConnect();

	int sendData(const char *, int);

	bool notify(const char *, int);

	bool run();

	void setStatus(SocketStatus newStatus) { status = newStatus; }
	SocketStatus getStatus() { return status; }

	pseudo_tcp_send_cbk mySend;
	pseudo_tcp_rx_cbk myRx;
	pseudo_tcp_open_cbk myOpen;
	pseudo_tcp_close_cbk myClose;

	bool         connectionReq;
	bool         disconnectionReq;

private:
	/**
	 * Socket connection status
	 * socket is connected on pseudotcp open callback
	 * and disconnected on pseudotcp close callback (with error)
	 * or when pseudo_tcp_socket_get_next_clock fails (clean disconnection)
	 */
	SocketStatus status;
	PseudoTcpSocket *sck;
	guint64 timeout;
	std::chrono::steady_clock::time_point timeRef;  ///< used for internal timer

private:
	bool connect();
	void disconnect(bool force);
};

#endif // PSEUDOTCPSCK_H
