/**
 *
 * @file ipccommon.h
 *
 *
 * Copyright (C) 2017 Comelit Group SpA
 *
 */

#ifndef IPCCOMMON_H
#define IPCCOMMON_H

#include <cmpot/common.h>
#include <cmpot/jsonutil.h>

#ifndef __clang__
#define GCC_VERSION (__GNUC__ * 10000 \
	                                 + __GNUC_MINOR__ * 100 \
	                                 + __GNUC_PATCHLEVEL__)

/* Check for GCC < 4.7.0 */
#if GCC_VERSION < 40700
namespace std    {
namespace chrono {
typedef monotonic_clock steady_clock;
}
}
#endif
#endif //not __clang__

namespace Comelit  {
namespace MPot     {
namespace IPC	   {

typedef void
*IPCRequestHandler; ///< Opaque type, pointer to a request handler (actually a pointer to IPCRequest)

/**
 * JsonRPC error codes.
 * Defined according to specifications:
 * http://www.jsonrpc.org/specification
 */
typedef enum
{
	/// This doesn't comes from jsonrpc specification, it is used internally
	JSONRPC_OK                       =  0,
	JSONRPC_PARSE_ERROR              = -32700,
	JSONRPC_INVALID_REQ              = -32600,
	JSONRPC_METHOD_NOT_FOUND         = -32601,
	JSONRPC_INVALID_PARAMS           = -32602,
	JSONRPC_INTERNAL_ERROR           = -32603
} JsonRPCErrCode;

/**
 * The IPCErrCode enum define possible errors on IPCManager calls
 */
typedef enum
{
	IPC_ERR_OK               =  0,
	IPC_ERR_TIMEOUT          =  1,
	IPC_ERR_OVERFLOW         =  2, ///< maxi size of queue (100) reached
	IPC_ERR_BUSY             =  3,
	IPC_ERR_EAGAIN           =  4, ///< try again (waiting for sync request to be completed)
	IPC_ERR_GENERIC_EXC      = -1,
} IPCErrCode;

/**
 * Handles a jsonrpc2.0 error object.
 * According to specification
 * http://www.jsonrpc.org/specification#error_object
 */
class RpcError
{
public:

	RpcError();
	RpcError(const RpcError &);
	
	RpcError &operator=(const RpcError &other);
	
	int                 code;    ///< error code
	string              message; ///< error string
	rapidjson::Document data;    ///< data object (optional)
};

/**
 * IPCRPCResponse class handles a jsonrpc response.
 * According to specifications
 * http://www.jsonrpc.org/specification#response_object
 */
class IPCRPCResponse
{
public:

	IPCRPCResponse();
	IPCRPCResponse(const IPCRPCResponse &);
	
	IPCRPCResponse &operator=(const IPCRPCResponse &other);
	
	/**
	 * Get the jsonrpc 2.0 string from the message
	 */
	string toJsonString();
	
	/**
	 * Delay the answer
	 * Note that a delayed answer may not container neither result or rpcError field
	 * since it has not to be send immediately.
	 *
	 * @param[in] timeout should be lower then the timeout set by the request
	 */
	void   delayAnswer(int timeout);
	
	/**
	 * Get timeout of the delayed answer
	 * @return timeout status (0 for not delayed answer)
	 */
	int    isDelayed();
	
	/**
	 * Create an IPCRPCResponse from a valid jsonrpc2.0 string payload
	 */
	static IPCRPCResponse createFromString(const string payload);
	
	int                  id;      ///< required, must be the same of the request
	rapidjson::Document
	result;  ///< json structure that must contain the answer if error is JSONRPC_OK
	
	/// json structure that must contain the error if error is not JSONRPC_OK
	RpcError             rpcError;
	
private:

	int                  timeout; ///< timeout of the delayed response
};

/**
 * The IPCAnswer class handles answers to an ipc request
 */
class IPCAnswer
{
public:

	IPCAnswer() : ipcErrorCode(IPC_ERR_GENERIC_EXC), errString("") {}
	IPCAnswer(const IPCAnswer &other);

	IPCAnswer &operator=(const IPCAnswer &other);
	
	IPCErrCode             ipcErrorCode; ///< ipc error
	std::string            errString;    ///< message associated to ipcErrorCode
	
	IPCRPCResponse         response;     ///< jsonRpc2.0 response
};


/**
 * IPCAsyncResponseCbk callback defined from userspace and called on request completed
 */
typedef std::function<void (void *obj, IPCAnswer *response, void *cbkArg, IPCRequestHandler req)>
IPCAsyncResponseCbk;

/// Callback defined from userspace, to be called whenever a message is received on this service
typedef std::function<void (void *obj, std::string method, rapidjson::Document &params, IPCRPCResponse *response, void *arg)>
CallbackOnMessageProvider;

/// Callback defined from userspace, to be called whenever a message is received on this service registered as listener
typedef std::function<void (void *obj, std::string method, rapidjson::Document &params)>
CallbackOnMessageListener;

/// Callback defined from userspace, to be called whenever a message is received on this service registered as sniffer
/// @note: NOT SUPPORTED synchronous ipc-request called by this callback (should be not needed, since sniffer should
/// capture only events and data)
typedef std::function<void (rapidjson::Document &messageJson)>
CallbackOnMessageSniffed;

} // namespace IPC
} // namespace MPot
} // namespace Comelit

#endif // IPCCOMMON_H
