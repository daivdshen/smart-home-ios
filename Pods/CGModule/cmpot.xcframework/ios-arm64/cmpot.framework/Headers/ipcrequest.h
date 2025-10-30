/**
 *
 * @file ipcrpcmessage.h
 *
 *
 * Copyright (C) 2017 Comelit Group SpA
 *
 */

#ifndef IPCREQUEST_H
#define IPCREQUEST_H

#include <cmpot/ipccommon.h>

namespace Comelit  {
namespace MPot     {
namespace IPC	   {

/**
 * The IPCRequest class handles a request from userspace to the ipc engine
 */
class IPCRequest
{

public:

	/**
	 * IPCRequest default constructor
	 *
	 * @param clientId mqtt client id, usually an unique identifier of the process
	 * @param serviceName service name
	 * @param method method name
	 * @param params json object containing request parameters
	 * @param cbk callback
	 * @param cbkArg an optional argument defined by the user that will be appended to the callback
	* @param timeout timeout of the request [ms]
	 */
	IPCRequest(const string &clientId, const string &serviceName, const string &method,
	           rapidjson::Document &params,
	           IPCAsyncResponseCbk cbk, void *cbkArg, int reqTimeout);
	           
	/**
	 * Get mid of mosquittopp library, may be used to check status of delivery to mqtt server
	 *
	 * @return reference to mid value
	 */
	int	&getMosId() { return mosId; }
	
	/**
	 * Check if request timeout is expired
	 * @return true if expired
	 */
	bool	isExpired();
	
	IPCAsyncResponseCbk &getCbk() { return cbk;    }
	
	void	*getCbkArg()      { return cbkArg;      }
	int	    getId()          { return seqId;       }
	string  getserviceName() { return serviceName; }
	string  getClientId()    { return clientId;    }
	string &getRawPayload()  { return payload;     }
	bool    isCompleted()    { return completed;   }
	/// Set request completed, used only for sync requests
	void    setCompleted()   { completed = true;   }
	
public:

	IPCAnswer				answer; ///< Request answer, compiled once answer is received
	
private:

	/**
	 * Get timeout value
	 * @return timeout value [ms]
	 */
	int	    getTimeout(void) { return timeout; }
	
private:

	std::string				serviceName; ///< service name
	std::string				method;	     ///< method do be called
	std::string				payload;	 ///< payload of the request
	string					clientId;	 ///< clientid the request is coming from
	IPCAsyncResponseCbk		cbk;		 ///< callback defined from userspace
	void					*cbkArg;		 ///< callback argument
	int						timeout;	 ///< current timeout of the message
	int						seqId;		 ///< unique sequence id of the message
	int						mosId;       ///< id assigned to the message by mosquitto publish function
	bool					completed;	 ///< request status
	std::chrono::steady_clock::time_point	timeRef;    ///< used for internal timer
};


} // namespace IPC
} // namespace MPot
} // namespace Comelit

#endif // IPCREQUEST_H
