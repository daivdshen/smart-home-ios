/**
 *
 * @file ipcrpcmessage.h
 *
 * Copyright (C) 2017 Comelit Group SpA
 */

#ifndef IPCRPCMESSAGE_H
#define IPCRPCMESSAGE_H

#include <cmpot/ipccommon.h>

namespace Comelit  {
namespace MPot     {
namespace IPC	   {

/**
 * IPCRPCMessage class handles a jsonrpc request.
 * According to specifications
 * http://www.jsonrpc.org/specification#request_object
 */
class IPCRPCMessage
{
public:

	IPCRPCMessage();
	IPCRPCMessage(const IPCRPCMessage &);
	
	/**
	 * Get the jsonrpc 2.0 string from the message.
	 */
	string toJsonString();
	
	/**
	 * Create IPCRPCMessage object from a valid json string payload.
	 *
	 * @param[in] payload
	 * @return IPCRPCMessage
	 */
	static IPCRPCMessage createFromString(const char *payload);
	
	/**
	 * Copy operator
	 *
	 * @param other
	 * @return IPCRPCMessage &operator
	 */
	IPCRPCMessage &operator=(const IPCRPCMessage &other);
	
	string              serviceName;  ///< service name
	string              method;       ///< method name
	int
	id;           ///< message ID (required for requests, optional for notifications)
	rapidjson::Document params;       ///< json parameters object
	string              clientId;     ///< mqtt clientID
	JsonRPCErrCode      err;          ///< error code
	bool                completed;    ///< set it true when message can be safely deleted
	/// timeout of the message if asyncronous, should be the same of IPCRequest timeout,
	/// but since from server side it is impossible to know the timeout of the client it is set to 10 seconds
	int                 timeout;
};

} // namespace IPC
} // namespace MPot
} // namespace Comelit

#endif // IPCRPCMESSAGE_H
