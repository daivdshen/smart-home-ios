#ifndef CLOUDTYPES_H
#define CLOUDTYPES_H
#include <string>
#include <functional>

namespace Comelit {
namespace Nebula  {

enum CloudErrCode
{
	CCAPI_ERR_OK                   =  0,
	CCAPI_ERR_GENERIC_EXC          = -1,
	CCAPI_ERR_PERMISSION_DENIED    =  10,
	CCAPI_ERR_TOKEN_NOT_VALID      =  30,
	CCAPI_ERR_TOKEN_EXPIRED        =  31,
	CCAPI_ERR_AUTHENTICATION_ERROR =  50,
	CCAPI_ERR_INSERT_DATA          =  1005,
	CCAPI_ERR_MISSING_ID           =  1006,
	CCAPI_ERR_MISSING_PID          =  1007,
	CCAPI_ERR_MAC_EXISTS           =  1008,
	CCAPI_ERR_SECRET_MANDATORY     =  1010,
	CCAPI_ERR_SECRET_NOT_VALID     =  1011,
	CCAPI_ERR_UUID_NOT_VALID       =  1012,
	CCAPI_ERR_GET_EPS_SERVER       =  1018,
	CCAPI_ERR_ID_DECRYPT_FAIL      =  1019,
	CCAPI_ERR_DEVICE_EXISTS        =  1020,
	CCAPI_ERR_MAX_REGS_REACHED     =  1022,
	CCAPI_ERR_DEVICE_UNREGISTERED  =  1023,
	//facerecognition Errors 16000
	CCAPI_ERR_FACERECOGNITION_RESOURCE_NOT_FOUND                = 16000,
	CCAPI_ERR_FACERECOGNITION_INVALID_PARAMETER                 = 16001,
	CCAPI_ERR_FACERECOGNITION_PROVISIONIED_THROUGHPUT_EXCEEDED  = 16002,
	CCAPI_ERR_FACERECOGNITION_IMAGE_TOO_LARGE                   = 16003,
	CCAPI_ERR_FACERECOGNITION_INVALID_IMAGE_FORMAT              = 16004,
	CCAPI_ERR_FACERECOGNITION_INVALID_PAGINATION_TOKEN          = 16005,
	CCAPI_ERR_FACERECOGNITION_RESOURCE_ALREADY_EXISTS           = 16006,
	CCAPI_ERR_FACERECOGNITION_INVALID_S3_OBJECT                 = 16007,
	CCAPI_ERR_FACERECOGNITION_FACE_NOT_VALID                    = 16008,
	CCAPI_ERR_TREE_ELEMENT_NOT_FOUND = 18000,
	CCAPI_ERR_TREE_USER_ANOUTHORIZED = 18001,
	CCAPI_ERR_TREE_DEVICE_NOT_LINKED = 18002
};

enum CloudErrorStep
{
	NOT_STARTED = -10, ///< doRequest() not called yet
	CONNECTION  =  -3, ///< Cannot connect to cloud server
	PARSING     =  -2, ///< Obtained a response from server that failed parsing
	CLOUD       =  -1, ///< Response from server has "error" != 0
	NO_ERROR    =   0  ///< Server returned a valid answer with "error" == 0
};

namespace CloudErrCodeV2 {
static const std::string ERR_TOKEN_NOT_VALID                = "30";
static const std::string ERR_TOKEN_EXPIRED                  = "31";
static const std::string ERR_AUTHENTICATION_ERROR           = "50";
static const std::string ERR_APPMANAGER_EXCEPTION           = "ApplicationManagedException";
static const std::string ERR_DEVICE_TIMEOUT_EXCEPTION       = "DeviceTimeoutException";
static const std::string ERR_DEVICE_NOT_FOUND_EXCEPTION     = "DeviceNotFoundException";
static const std::string ERR_DEVICE_NOT_CONNECTED_EXCEPTION = "NotConnectedDeviceException";
static const std::string ERR_BAD_REQUEST_EXCEPTION          = "BadRequestException";
static const std::string ERR_CONSTRAINT_VIOLATION_EXCEPTION = "ConstraintViolationException";
static const std::string ERR_BAD_DEVICE_RESPONSE_EXCEPTION  = "BAD_DEVICE_RESPONSE";
}

struct ICloudAnswer
{
	union
	{
		CloudErrCode   cloudErrorCode;
		int            intErrorCode;
	};

	CloudErrorStep   errorStep;
	std::string      errString;

	/**
	 * @brief isOK
	 * @return true if answer has been properly processed
	 * AND the Cloud error code is OK
	 */
	bool isOK() const { return errorStep == 0; }

	/**
	 * @brief getReqToken
	 * Interface to retrieve the Token used for the
	 * corresponding request
	 * @return
	 */
	virtual const std::string getReqToken() = 0;

protected:
	/**
	 * @brief ICloudAnswer
	 * default constructor, initialize values
	 */
	ICloudAnswer() : cloudErrorCode(CCAPI_ERR_GENERIC_EXC), errorStep(NOT_STARTED) {}

	virtual ~ICloudAnswer() {}
};


/**
 * @brief The ICloudRequestHandler class
 * Generic Cloud Request handler interface.
 * Represents a class that communicates with cloud via cloud request/answers
 * Allow the possibility to register a callback to be executed upon Cloud errors
 */
class ICloudRequestHandler
{
public:
	/// Prototype for the function callback to be invoken on a registration-related event
	typedef std::function<void (int errorStep, CloudErrCode errorCode, ICloudAnswer* response)> OnErrorEvent;

	/**
	* @brief setOnErrorEvent
	* setup a callback to be executed when Cloud Request fails
	* @param cbk
	*/
	void setOnErrorEvent(OnErrorEvent cbk)
	{
		onErrorEvent = cbk;
	}

protected:
	/**
	 * @brief CloudRequestHandler
	 * default constructor, protected
	 */
	ICloudRequestHandler() : onErrorEvent(nullptr) { }

	/**
	 * @brief handleErrorEvent
	 * check if a cloud
	 * @param errorStep
	 * @param errorCode
	 * @param response
	 */
	void handleErrorEvent(int errorStep, CloudErrCode errorCode, ICloudAnswer* response)
	{
		if (onErrorEvent != nullptr)
		{
			onErrorEvent(errorStep, errorCode, response);
		}
	}

	OnErrorEvent onErrorEvent;
};

} // Comelit
} // Nebula

#endif
