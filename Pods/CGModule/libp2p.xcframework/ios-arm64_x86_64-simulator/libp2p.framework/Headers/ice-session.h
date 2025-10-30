/**
 * @file   ice-session.h
 *
 * Handles a UDP/TCP connection towards a MQTT/HTTP server for receiving and publishing
 * messages. It exports the function for creating ICE connections using TCP
 * or UDP.
 *
 * @author Roberto Muzzì
 */

#ifndef ICE_SESSION_H
#define ICE_SESSION_H

#include <vector>
#include <string>
#include <list>
#include <set>
#include <unordered_set>
#include <mutex>
#include <deque>

#include "p2pconfig.h"
#include "mqtt-client.h"
#include "pjsip-client.h"
#if defined(LIBP2P_ENABLE_HTTPCLIENT)
#include "rest-client.h"
#endif
#if defined(LIBP2P_ENABLE_DTLS)
#include "dtls.h"
#endif

class IceSession;
class RestClient; ///< forward declaration

/// List of possible event types
typedef enum
{
	IMS_EVENT_TYPE_MQTT=0, ///< event from mqtt module
	IMS_EVENT_TYPE_ICE,    ///< event from ice  module
	IMS_EVENT_TYPE_SDP,    ///< event from sdp
	IMS_EVENT_TYPE_HTTP,   ///< event from http module
	IMS_EVENT_TYPE_NO
} iceMqttEventTypes_t;

typedef enum
{
	ICE_EVENT_LINE_INIT = 0,
	ICE_EVENT_LINE_ESTABLISHED,
	ICE_EVENT_LINE_CLOSE,
	ICE_EVENT_LINE_ERR,
	ICE_EVENT_LINE_NO
} iceEventTypes_t;

typedef enum
{
	HTTP_REST_OK = 0,
	HTTP_REST_AUTH_ERROR,
	HTTP_REST_DEVICE_NOT_CONNECTED,
	HTTP_REST_BAD_RESPONSE,
	HTTP_REST_DEVICE_TIMEOUT,
	HTTP_REST_DEVICE_NOT_FOUND,
	HTTP_REST_UNKNOWN_ERROR,
	HTTP_REST_NO
} httpEventTypes_t;

typedef enum
{
	IMS_SUCCESS = 0,
	IMS_FAILED
} iceMqttOperationStatus_t;

typedef enum
{
	ICE_FILTER_STRATEGY_NONE = 0, ///< use all the remote candidates
	ICE_FILTER_STRATEGY_ONLY_PEER, ///< use only remote peer candidates
	ICE_FILTER_STRATEGY_ONLY_RELAYED ///< use only remote relayed candidates
} iceCandidatesFilterStrategy_t;

/// Event data structure
typedef struct
{
	uint8_t type;  ///< type ::iceMqttEventTypes_t
	uint8_t id;    ///< event index
	uint8_t error; ///< result
	uint8_t flags; ///< only for ice event types (specifies tcp or udp)
	char   *err_str; ///< error string if any
	int     sub;   ///< index of ice client
} iceMqttEventDataStruct_t;

/// Contains the data received from ICE module
typedef struct
{
	const char *data;  ///< pointer to data received
	int   len;   ///< lenght data received
	int   flags; ///< only for ice event types (specifies tcp or udp)
	int   sub;   ///< index of ice client
} iceMqttReceiveDataStruct_t;

typedef void (*ice_mqtt_event_cbk_t)(IceSession *sender, void *arg,
                                     iceMqttEventDataStruct_t *event);
typedef void (*ice_mqtt_receive_data_cbk_t)(IceSession *sender, void *arg,
                                            iceMqttReceiveDataStruct_t *data);


class SdpInfo
{
public:
	SdpInfo(const char *sdp, int sub);
	std::unique_ptr<char[]> payload;
	int sub;
	int session;
	int role;
	bool isWebrtc;

	bool operator==(const SdpInfo &info)
	{
		return (sub == info.sub);
	}
};


/**
 * Class IceSession
 */
class IceSession
{

public:
	/**
	 * Mosquitto lib global function
	 * To be called on application space only if this is
	 * the only one library that use mosquitto_lib.
	 */
	static void mosquittoInit();
	static void mosquittoCleanup();
public:

