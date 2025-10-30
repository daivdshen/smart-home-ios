#ifndef MQTTSERVICELOOKUP_H
#define MQTTSERVICELOOKUP_H

#include <cloudnext/cloudv2request.h>

namespace Comelit {
namespace Nebula  {

#define MQTTSLK_SLK_NAME "mqttc2_1_0"

/**
 * @brief The MqttSlkData struct
 * Structure used to store Mqtt Slk informations
 */
struct MqttSlkData
{
	std::string clusterId;
	std::string brokerId;
	std::string url;
	long         keepAlive;
	long         appPing;
	long         maxmsgSize;
	std::string type;
	bool        lastWillAllowed;
	bool        persSessionAllowed;
	bool        persMessageAllowed;
	bool        forceRestart;

	void reset()
	{
		clusterId = "";
		brokerId  = "";
		url       = "";
		keepAlive = 0;
		appPing   = 0;
		maxmsgSize = 0;
		type = "";
		lastWillAllowed = false;
		persSessionAllowed = false;
		persMessageAllowed = false;
		forceRestart = false;
	}

	bool isValid() const
	{
		//For the Slk to be valid, url, clusterId and brokerId
		//cannot be empty
		return (!(clusterId.empty() ||
		          brokerId.empty()  ||
		          url.empty()));
	}
};

typedef std::function<void (const MqttSlkData &data, time_t expireTime)>
OnMqttSlkChangeEvent;

/**
 * @brief The MqttServiceLookup class
 * is responsable to Update the Mqtt Service Parameters for the device
 * MqttServiceLookup retrieve Mqtt Parameters from Cloud via mqtt slk request.
 * Upon data expiration, MqttServiceLookup update the data via
 * service lookup request to Cloud.
 * For each device talking with cloud, only one single Mqtt instance
 * should connect with Cloud mqtt broker, therefore one single service lookup is needed
 */
class MqttServiceLookup : public ICloudRequestHandler
{
public:

	typedef enum
	{
		EPS_OK,
		EPS_EXPIRED,
		EPS_NOTFOUND
	} EpsStatus;

public:
	/**
	 * @brief MqttServiceLookup
	 * default constructor
	 */
	MqttServiceLookup();

	/**
	 * @brief getMqttSlkData
	 * @return
	 */
	const MqttSlkData &getMqttSlkData();

	/**
	 * @brief getEpsStatus
	 * @return
	 */
	EpsStatus getEpsStatus();

	/**
	 * Run function
	 * Require a valid token to optionally perform an
	 * eps refresh
	 * @param[in] token
	 */
	void run(const std::string &token);

	/**
	 * Set valid Mqtt data
	 * @param data
	 * @param epsExpireTime
	 */
	void setMqttSlkData(const MqttSlkData &data, time_t expireTime);

	/**
	 * Set valid Mqtt data
	 * @param data
	 * @param epsExpireTime
	 */
	void setMqttSlkDefaultData(const MqttSlkData &data);

	/**
	 * @brief setOnEpsChangeEvent
	 * setup a callback to be executed when Mqtt Slk data are updated
	 * @param cbk
	 */
	void setOnEpsChangeEvent(OnMqttSlkChangeEvent cbk) { onMqttSlkChangeEvent = cbk;}

	/**
	 * @brief setAutoUpdate
	 * Enable eps update on ttl expiration
	 * by default this is enable for Mqtt Slk
	 * @param enable
	 */
	void setAutoUpdate(bool enable) { update = enable; }

	/**
	 * @brief invalidateSlk
	 * invalidate current Slk
	 */
	void invalidateSlk();

	/**
	 * @brief forceExpire
	 * force expiration of current slk
	 */
	void forceExpire();

	/**
	 * @brief parseSlkJsonData
	 * parse a JSON containing mqtt slk data from comelit cloud
	 * @param[in] body json body
	 * @param[out] cloudData mqtt slk data derived from json
	 * @param[out] ttl expiration ttl of the mqtt slk
	 * @return true if parsing can be successfully completed
	 */
	static bool parseSlkJsonData(const JSON::JsonType &body, MqttSlkData &cloudData, time_t &expire);

	/**
	 * Get remaining time in seconds before expiration
	 * @return 0 if already expired
	 */
	double getRemainingTime();

	/**
	 * @brief forceNewSlk trigger a new slk request on Comelit Cloud.
	 */
	void forceNewSlk();
private:

	typedef enum
	{
		IDLE,
		EPS_REFRESH,
		EPS_REFRESHING
	} Status;

	std::mutex           mqttslkmutex;
	MqttSlkData          defaultdata;
	MqttSlkData          mqttSlkData;
	time_t               expireTime;              ///< expiration time (datetime utc)
	OnMqttSlkChangeEvent onMqttSlkChangeEvent;    ///< callback to notify Slk updates
	Status               status;
	uint                 retry;
	void                 *curCloudRequest;
	bool                 update; ///< true if eps sync on expiration is enabled

private:
	/**
	 * Get updated eps from cloud
	 * @param[in] token valid token to be used for cloud authentication
	 * @return true if eps correctly update
	 */
	bool   refreshSlk(const std::string &token);

	/**
	 * @brief processSLKResponse
	 * process Cloud answerto Mqtt Slk request
	 * @param response
	 */
	void   processSLKResponse(CloudV2Answer const *response);
};

extern MqttServiceLookup *mqttServiceLookup;

} // Nebula  NS
} // Comelit NS
#endif //MQTTSERVICELOOKUP_H
