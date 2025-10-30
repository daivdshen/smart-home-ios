/**
 * @file mqtt-client.h
 *
 */

#ifndef _MQTT_CLIENT_H
#define _MQTT_CLIENT_H

#include <map>
#include <vector>
#include <string>
#include <thread>
#include <mosquittopp.h>
#include <chrono>
#include <atomic>
#include <mutex>
#include <queue>

typedef enum
{
	MQTT_CONNECT = 0,
	MQTT_SUBSCRIBE,
	MQTT_PUBLISH,
	MQTT_MESSAGE,
	MQTT_DISCONNECT,
	MQTT_CONN_LOST,
	MQTT_ERROR
} MqttEventEnum_t;

/// Callback function to get messages on the subscribed topics
typedef void (*MqttClientRcvCbk)(void *arg, const char *buf, int len, const char *topic);
typedef void (*MqttClientEventCbk)(void *arg, MqttEventEnum_t ev, int res_code, char *errstr);


/// Class which handles the communication to an MQTT server
class MqttClient : private mosqpp::mosquittopp
{

public:

	/**
	 * Creates and initialize MQTT client instance, in order to activate
	 * the ::connect function should be called.
	 *
	 * @param sub_topics    list of topics to be subscribed to
	 * @param serverURI     address of the MQTT server
	 * @param clientId      name of the client
	 * @param username      username for authentication
	 * @param password      password for authentication
	 */
	MqttClient(std::vector<std::string> sub_topics, const char *serverURI, const char *clientId,
	           char *username, char *password);

	~MqttClient();

	/**
	 * Connects to MQTT server, the connection is established asyncronously.
	 *
	 * @param cert pointer to PEM file to be used to verify SSL connection to MQTT server
	 *
	 * @return 0 if connection started, -1 if an error occurred or connection already started
	 */
	int start(char *cert=0);

	/**
	 * Disconnect from MQTT server,the disconnection is perfomed asyncronously.
	 *
	 * @note wait disconnection callback before destrory the object
	 *
	 * @return 0 if disconnection started, -1 if an error occurred or disconnection already performed
	 */
	int stop();

	/**
	 * Replace the existing topics with a new list
	 *
	 * @param subTopics Topic list
	 */
	void replaceSubTopics(const std::vector<std::string> &subTopics);

	/**
	 * Add a new topic list
	 *
	 * @param subTopics Topic list
	 */
	void addSubTopics(const std::vector<std::string> &subTopics);

	/**
	 * Remove the topic list reported
	 *
	 * @param subTopics Topic list
	 */
	void removeSubTopics(const std::vector<std::string> &subTopics);

	/**
	 * Publish a message on a specific topic.
	 *
	 * @param buf   message to be published
	 * @param len   length of the message
	 * @param topic topic where publishing the message
	 * @param qos   quality of service (use 1 => at least one notification)
	 */
	int publishService(char *buf, int len, const char *topic, int qos);

	/**
	 * Adds a callback for a specific topic, if the topic already exists the
	 * previous callback is overwritten.
	 *
	 * @param topic name of the queue on server side where to receive messages
	 * @param cbk   callback function called when a message is received
	 * @param arg   callback correlator
	 */
	void setRcvCallback(const char *topic, MqttClientRcvCbk cbk, void *arg);

	/**
	 * Removes the callback associated to the topic
	 *
	 * @param topic topic to be removed
	 */
	void removeRcvCallback(const char *topic);

	/// Returns whether the client is connected and subscribed
	bool isActive()
	{
		return (m_connect == MQTT_CONN_CONNECTED && m_subscribed == MQTT_SUB_DONE);
	}

	/// Sets event callback for receiving the result of connect,
	/// subscribe or publish operations.
	void setEventCallback(MqttClientEventCbk cbk, void *arg)
	{
		evCbk = cbk;
		evCbkArg = arg;
	}

	/**
	 * Set a keepalive ping [s] on application layer
	 * @param[in] timeout to start a ping check for all topics, set 0 to disable (default 10 minutes)
	 */
	void setApplicationKeepAlive(int timeout);

private:

	struct MqttTopic
	{
		void reset()
		{
			subscribed = false;
			lastMid = -1;
			checked = false;
		}

		std::string topic;      ///< mqtt topic
		bool        subscribed; ///< subscription status
		int         lastMid;    ///< mid of the last subscription request on this topic
		bool        checked;   ///< true if topic subscribtion has been checked with a ping message
	};