	/**
	 * Constructor initializes class
	 */
	IceSession();

	/**
	 * IceSession destructor
	 */
	~IceSession(void);

	/**
	 * Initializes module
	 *
	 * @param role      role for TCP ICE ('o' controlling agent, 'a' controlled)
	 * @param name      arbitrary name used in MQTT connections
	 */
	void setInitParams(char role, char *name);

	/**
	 * @brief addClient
	 * add one ICE client to the list of possible clients
	 * @param subId generic client Id
	 * @param sub_topic topic where the client should receive SDPs
	 * @param pub_topic topic where the client should publish SDPs
	 */
	void addClient(int subId, const std::string &sub_topic, const std::string &pub_topic);

	/**
	 * @brief removeClient
	 * remove one ICE client to the list of possible clients
	 * @param subId  generic client Id
	 */
	void removeClient(int subId);

	/**
	 * Set the parameters which are related to the ICE communication
	 *
	 * @param stun_srv_addr1    main stun server address
	 * @param stun_srv_addr2    secondary stun server address (can be the same of @a stun_srv_addr1)
	 * @param turn_username     username for logging to turn server
	 * @param user_password     password for logging to turn server
	 * @param turn_srv_addr     turn server address
	 * @param strategy          strategy used to filter remote candidates
	 * @param is_ipv6_only      force the ipv6 address resolution
	 * @param rem_nego_delay    delay the start of udp and tcp negotitation, only used by the SLAVE to set the MASTER negotiation delay.
	 *                          The SLAVE role DO NOT use it, because today (18/07/2018) the start_nego_delayed is not thread_safe
	 */
	void setIceParameters(char *stun_srv_addr1, char *stun_srv_addr2, char *turn_username,
	                      char *user_password, char *turn_srv_addr,
	                      iceCandidatesFilterStrategy_t strategy = ICE_FILTER_STRATEGY_NONE, bool is_ipv6_only = false,
	                      int rem_nego_delay = 0);

	/**
	 * Runs active clients loop event should be called frequently.
	 */
	void run(void);

	/**
	 * Starts the Mqtt communication for publishing and receiving messages
	 *
	 * @param srv_name      mqtt server address
	 * @param sub_topics    topics to be registered to
	 * @param username      username for authentication
	 * @param password      password for authentication
	 * @param cert          certificate for secure connection
	 * @retval 0
	 */
	int startMqtt(char *srv_name, std::vector<std::string> sub_topics, char *username, char *password,
	              char *cert=0);

	int startMqtt(char *srv_name, char *sub_topic, char *username, char *password, char *cert=0);

	/**
	 * Stops the Mqtt communication (destroy mqtt client object)
	 */
	int stopMqtt();

	/**
	 * Starts the Rest communication client for publishing and receiving messages
	 * Note that it can be used only on client mode 'a'
	 *
	 * @param[in] endpoint url
	 * @param[in] ccsTkn
	 * @param[in] duuid device id
	 * @param[in] authentication mode
	 * @param[in] authentication token
	 * @return 0 on success
	 */
	int startHttp(const std::string &endpoint, const std::string &ccsTkn, const std::string &duuid,
	              const std::string &authMode, const std::string &viperTkn);

	/**
	 * Stops the HTTP communication (destroy rest client object)
	 */
	int stopHttp();

	/**
	 * Starts the ICE communication to establish a TCP connection with a relay server
	 * @param sub identifier of the object (links to the peer ViP subaddress)
	 * @retval 0  successfully started or already started
	 * @retval -1 not ready to start
	 */
	int startIceTcpSession(int sub);

	/**
	 * Starts the ICE communication to establish an UDP connection, it starts both
	 * a controller and controlling ICE agents in order to bypass asymmetric NAT problem.
	 * @param sub identifier of the object (links to the peer ViP subaddress)
	 *
	 * @retval 0  successfully started or already started
	 * @retval -1 not ready to start
	 */
	int startIceUdpSession(int sub);


	/**
	 * Starts the ICE communication to establish an UDP connection, it starts both
	 * a controller and controlling ICE agents in order to bypass asymmetric NAT problem.
	 *
	 * @param sub identifier of the object (links to the peer ViP subaddress)
	 *
	 * @retval 0  successfully started or already started
	 * @retval -1 not ready to start
	 */
	int startIceSession(int sub);


