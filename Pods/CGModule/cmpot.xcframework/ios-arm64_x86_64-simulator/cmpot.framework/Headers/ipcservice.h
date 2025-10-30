/**
 *
 * @file ipcmosquittomessage.h
 *
 *
 * Copyright (C) 2017 Comelit Group SpA
 *
 */

#ifndef IPCSERVICE_H
#define IPCSERVICE_H

#include <cmpot/ipccommon.h>

namespace Comelit  {
namespace MPot     {
namespace IPC	   {

/**
 * The IPCService class handles a service and his callback
 */
class IPCService
{
public:

	/**
	 * IPCService default constructor
	 * @param[in] serviceName service name
	 * @param[in] cbk callback
	 */
	IPCService(std::string serviceName, CallbackOnMessageProvider cbk) :
		cbkSrv(cbk), serviceName(serviceName), subscribed(false), lastMid(-1)
	{}
	
	IPCService(std::string serviceName, CallbackOnMessageListener cbk) :
		cbkLst(cbk), serviceName(serviceName), subscribed(false), lastMid(-1)
	{}

	IPCService(std::string serviceName, CallbackOnMessageSniffed cbk) :
		cbkSnf(cbk), serviceName(serviceName), subscribed(false), lastMid(-1)
	{}

	IPCService(std::string serviceName) :
		serviceName(serviceName), subscribed(false), lastMid(-1)
	{}
	
	/**
	 * Compare operator
	 *
	 * @param other
	 * @return bool operator
	 */
	bool operator==(const IPCService &other) const;
	
	/**
	 * Compare operator
	 *
	 * @param other
	 * @return bool operator
	 */
	bool operator!=(const IPCService &other) const;

	/**
	 * Compare operator
	 *
	 * @param other
	 * @return bool operator
	 */
	bool operator<(const IPCService &other) const;
	
	CallbackOnMessageProvider    cbkSrv;		 ///< user defined callback
	CallbackOnMessageListener    cbkLst;		 ///< user defined callback
	CallbackOnMessageSniffed     cbkSnf;		 ///< user defined callback
	string                  serviceName; ///< service name
	bool        subscribed; ///< subscription status
	int         lastMid;    ///< mid of the last subscription request on this topic
	
};

} // namespace IPC
} // namespace MPot
} // namespace Comelit

#endif // IPCSERVICE_H
