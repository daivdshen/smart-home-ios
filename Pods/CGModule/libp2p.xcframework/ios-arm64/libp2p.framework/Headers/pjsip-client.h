/**
 * @file pjsiop-client.h
 *
 * Pjsip client interface module.
 *
 * Contains all the functions to create local ice candidates and
 * to start an ice negotiation with a remote peer.
 *
 * @author Roberto Muzzì
 */

#ifndef ICE_CLIENT_H
#define ICE_CLIENT_H

#include <pjlib-util.h>
#include <pjnath.h>
#include <string>
#include <pthread.h>
#include <queue>
#include <mutex>
#include <chrono>
#include <memory>
#include <thread>
#include <atomic>
#include <condition_variable>

#include "p2pconfig.h"
#include "sdp.h"
#if defined(LIBP2P_ENABLE_DTLS)
#include "dtls.h"
#endif


class PseudoTcpSck;
class IceClient;

/// Lists of ice session transport types
typedef enum
{
	ICE_SESSION_TCP = 0, ///< TCP session
	ICE_SESSION_UDP,   ///< UDP session
	ICE_SESSION_MUX,   ///< Muxed udp/tcp session
	ICE_SESSIONS_NO
} ice_session_enum_t;

/// Lists of ice session data types
typedef enum
{
	ICE_DATA_TCP    = ICE_SESSION_TCP,       ///< TCP data (keep backward compatibility with ice_session_enum_t
	ICE_DATA_STREAM = ICE_SESSION_UDP,       ///< Stream RTP data encapsulated on comelit viper protocol (UDP)
	ICE_DATA_DTLS, ///< dtls data (dtls stuff or TCP encrypted
	ICE_DATA_NO
} ice_data_enum_t;


/// Lists of ice role possible types
typedef enum
{
	ICE_ROLE_MASTER = 'o', ///< Ice controlling agent
	ICE_ROLE_SLAVE  = 'a', ///< Ice controlled agent
	ICE_ROLES_NO
} ice_role_enum_t;

///Struct that allow to store the type and the adresses of nominated candidates
typedef struct
{
	pj_ice_cand_type local;  /// local nominated candidate
	pj_ice_cand_type remote; /// remote nominated candidate
	std::string localAddrExt; ///  public address of local candidate
	std::string localAddrInt; ///  private address of local candidate
	std::string remoteAddrExt; /// public address of remote candidate
} ice_nominated_candidates;

typedef enum
{
	ICE_TCP_CONNECT = 0,
	ICE_TCP_DISCONNECT
} ice_tcp_state_t;

typedef void (*iceClientStateCbk_t)(void *arg, void *icecl,
                                    pj_ice_strans_op op, pj_status_t status);

typedef void (*iceRxDataCbk_t)(void *icecl, void *arg, const char *data, int len, int flags);


typedef void (*iceClientTcpStateCbk_t)(void *arg, void *icecl,
                                       ice_tcp_state_t state);

#if defined(LIBP2P_ENABLE_DTLS)
typedef std::function<void (IceClient *sender, DtlsSrtp::DtlsEvent event)> IceClientDtlsEventCbk;
#endif

/**
 * This is the main application initialization function to be called
 * once (and only once) during application initialization sequence.
 */
extern "C" void iceInit(char *log_file);

/**
 * Callback called when user data (not ICE protocol messages)
 * is received on ICE session.
 */
extern "C" void cb_on_rx_data(pj_ice_strans *ice_st,
                              unsigned comp_id,
                              void *pkt, pj_size_t size,
                              const pj_sockaddr_t *src_addr,
                              unsigned src_addr_len);

/**
 * Callback called when ICE steps are completed (gathering candidates,
 * nominated candidates,...)
 */
extern "C" void cb_on_ice_complete(pj_ice_strans *ice_st,
                                   pj_ice_strans_op op,
                                   pj_status_t status);

/**
 * Callback called when ICE destroy is completed (all resources are free)
 */
extern "C" void cb_on_ice_destroy(void *user_data);