	struct SubscribeCmd
	{
		enum CmdType
		{
			TOPIC_ADD,
			TOPIC_REMOVE,
			TOPIC_REPLACE
		};

		CmdType actType;
		std::vector <std::string> topics;
	};

	typedef enum
	{
		MQTT_CONN_INIT,
		MQTT_CONN_PRERECONNECT,
		MQTT_CONN_RECONNECT,
		MQTT_CONN_CONNECTED,
		MQTT_CONN_CONNECTING,
		MQTT_CONN_QUIT
	} ConnStatus;

	typedef enum
	{
		MQTT_SUB_INIT,
		MQTT_SUB_ONGOING,
		MQTT_SUB_DONE,
		MQTT_SUB_ERROR
	} SubscriptionStatus;

	typedef enum
	{
		MQTT_PING_CHECK,
		MQTT_PING_CHECKING,
		MQTT_PING_DONE,
		MQTT_PING_ERROR
	} PingStatus;

	char  m_username[128];
	char  m_password[128];
	char  m_serverURI[128];
	int   m_serverPort;
	std::atomic<ConnStatus>          m_connect;  ///< flag client successfully connected to the server
	std::atomic<SubscriptionStatus>  m_subscribed;  ///< flag client successfully subscribed its topics
	std::atomic<PingStatus>          m_ping;  ///< indicate current status of application layer ping
	int   keepAlive;     ///< mosquitto keep alive in seconds
	int   appKeepAlive; ///< application layer keepalive in seconds

	/// Each topic is mapped to a callback function and correlator
	std::map<std::string, std::pair<MqttClientRcvCbk, void *>> rcvCbkMap;

	/// Callback to signal connect, subscribe or publish operations result
	MqttClientEventCbk evCbk;
	void *evCbkArg;     ///< event callback correlator
	std::thread *loopThread;
	volatile bool  quitThread;  ///< force mqttLoop to exit
	volatile bool  connRetry;   ///< change #on_disconnect behavior when mqtt disconnection occurs

	/// Lists of the topics the client is subscribed to
	std::vector<MqttTopic>   m_topics;

	/// Lists of the temporary topics
	std::mutex actionsMutex;
	std::queue<SubscribeCmd>   actionsQueue;

	std::chrono::steady_clock::time_point timeRef;  ///< used for internal timer
	unsigned int  connectionRetries; ///< number of consecutives connection errors;

	std::chrono::steady_clock::time_point subsTimeRef;  ///< used for subscription status internal timer
	unsigned int  subscriptionRetries; ///< number of consecutives connection errors;

	std::chrono::steady_clock::time_point pingTimeRef;  ///< used for ping status internal timer

private:
	bool subscribeAll();
	bool unsubscribeAll();
	bool pingCheckAll();

	void resetTopics(); ///< reset all mqtt topics to default status
	bool isAllSubscribed(); ///< return true if all topics are currently subscribed
	bool isAllTopicChecked(); ///< return true if all topics are currently subscribed and checked

	// mosquittopp interface
	void on_connect(int rc);
	void on_disconnect(int rc);
	void on_publish(int mid);
	void on_error();
	void on_message(const mosquitto_message *message);
	void on_subscribe(int mid, int qos_count, const int *granted_qos);
	void on_unsubscribe(int mid);
	void on_log(int level, const char *str);

	void runLoopMqtt();

	/**
	 * force mqtt loop end and join it
	 * @attention #loopThread must be valid
	 */
	void forceLoopMqttEnd();

	/**
	 * Add subscription a topic, the client should be connected to the server
	 * in order to be subscribe it own topic. The list of topics passed in the constructor
	 * are automatically registered after successful connection
	 *
	 * @param topic     topic to be registered
	 * @param qos       quality of service (use 1 => at least one notification)
	 * @param[out] mid  mid of the subscribe request
	 *
	 * @return 0 if subscription started, -1 if an error occurred
	 */
	int subscribeService(const char *topic, int qos, int &mid);

	/**
	 * Remove a topic subscription
	 *
	 * @param topic    topic to be unregistered
	 *
	 * @return 0 if unsubscription started, -1 if an error occurred
	 */
	int unsubscribeService(const char *topic);

	/**
	 * Check message received on subscribe test special topic
	 * @return true if correctly handled
	 */
	bool checkPingMessage(const mosquitto_message *message);

	/**
	 * Send an application ping on the selected topic
	 * @param topic
	 * @return 0 if ping started, -1 if an error occurred
	 */
	int  sendApplicationPing(const char *topic);
	bool updateTopics();
};
#endif // _MQTT_CLIENT_H