	/**
	 * Stops the ICE communication of a muxed (tcp+udp) session
	 *
	 * @param sub identifier of the object (links to the peer ViP subaddress)
	 * @retval 0
	 */
	int stopIceSession(int sub);


	/**
	 * Stops the TCP ICE communication with a relay server
	 * @param sub identifier of the object (links to the peer ViP subaddress)
	 * @retval 0
	 */
	int stopIceTcpSession(int sub);

	/**
	 * Stops the UDP ICE communication
	 * @param sub identifier of the object (links to the peer ViP subaddress)
	 *
	 * @retval 0
	 */
	int stopIceUdpSession(int sub);

	bool isIceClientRunning(int session, int sub);

	bool isIceConnectionEstablished(int session, int sub);

	bool iceClientExist(int session, int sub);

	/**
	 * Returns the IceClient pointer requested.
	 */
	IceClient  *getIceClient(int session, int sub);

	/**
	 * Returns the Mqtt session handler.
	 */
	MqttClient *getMqttClient();

	/**
	 * Returns the candidates information for the given session and sub
	 */
	ice_nominated_candidates getCandidatesInfo(int session, int sub);

	/**
	 * Send data to the peer connection via ICE protocol
	 *
	 * @param buf   message buffer
	 * @param len   message buffer length
	 * @param flags send on TCP or UDP ICE session
	 * @param sub   identifier of the ICE client
	 * @return pointer to the data packet or nullptr on failure
	 */
	std::shared_ptr<IceMessage> sendData(char *buf, int len, int flags, int sub);

	/**
	 * Send data to the peer connection via ICE protocol
	 * Typically used for retransmissions
	 * @param data message data
	 * @param flags send on TCP or UDP ICE session
	 * @param sub   identifier of the ICE client
	 * @return 0 on success -1 on error
	 */
	int sendData(std::shared_ptr<IceMessage> data, int flags, int sub);

	int setEventCbk(ice_mqtt_event_cbk_t cbk, void *arg);

	/**
	 * Sets the callback to receive data on an ICE connection.
	 * @param cbk   callback function
	 * @param arg   correlator callback
	 * @retval 0
	 */
	int setReceiveDataCbk(ice_mqtt_receive_data_cbk_t cbk, void *arg);

	/**
	 * Returns the callback to receive data on an ICE connection.
	 */
	ice_mqtt_receive_data_cbk_t  getReceiveDataCbk();

	/**
	 * Returns the callback argument associated to received data callback
	 */
	void                        *getReceiveDataCbkArg();

	/**
	 * Returns the publish topic associated with an IceClient identified by @a sub
	 * @param sub index of the IceCLient
	 * @return publish topic if found, emtpy string otherwise
	 */
	std::string getPubTopic(int sub);

	/**
	 * Parse the received sdp received and pass the information to the related
	 * Ice client. The Sdp contains info about the peer ICE candidates.
	 *
	 * @param sdp   buffer containg the received sdp
	 * @param len   len of sdp buffer
	 * @param topic mqtt topic on which the sdp is received
	 */
	void newSdpReceived(const char *sdp, int len, const char *topic);
	void handleSdpReceived(const SdpInfo &sdp);

	/**
	 * Callback function that handle events from ice clients and forwards them to user.
	 *
	 * @param cl     IceClient the event is related to
	 * @param op     transport operation phase the event is related to
	 * @param status status of the transport operation
	 */
	void handleIceClientStateChanged(void *cl, pj_ice_strans_op op, pj_status_t status);

	/// Utility function to create and enque events via registered callback
	void addEvent(uint8_t type, uint8_t ev, uint8_t err, char *str, int sess_id, int sub);

	/// Register internal thread to call pjsip methods
	void registerIceThread();

	/**
	 * Start a dedicate thread that handles events
	 */
	void loopStart();
	void loopStop();

#if defined(LIBP2P_ENABLE_DTLS)
	/**
	 * Enable and create Dtls context
	 * @return 0 on success
	 */
	int enableDtls();
#endif

private:

	/**
	 * Erase dead ice session
	 * @return number of session that are still alive
	 */
	int eraseDeadIceSessions();

