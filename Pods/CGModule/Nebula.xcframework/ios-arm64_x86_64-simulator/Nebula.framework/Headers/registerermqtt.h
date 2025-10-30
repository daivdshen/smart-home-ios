#ifndef REGISTERERMQTT_H
#define REGISTERERMQTT_H

#include <nebula/mqttservicelookup.h>
#include <nebula/registerer.h>
#include <cloudnext/cloudv2request.h>

namespace Comelit {
namespace Nebula  {

/**
 * @brief The RegistererMqtt class implements the same functions of the Regieterer Class.
 * The only difference is that RegistererMqtt communicates with Cloud via Mqtt Protocol (instead of HTTP)
 * and uses CCAPI v2 protocol.
 * Since the logic of this class is exactly the same of Registerer Class,
 * only the methods which include communication with Cloud are different.
 * See description of Registerer Class for details about usage
 */
class RegistererMqtt : public Registerer
{
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
	RegistererMqtt(const REDReqData &regData);

	/**
	 * @brief getMqttRegData
	 * retrieve Mqtt Slk Data received within Registration reply
	 * with CCAPI v2 protocol, when the registration Reply contains also some mqtt Slk information.
	 * Data are valid only when registration is executed.
	 * @param[out] mqttdata mqtt slk data
	 * @param[out] expireTime mqtt slk expire time
	 */
	void getMqttRegData(MqttSlkData &mqttdata, time_t& expireTime);

	void run() override;

private:

	bool registerDevice() override;

	bool refreshToken() override;

	void processREDResponse(CloudV2Answer const *response);

	void processRTKResponse(CloudV2Answer const *response);

	void makeREDRequestBody(CloudV2Request &request);

private:
	MqttSlkData mqttRegData;
	time_t mqttDataExpire;
};

} // Nebula  NS
} // Comelit NS

#endif //REGISTERERMQTT_H
