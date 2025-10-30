/**
 *
 * @file registerer.h
 *
 * Registerer class: performs registration-related
 * operations on Comelit Cloud services
 *
 * Copyright (C) 2016 Comelit Group SpA
 *
 */

#ifndef REGISTERER_H
#define REGISTERER_H

#include <fstream>
#include <functional>
#include <string>
#include <cstring>
#include <thread>
#include <map>
#include <vector>
#include <chrono>

#include <cloud/cloudrequest.h>

#include <cloud/httpclient.h>

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

namespace Comelit {
namespace Nebula  {

using std::map;
using std::string;
using std::thread;
using std::vector;

/**
 * @brief The REDReqData struct contains all data required to
 * register a device on Comelit Cloud registration service (RED)
 *
 * @note: all fields marked with a (*) are mandatory and they are used by Cloud
 *        to univoquely identify a device. Changing some of them in a new/successive
 *        registration attempt (after the preious one succeeded, of course) will
 *        trigger an error from the registration service.
 */
struct REDReqData
{
	string         id32;      ///< ID32 unique device identifier
	string         deviceId;  ///< device identifier string (*)
	string         modelId;   ///< device model identifier string (*)
	string         swVersion; ///< software version (*)
	string         hwVersion; ///< hardware revision/version (*)
	string         serNumber; ///< serial number (device own unique identifier string) (*)
	string         ipAddress; ///< device IP address (*)
	string         secondId;  ///< secondary/sub device identifier (if any)
	string         mac;       ///< mac address to register on cloud
	string         mac2;      ///< mac address to register on cloud
};

/**
 * @brief The REDReqData struct contains the response from
 * Comelit Cloud registration service (RED) after a registration
 * attempt
 */
struct REDResData
{
	REDResData()
	{
		reset();
		retry = 0;
	}
	
	void reset()
	{
		requuid = "";
		uuid    = "";
		secret  = "";
		error   = 0;
		errmsg  = "";
		timeRef = std::chrono::steady_clock::now();
	}
	
	string requuid; ///< same request identifier if present in the original request
	string uuid;    ///< uuid of the caller; empty on error
	string secret;  ///< secret code supplied to the caller, to be used in conjunction with the uuid to get the token
	int    error;   ///< error code for the operation
	string errmsg;  ///< error message/extended description string
	uint   retry;    ///< Number of retries after the last successfull request
	/// timeref of the last registration retry
	std::chrono::steady_clock::time_point timeRef;
};

/**
 * @brief The RTKResData struct contains the response from
 * Comelit Cloud token service (RTK) after a token request
 * attempt
 */
struct RTKResData
{
	RTKResData() { reset(); }
	
	void reset()
	{
		requuid		= "";
		uuid		= "";
		token		= "";
		error		= 0;
		errmsg		= "";
		ttltoken	= 0;
		expireTime	= 0;
		retry		= 0;
	}
	
	string requuid;  ///< same request identifier if present in the original request
	string uuid;     ///< uuid of the caller; empty for responses with error
	string token;    ///< new token for the device; empty on error
	int    ttltoken; ///< expiration time (TTL) in seconds for the token; 0 on error
	time_t expireTime; ///< expiration time (datetime) for the token
	int    error;    ///< error code for the operation
	string errmsg;   ///< error message/extended description string
	uint   retry;    ///< Number of retries after the last successfull request
};

/**
 * @brief The ID32ResData struct contains the response from
 * Comelit Cloud after a ID32 request
 */
struct ID32ResData
{
	ID32ResData()
	{
		reset();
		retry = 0;
	}

	void reset()
	{
		requuid = "";
		id32    = "";
		error   = 0;
		errmsg  = "";
		timeRef = std::chrono::steady_clock::now();
	}