	void stopSession(int session, int sub);

	/**
	 * @brief stopIceClient
	 * stop ice client, add to the list of clients that
	 * will be deleted by eraseDeadIceSessions()
	 * and reset the input iceCl pointer to null
	 * @param iceCl ice client to stop and reset
	 */
	void stopIceClient(IceClient **iceCl);

	/**
	 * @brief getSubTopic
	 * get the sub related to a specific topic
	 * @param topic
	 * @param[out] sub
	 * @return true if a sub exists for the requestsed topic
	 */
	bool getSubFromTopic(const char *topic, int &sub);

	std::vector<pj_ice_cand_type> getAllowedCandidateTypes();

#if defined(LIBP2P_ENABLE_DTLS)
	void handleDtlsEvent(IceClient *cli, DtlsSrtp::DtlsEvent evt);
#endif

private:

	char    m_role;     ///< role ('o' controlling agent, 'a' controlled)
	char    m_name[64]; ///< Base name for ICE/MQTT client, should be unique at server side

	char    m_stun_srv_addr1[128]; ///< STUN server address for UDP controlling agent
	char    m_stun_srv_addr2[128]; ///< STUN server address for UDP controlled agent
	char    m_turn_username[128];  ///< TURN server username
	char    m_turn_password[128];  ///< TURN server password
	char    m_turn_srv_addr[128];  ///< TURN server address

	bool    m_is_ipv6_only;            ///< flag to force the ipv6 address resolution
	int     m_rem_nego_delay;          ///< delay the start of udp and tcp negotitation

	iceCandidatesFilterStrategy_t     m_strategy;            ///< Filter strategy

	struct IceCliStr
	{
		IceClient   *tcp;       ///< Ice client for tcp connection
		IceClient   *udp;       ///< Ice Client for direct UDP connection
		IceClient   *cli;       ///< Ice Client for muxed tcp/udp connection
		std::string sub_topic;
		std::string pub_topic;

		IceCliStr() : tcp(nullptr), udp(nullptr), cli(nullptr)
		{
		}

		IceCliStr(const std::string &sub_topic, const std::string &pub_topic) :
			tcp(nullptr), udp(nullptr), cli(nullptr),
			sub_topic(sub_topic), pub_topic(pub_topic)
		{}
	};

	/// List ICE clients that need to be closed
	std::mutex             closeClsMtx;
	std::list<IceClient *> closeCls;

	/// Instance of MQTT client
	MqttClient  *mqttCl;
	/// Instance of REST client
	RestClient  *httpCl;

	ice_mqtt_event_cbk_t        event_cbk;
	ice_mqtt_receive_data_cbk_t receive_data_cbk;

	void *event_cbk_arg;
	void *receive_data_cbk_arg;

	std::mutex clMutex; ///< protects the list of active clients
	std::map<int, IceCliStr> iceClList; ///< list of active clients


	bool quitThread;
	std::thread *iceThread;

	/// ice thread descriptor object for pjsip library
	void *iceThreadDesc;

	std::list<SdpInfo> sdpRxList;
	std::mutex sdpMutex;

	std::deque<iceMqttEventDataStruct_t *> eventList;
	std::mutex eventListMutex;

	/// Instance of DTLS client
#if defined(LIBP2P_ENABLE_DTLS)
	Dtls  *dtlsCl {nullptr};
#endif

	bool destroy;
};

// psjsip thread registration function ======================================

/**
 * If auto registration is enabled, every function which internally calls
 * pjsip library will check if the calling thread is registered to pjsip.
 * If not it automatically registers it.
 * Autoregistration is enabled by default.
 *
 * @param on flag false to disable auto registration (every thread should be manually
 *                registered/deregistered using registerThread()/unregisterThread() functions
 */
extern void setAutoRegisterThreadMode(bool on);

/// Returns if autoamtic thread registration is enabled or not
extern bool isAutoRegisterThreadEnable();

/**
 * Registers the calling thread to pjsip library.
 *
 * @param name thread name
 * @return the internal descriptor object which should be used
 *         for thread unregistration
 */
extern void *registerThread(const char *name);

/// Unregisters the internal thread descriptor object
extern void unregisterThread(void *desc);

#endif  // ICE_SESSION_H