/**
 * The IceMessage class is a container of ice packets send/receive
 */
class IceMessage
{
public:
	IceMessage(const char *data, int len, int type);
	~IceMessage();
public:
	char *pkt;
	int   len;
	int   type;
	bool  isEncrypted;
	int   transmissionCount;
};

/**
 * The IceStream Class contains data on video/audio active stream
 */
class IceStream
{
public:
	IceStream(int pt);
	void reset()
	{
		pt = 0;
		ssrc = 0;
		isActive = false;
	}
public:
	int   pt;
	uint32_t ssrc;
	bool  isActive;
};

class ThreadTask
{
public:
	ThreadTask(const std::string &_name, std::function<void()> &&_func);

	~ThreadTask();

	bool isRunning() {return running; }

private:
	std::string name;
	bool running {false};
	std::function<void()> func;
	std::thread *thread;

	void stop();
};

/* This is our global variables */
class IceClient
{
public:
	/**
	 * Create ICE stream transport instance.
	 */
	IceClient(char role, int sub, char *stun_srv,
	          char *turn_username, char *turn_password, char *turn_srv, pj_bool_t turn_tcp,
	          std::vector<pj_ice_cand_type> allowedCandidateTypes, bool is_ipv6_only, int sessionType);

	/**
	 * Destroy ICE stream transport instance.
	 */
	~IceClient();

	/**
	 * Create ICE session.
	 */
	void initSession();

	/**
	 * Stop/destroy ICE session.
	 */
	void stopSession();

	/**
	 * Encode ICE information in SDP.
	 */
	int encodeSession(std::stringstream &ss, const std::string &fingerprint);

	/**
	 * Input and parse SDP from the remote (containing remote's ICE information)
	 * and save it to global variables.
	 */
	void inputRemote(char *remote_sdp);

	/**
	 * Start ICE negotiation!.
	 */
	void startNego();

	/**
	 * Start negotiotion after the delay suggested by the remote peer
	 * @attention method not thread safe. Call in worker_thread context only
	 */
	void startNegoDelayed();

	/**
	 * Send application data to remote agent using udp or tcp connection.
	 */
	std::shared_ptr<IceMessage> sendData(unsigned comp_id, const char *data, int len,
	                                     int type = ICE_DATA_STREAM);

	/**
	 * Send application data to remote agent using udp or tcp connection.
	 */
	bool sendData(unsigned comp_id, std::shared_ptr<IceMessage> data);

	/// Returns when the role is controlling or not (controlled)
	bool isController()
	{
		return m_role=='o';
	}

	/// Returns the state of the ice session
	int getState();

	int sub(void) { return m_sub; }

	int getRole() { return m_role; }

	int getSession() { return currSessionId; }

	bool hasRemoteInfo() { return remoteSdpRecved; }

	bool getCandidatesInfo(ice_session_enum_t session, ice_nominated_candidates &candidatesInfo);

	void setIceClientStateCbk(iceClientStateCbk_t cbk, void *param)
	{
		iceClientStateCbk    = cbk;
		iceClientStateCbkArg = param;
	}

	void setIceRxDataCbk(iceRxDataCbk_t cbk, void *param)
	{
		iceRxDataCbk    = cbk;
		iceRxDataCbkArg = param;
	}

	void setIceClientTcpStateCbk(iceClientTcpStateCbk_t cbk, void *param)
	{
		iceClientTcpStateCbk    = cbk;
		iceClientTcpStateCbkArg = param;
	}

	/**
	 * @brief isRunning
	 * @return true is ICE client started and stopSession() has not
	 * been requested yet.
	 */
	pj_bool_t isRunning() { return !threadQuitFlag; }

	/**
	 * @brief isAlive
	 * @return true if any of the underlying threads is still processing
	 * data. checking for isAlive() before delete is recommended to ensure
	 * thread join() will be fast
	 */
	bool isAlive();

	/**
	 * Starts the pjsip thread that handles events
	 *
	 * @return true if thread successfully created
	 */
	pj_bool_t startWorkerThread();