	string requuid; ///< same request identifier if present in the original request
	string id32;    ///< id32 value if present
	int    error;   ///< error code for the operation
	string errmsg;  ///< error message/extended description string
	uint   retry;   ///< Number of retries after the last successfull request
	/// timeref of the last attempt
	std::chrono::steady_clock::time_point timeRef;
};

/// Prototype for the function callback to be invoken on a registration-related event
typedef std::function<void (bool success, REDResData results)> OnRegistrationEvent;

/// Prototype for the function callback to be invoken on a token-related event
typedef std::function<void (bool success, RTKResData results)> OnTokenChangeEvent;

/// Prototype for the function callback to be invoken on a ID32 Change event
typedef std::function<void (bool success, const std::string &id32value)> OnId32AvailableEvent;

/**
 * @brief The Registerer class performs two important operations on Comelit Cloud services:
 * 1. registers the device on Comelit Cloud RED service, obtaining data used to get the token
 *    code required for perfoming operations on other Cloud services. This operation should happen
 *    just once, on the first device bootup, then all returned data is saved on a successful
 *    registration and can be used to obtain token when needed.
 *    This class also provides a way to trigger a new registration, if necessary.
 *
 * 2. periodically obtain a new token code. This is special code assigned to the registered device
 *    and allowing it to perform operations on other Clous services. This code has an expiration time
 *    (TTL) so it must be periodically refreshed. This class takes care of this, periodically
 *    performing token requests but provides also a way to trigger a new token request if necessary.
 *
 * So the normal operations flow is: register once the device on RED service then use the returned data
 * to periodically request a new token from RTK service. A valid token allows to perform operations
 * on other Cloud services.
 *
 * Usage:
 * 1. Create instance with data required for cloud registration
 * 2. Optionally call setRegistrationUserData to set uuid and secret if already available
 * 3. Call start(), this will initialize engine and perform cloud registration if uuid and
 *    secret are not provided by user
 * 4. Call run() periodically, this function perform token refresh automatically, so it doesn't need to
 *    be run with a short period.
 * 5. stop() engine if running when exiting.
 */
class Registerer
{
public:

	typedef enum
	{
		TOKEN_OK,
		TOKEN_EXPIRED,
		TOKEN_NOTFOUND
	} TokenStatus;
	
public:

	/**
	 * @brief Registerer Class constructor. This class registers the device on Comelit Cloud server and
	 *        periodically obtain a new token used for operations on Cloud services.
	 * @param[in] id32 device unique identifier
	 * @param[in] deviceId Comelit device identifier
	 * @param[in] modelId Comelit model identifier
	 * @param[in] swVersion software version
	 * @param[in] hwVersion hardware revision/version
	 * @param[in] serCode unique identifier for the device
	 * @param[in] ip current device IP, usually the one used for communicating with the Cloud
	 * @param[in] secondId Comelit secondary (non mandatory) identifier
	 * @param[in] mac address to register on cloud
	 */
	Registerer(string id32, string deviceId, string modelId, string swVersion,
	           string hwVersion, string serCode, string ip, string secondId, string mac, string mac2);
	           
	/**
	 * @brief Registerer Class constructor. This class registers the device on Comelit Cloud server and
	 *        periodically obtain a new token used for operations on Cloud services.
	 * @param[in] request request struct, containing all data required for registering on Comelit Cloud (the same
	 *            parameters required by the other constructor)
	 */
	Registerer(const REDReqData &request);
	
	virtual ~Registerer();
	
	/**
	 * @brief setRegistrationData Change internal registration parameters at any time.
	 * @param[in] id32 device unique identifier
	 * @param[in] deviceId Comelit device identifier
	 * @param[in] modelId Comelit model identifier
	 * @param[in] swVersion software version
	 * @param[in] hwVersion hardware revision/version
	 * @param[in] serCode unique identifier for the device
	 * @param[in] ip current device IP, usually the one used for communicating with the Cloud
	 * @param[in] secondId Comelit secondary (non mandatory) identifier
	 * @param[in] mac address to register on cloud
	 * @param[in] mac2 second mac address to register to cloud
	 */
	void setRegistrationData(string id32, string deviceId, string modelId, std::string swVersion, string hwVersion,
	                         string serCode, string ip, string secondId, string mac, string mac2);
	                         
	/**
	 * @brief setRegistrationUserData set uuid and secret to be used on token requests
	 * @param[in] uuid
	 * @param[in] secret
	 */
	void setRegistrationUserData(const string uuid, const string secret);
	
	/**
	 * @brief setOnRegistrationEvent set the callback invoked when a new registration-related event occurs
	 * @param[in] callback callback function to be invoked on a new registration event
	 */
	void setOnRegistrationEvent(OnRegistrationEvent &&callback);
	
	/**
	 * @brief setOnTokenChangeEvent set the callback invoked when a new token-related event occurs
	 * @param[in] callback callback function to be invoken on a new token-related event
	 */
	void setOnTokenChangeEvent(OnTokenChangeEvent &&callback);

	/**
	 * @brief setOnId32ChangeEvent set the callback invoked when a new Id32 is available.
	 * @param[in] callback callback function to be invoken on a new Id32 event
	 */
	void setOnId32AvailableEvent(OnId32AvailableEvent &&callback);
	
	/**
	 * Start the registered engine
	 * after it has been started call frequently run() to maintain
	 * the device registered on Comelit Cloud and periodically
	 * obtaining a new token to be used for Cloud service operations
	 * @return
	 */
	bool start();
	