	/**
	 * Suggested delay to remote to start negotiation
	 * @param[in] suggested remote delay [ms]
	 * @retval true remote delay is valid and applied
	 */
	bool setRemoteNegoDelay(int value);

	const std::string &getSdpReplyId() {return sdp_reply_id;}

#if defined(LIBP2P_ENABLE_DTLS)
	/**
	 * Set openssl context and enable dtls encryption on session
	 * @param ssl
	 */
	void setDtlsCtx(SSL_CTX *ssl);

	/**
	 * Set callback for dtls events
	 * @param cbk
	 */
	void setDtlsEventcallback(IceClientDtlsEventCbk cbk) { dtlsEvtCbk = cbk; }
#endif

	bool isEncryptionEnabled() { return isEncrypted; }

	void dataChannelCreate(); ///< create tcp socket over udp

	void setRtpPaddingLen(int padding) { rtpPaddingLen = padding; }

	void setIceRole(pj_ice_sess_role role);

	/**
	 * Change session id from mux to legacy tcp
	 * to be used if o role does not support muxed socket
	 * @param sessId
	 * @return true on success
	 */
	bool changeSessionId(int sessId);

	void setSessionEnd();

	/*
	 * rx data processing must be public
	 * in order to be called from pjsip callback
	 */
	void processDataRxPck(char *pkt, int size);

	IceStream getVideoStreamInfo() { return rtpVideoStream; }
	IceStream getAudioStreamInfo() { return rtpAudioStream; }
	IceStream &getAudioRemoteStreamInfo() { return rtpAudioStreamRemote; }
	IceStream &getVideoRemoteStreamInfo() { return rtpVideoStreamRemote; }

private:
	/**
	 * Create ICE stream transport instance, which restart negotiation
	 */
	bool createSession();

	/// Returns the type of connection between noninated peers
	/// @return connection type or PJ_ICE_CAND_TYPE_MAX if no
	/// nominated peer connected.
	int getNominatedCandType();

	int isTurnTcp() { return opt.turn_tcp; }

	/**
	 * Handler of pjsip internal events (io queues and timers)
	 *
	 * @param timeout_ms internal poll timeout for events [ms]
	 * @return status code
	 */
	pj_status_t handle_events(int timeout_ms = 100);

	void cleanUp(void);

	/**
	 * Stop worker thread
	 * call ONLY outside the worker thread context
	 * @return
	 */
	pj_bool_t stopWorkerThread();

	void send(const char *, int len);

	/**
	 * Send Data packet
	 * @param len
	 */
	int sendData(const char *, int len);

	/**
	 * Send rtp packet
	 * @param len
	 */
	int sendRtp(std::shared_ptr<IceMessage> pkt);

	/**
	 * Receive Rtp Packet
	 * @param[in] pkt data from ice connection
	 * @param[in] len of packet from ice connection
	 * @param[in] type of packet
	 */
	void receiveRtp(char *pkt, int len, int type);

	void receive(const char *, int len, int type);

	void notifyTcpLine(ice_tcp_state_t state);

	void enqueueRxMsg(std::shared_ptr<IceMessage> pkt);

	std::shared_ptr<IceMessage> getRxMsg();

	bool enqueueTxMsg(std::shared_ptr<IceMessage> pkt);

	std::shared_ptr<IceMessage> getTxMsg();
	std::shared_ptr<IceMessage> getTxTcpMsg();

	bool isSessionEnd();

	/**
	 * Suggested delay from remote to start negotiation
	 * @return suggested delay [ms]
	 */
	int getStartNegoDelay();

	int  getRtpPaddingLen() { return rtpPaddingLen; }
	bool isWebrtc() { return rtpPaddingLen == 0; } ///< generic webrtc session does not have viper padding

	/// Utility to create a=candidate SDP attribute
	void printCand(std::stringstream &ss, const pj_ice_sess_cand *cand);