	/**
	 * Run main cycle, this function must be continously called to let registration engine work
	 * maintaining the device registered on Comelit Cloud and periodically
	 * obtaining a new token to be used for Cloud service operations
	 */
	virtual void run();
	
	/**
	 * Stop engine, if running
	 */
	void stop();
	
	/**
	 * @brief isRegistered return device registration status on Comelit Cloud
	 * @return true if the device is registered thus has valid uuid and secret codes; false otherwise
	 */
	bool isRegistered();
	
	/**
	 * @brief getToken get the last retrieved token, if any
	 * @param[out] err TokenStatus result
	 * @return token or empty string
	 */
	string getToken(int &err);
	
	/**
	 * @brief forceNewRegistration trigger a new registration on Comelit Cloud.
	 */
	void forceNewRegistration();
	
	/**
	 * @brief forceNewToken trigger a new token request on Comelit Cloud.
	 */
	void forceNewToken();

	/**
	 * @brief invalidateToken invalidate current token and request a new one on Comelit Cloud
	 */
	void invalidateToken();
	
	/**
	 * @brief getREDLastOpStatus return the status for the last operation involving the registration service (RED)
	 * @return the status for the last operation performed on RED service
	 */
	CloudSvcLastOpStatus getREDLastOpStatus() { return redLastOpStatus; }
	
	/**
	 * @brief getREDLastOpStatus return the status for the last operation involving the token request service (RTK)
	 * @return the status for the last operation performed on RTK service
	 */
	CloudSvcLastOpStatus getRTKLastOpStatus() { return rtkLastOpStatus; }
	
	/**
	 * Set token data
	 * @param[in] tkn
	 * @param[in] expireTime expressed in utc time_t
	 */
	void setTokenData(string tkn, time_t expireTime);
	
	/**
	 * Get remaining time in seconds before token expiration
	 * @return 0 if token is already expired
	 */
	double getRemainingTime();
	
	/**
	 * Process cloud "eps/rtk" response
	 * Process cloud "eps/rtk" response and act accordingly
	 * @param[in] cresponse cloud response
	 */
	void   processRTKResponse(CloudAnswer *response);

	/**
	 * Process cloud "eps/newRedIdV2" response and act accordingly
	 * @param[in] cresponse cloud response
	 */
	void   processID32Response(CloudAnswer *response);
	
	/**
	 * Process cloud "eps/red" response
	 * Process cloud "eps/red" response and act accordingly
	 * @param[in] cresponse cloud response
	 */
	void   processREDResponse(CloudAnswer *response);
	
	const std::string getRegistrationUuid();

	const std::string &getRegistrationSecret();

	/**
	 * @brief getModelId
	 * @return the device ModelID for registration
	 */
	const std::string &getModelId() const {return regRequest.modelId;}

	/**
	 * @brief getSerialCode
	 * @return the device Serial Code for registration
	 */
	const std::string &getSerialCode() const {return regRequest.serNumber;}

protected:

	typedef enum
	{
		IDLE,
		TO_REGISTER,
		REGISTER_RETRY,
		REGISTERING,
		TOKEN_REFRESH,
		TOKEN_REFRESHING,
		ID32_REQUEST,
		ID32_RETRY,
		ID32_WAITING
	} Status;
	
private:

	virtual bool registerDevice();

	/// Request a new token to cloud and set ttl accordingly
	virtual bool refreshToken();

	bool requestId32();
	
	string makeREDLogin();
	
	string makeREDRequest();
	
	string makeRTKLogin();
	
	string makeID32Request();
protected:

	bool
	keepOn;                  ///< running status for this object: set to false to stop it
	REDReqData
	regRequest;              ///< RED request object: store all data required for a (new) registration on Comelit Cloud
	REDResData
	regData;                 ///< RED data object: store all returned by Comelit Cloud after the last registration attempt
	RTKResData
	rtkResponse;             ///< RTK data object: store all returned by Comelit Cloud after the last token request attempt
	ID32ResData
	id32Response;
	OnRegistrationEvent
	onRegistrationEvent;     ///< callback function to be invoked on a registration-related event
	OnTokenChangeEvent
	onTokenChangeEvent;      ///< callback function to be invoked on a token-related event
	const string         tokenPath;               ///< path on disk to retrieve the updated token
	CloudSvcLastOpStatus
	redLastOpStatus;         ///< status of the last operation performed on the Cloud registration service (RED)
	CloudSvcLastOpStatus
	rtkLastOpStatus;         ///< status of the last operation performed on the Cloud token request service (RTK)
	OnId32AvailableEvent
	onId32AvailableEvent;    ///< callback function to be invoked on ID32 change event
	std::mutex           runMutex;
	Status               status;
};

} // Nebula  NS
} // Comelit NS

#endif // REGISTERER_H