	int encodeSessionAnswer(std::stringstream &ss,
	                        const std::string &fingerprint); ///< encode session 'o' role
	int encodeSessionOffer(std::stringstream &ss,
	                       const std::string &fingerprint); ///< encode session 'a' role
	/* Process data functions
	 * Those set of functions MUST be
	 * called on the same thread context
	 */
	int processData();
	void processDataRx();

	void stopThreads();

	void dataChannelCreatePrivate(); ///< create tcp socket over udp

public:
	pj_ice_strans	*icest;

	iceClientStateCbk_t iceClientStateCbk;
	void               *iceClientStateCbkArg;

#if defined(LIBP2P_ENABLE_DTLS)
	DtlsSrtp *dtlsSrtp {nullptr}; ///< dtls srtp encryption instance
#endif

private:
	/// Flag to quit main ice client thread
	pj_bool_t threadQuitFlag;

	iceClientTcpStateCbk_t iceClientTcpStateCbk;
	void                  *iceClientTcpStateCbkArg;

	iceRxDataCbk_t      iceRxDataCbk;
	void               *iceRxDataCbkArg;

	PseudoTcpSck *tcpSck;

	bool enStartNego;
	std::chrono::steady_clock::time_point nowRef;

	bool      isEncrypted; ///< true if session require dtls encryption

	int  currSessionId;
	bool remoteSdpRecved;
	Sdp  *remoteSdp;

	/// client id it uses subaddr
	/// @todo check if can conflict between apartments
	char clientid[2];

	/// Command line options are stored here
	struct options
	{
		unsigned    comp_cnt;
		pj_str_t    ns;
		int         max_host;
		pj_bool_t   regular;
		pj_str_t    stun_srv;
		pj_str_t    turn_srv;
		pj_bool_t   turn_tcp;
		pj_str_t    turn_username;
		pj_str_t    turn_password;
		pj_bool_t   turn_fingerprint;
		pj_ice_sess_role ice_role;
		const char *log_file;
	} opt;

	/// Our global variables
	pj_caching_pool	  cp;
	pj_pool_t		  *pool;
	ThreadTask                *wthread;
	pj_ice_strans_cfg iceCfg;
	pj_ice_strans_cb  icecb;

	/// Variables to store parsed remote ICE info
	struct rem_info
	{
		char		     ufrag[80];
		char		     pwd[80];
		unsigned	     comp_cnt;
		pj_sockaddr	     def_addr[PJ_ICE_MAX_COMP];
		unsigned	     cand_cnt;
		pj_ice_sess_cand cand[PJ_ICE_ST_MAX_CAND];
		int			     start_nego_delay;              ///< suggested start nego delay
	} rem;

	char m_role;
	int  m_sub;

	std::condition_variable rxCond;
	std::mutex rxMsgLock;
	std::queue<std::shared_ptr<IceMessage>> rxMsg;
	std::queue<std::shared_ptr<IceMessage>> txMsg;
	std::queue<std::shared_ptr<IceMessage>> txTcpMsg;
	std::mutex txMsgLock;
	std::mutex txTcpMsgLock;

	bool isEnd;
	std::vector<pj_ice_cand_type>
	m_allowedCandidateTypes;   ///< list of allowed remote candidate type (used to filter some candidates for some bad mobile operators)
	int remNegoDelay;	// suggested remote start nego delay
	std::string sdp_reply_id;

	std::mutex pjCfgLock; ///< protect access to pjstrans session setting

#if defined(LIBP2P_ENABLE_DTLS)
	IceClientDtlsEventCbk dtlsEvtCbk;
#endif
	int rtpPaddingLen;

	IceStream rtpVideoStream; ///< Rtp video stream data (if streaming video)
	IceStream rtpAudioStream; ///< Rtp audio stream data (if streaming audio)

	IceStream rtpVideoStreamRemote; ///< Rtp video stream data (if streaming video)
	IceStream rtpAudioStreamRemote; ///< Rtp audio stream data (if streaming audio)

	std::atomic<bool> createDataChannel; ///< data channel creation request status

	ThreadTask *rxThread;
};

#endif // ICE_CLIENT_H
