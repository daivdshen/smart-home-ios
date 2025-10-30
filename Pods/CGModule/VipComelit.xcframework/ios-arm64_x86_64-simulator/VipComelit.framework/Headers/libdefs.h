#ifndef LIBDEFS_H
#define LIBDEFS_H

#include <stdint.h>
#include <sstream>

using std::string;

/// Default value to represent the user's password absence
const int USER_PASSWORD_INVALID = -1;

#define VIPER_DEFAULT_CONN_TIMEOUT    5       ///< [ s ] Default connection timeout for viper system creation

enum ViperChannelType{
    VIPER_CH_UNDEF = -1,
    VIPER_CH_INFO  = 0,
    VIPER_CH_PUSH,
    VIPER_CH_ECHO,
    VIPER_CH_UAUT,
    VIPER_CH_UADM,
    VIPER_CH_UCFG,
    VIPER_CH_FACT,
    VIPER_CH_CTPP,
    VIPER_CH_CSPB,
    VIPER_CH_ECHO_SRV,
    VIPER_CH_RTPC,
    VIPER_CH_FRCG,
    VIPER_CH_TCPSOCK,
    VIPER_CH_MAX_NUM_TYPE
};

enum ViperChannelError{
    VIPER_CH_OK,
    VIPER_CH_GENERIC_ERROR,
    VIPER_CH_CHANNEL_FORBIDDEN,
    VIPER_CH_CHANNEL_UNKNOWN
};

enum UnitFlags{
    FLAGS_NONE = 0x00,                      ///< No flags, used when we don't want any particular flag for the unit
    ENABLE_FSM_REF_COUNT = 0x01,            ///< Set newly-created FSM ref count to 1, i.e. make them owned by the application. The application must then call releaseFsm() to allow its destruction.
    VIP_UNIT_MASTER_NOT_DO_SUBPROXY = 0x02, ///< With this flag, the master unit don't create the subproxy locally (needed i.e. when extender master enabled on same device)
    ENABLE_RTP_OVER_RTSP = 0x04,             ///< Enable the receiving of the RTP flows on the RTSP stream
    ENABLE_RTSP_OVER_VIPER = 0x08           ///< Enable Rtsp traffic over viper tunnel (require ENABLE_RTP_OVER_RTSP)
};

enum CodecType{
    RTP_CT_MJPG = 129, ///< Codec Type MJPEG
    RTP_CT_H264 = 130, ///< Codec Type H264
    RTP_CT_MP4V = 131, ///< Codec Type MPEG-4
    RTP_CT_PCMA = 132, ///< Codec Type A-law (RFC3551#section-6)
    RTP_CT_PCMU = 133, ///< Codec type A-law (RFC3551#section-6)
    RTP_CT_NONE = 255  ///< Nessun Codec Type selezionato
};

/// Defines the maximum value of the RTP ADU (rtp adu max size 300 kB)
const int RTP_ADU_MAX_SIZE = 512 * 1024;
/// Default value of the maximum RTP payload size
const int MAX_RTP_MTU = 1440;

///< contains main parameters for a Stream
typedef struct {
    unsigned int width;              ///< width[pixels]
    unsigned int height;             ///< height [pixels]
    unsigned int ct;                 ///< codec type
    unsigned int fps;                ///< framerate of flow (avaiable after rx 2 consecutive frames)
} streamInfo;

typedef enum
{
    FILTER_STRATEGY_NONE = 0, ///< use all the remote candidates
    FILTER_STRATEGY_ONLY_PEER, ///< use only remote peer candidates
    FILTER_STRATEGY_ONLY_RELAYED ///< use only remote relayed candidates
}p2pFilterStrategy;


typedef enum
{
    P2P_SDP_MQTT,
    P2P_SDP_HTTP
}p2pSdpMode;

typedef enum
{
    P2P_VIPER_TOKEN,
    P2P_CLOUD_ACT_CODE,
    P2P_SAFE_TOKEN
}p2pHttpAuthMode;

struct p2pParameters{
    char mqttRole;          ///< the role of this peer in the p2p negotiation ('a' for controlled role, 'o' for controlling role)
    p2pSdpMode iceSdpMode;   ///< define use of mqtt or http transport for sdp exchange
    // mqtt parameters
    string mqttName;         ///< the client id to use on the MQTT broker
    string mqttPublicTopic;  ///< the topic to use to publish MQTT message (is the topic where the controlling agent listen)
    string mqttServer;       ///< the MQTT server used to exchange MQTT messages
    string mqttSubTopic;     ///< the MQTT topic where the controllend agent listen
    string mqttUser;         ///< the MQTT username used to authenticate on MQTT server
    string mqttPwd;          ///< the MQTT password used to authenticate on MQTT server
    string certificate;      ///< the certificate needed in order to use the mqtts on the MQTT server (NULL <=> plain-text MQTT messages)
    // http parameters
    string httpEndpoint;     ///< the http client endpoint
    string httpCcsToken;     ///< the user ccstoken
    string httpDuuid;        ///< the device uuid to connect to
    p2pHttpAuthMode httpAuthMode; ///< http authentication mode
    string httpAuthToken;    ///< the token to authenticate on device
    // connection parameters
    string stunSrvAddr1;     ///< the STUN server address used to gathering candidates
    string stunSrvAddr2;     ///< another STUN server address (used in case the first one is unreachable)
    string turnUser;         ///< the TURN username used to authenticate on TURN server
    string turnPwd;          ///< the TURN password used to authenticate on TURN server
    string turnSrvAddr;      ///< the TURN server used for relayed connection
    p2pFilterStrategy  strategy;    ///< The selected strategy used to filter the remote candidates
    bool isIPv6Only;        ///< flag to force the ipv6 address resolution
    int remNegoDelay;       ///< delay the start of udp and tcp negotitation

    p2pParameters() :
    mqttRole('a'),
    iceSdpMode(P2P_SDP_MQTT),
    mqttName(""),
    mqttPublicTopic(""),
    mqttServer(""),
    mqttSubTopic(""),
    mqttUser(""),
    mqttPwd(""),
    certificate(""),
    httpEndpoint(""),
    httpCcsToken(""),
    httpDuuid(""),
    httpAuthMode(P2P_VIPER_TOKEN),
    httpAuthToken(""),
    stunSrvAddr1(""),
    stunSrvAddr2(""),
    turnUser(""),
    turnPwd(""),
    turnSrvAddr(""),
    strategy(FILTER_STRATEGY_NONE),
    isIPv6Only(false),
    remNegoDelay(0)
    {}

    string getHttpAuthModeCloudKey() {
        switch (httpAuthMode) {
        case P2P_CLOUD_ACT_CODE: return "user_cloud_token";
        case P2P_VIPER_TOKEN:    return "user_viper_token";
        case P2P_SAFE_TOKEN:     return "user_safe_token";
        }
    }
};

///< struct to holds the mqtt parameters needed to establish a Mqtt "triangulation" connection to a safe control panel
struct SafeMqttTriangulationParameters {
    string mqttBaseName; ///< the common part for MQTT topics (all the safe triangulation topics are generated from this parameter)
    string serverURI; ///< the MQTT server used to exchange MQTT messages
    string clientId; ///< the client id to use on the MQTT broker
    string username; ///< the MQTT username used to authenticate on MQTT server
    string password; ///< the MQTT password used to authenticate on MQTT server
    string certificate; ///< the certificate needed in order to use the mqtts on the MQTT server ("" <=> plain-text MQTT messages)

    SafeMqttTriangulationParameters() :
    mqttBaseName(""),
    serverURI(""),
    clientId(""),
    username(""),
    password(""),
    certificate("")
    {}
};

typedef struct ViperConnParameters_TAG
{
    std::string address;
    int tcpPort;
    int udpPort;
    int connTimeout = VIPER_DEFAULT_CONN_TIMEOUT;
} ViperConnParameters;

enum ViperConnMode
{
    VIPER_CONNECTION_UNASSINGNED = 0,
    VIPER_CONNECTION_DIRECT,
    VIPER_CONNECTION_DIRECT_WITHOUT_RUN,
    VIPER_CONNECTION_P2P
};

typedef struct ViperParameters_TAG
{
    int sysId          = -1;
    ViperConnMode mode = VIPER_CONNECTION_UNASSINGNED;
    p2pParameters p2p;
    ViperConnParameters direct;
    bool          autoStart = true; ///< start tunnel immediately
} ViperParameters;

// Definitions of the push notification informations of the device

// Maximum length of Profile-ID field
const int APP_PROFILE_ID_SIZE = 4;
// Maximum length of Bundle-ID field
// Attention: Vedo family supports as length only 32 byte
const int APP_BUNDLE_ID_SIZE = 256;
// Maximum length of Token field
const int APP_DEVICE_TOKEN_SIZE = 256;

/// Structure to define the push notification informations of the device
typedef struct
{
    uint32_t profile_id;                             ///< Unique identifier of the push notification source (defined by destination device)
    uint8_t bundle_id[APP_BUNDLE_ID_SIZE];           ///< Unique identifier of the destination App of the push notification (Andorid, iOS)
    uint8_t device_token[APP_DEVICE_TOKEN_SIZE];     ///< Unique identifier of the destination Device (assigned by cluod service of the corresponding platform)
} pushNotificationInfo;

typedef union
{
    uint64_t tot;
    struct
    {
        uint16_t row_nbr;
        uint16_t row_size;
        uint16_t col_nbr;
    }spec;
    
} struct_tab_info_t;

typedef struct
{
    uint32_t fw_hardware_id;
    uint16_t fw_hardware_boot_ver;
    uint32_t fw_version_code;
    
    struct_tab_info_t subZoneCblTabInfo;
    struct_tab_info_t subZoneRadioTabInfo;
    struct_tab_info_t palIpCamerasTabInfo;
    struct_tab_info_t nvrCamerasTabInfo;
    struct_tab_info_t *cfgStructTabInfo;
} struct_control_panel_info;

/// Structure of the HomeServer status
typedef union
{
    uint16_t homeServerFlags;
    struct
    {
        uint16_t linkError:1;
        uint16_t internetError:1;
        uint16_t sdError:1;
        uint16_t dhcpError:1;
        uint16_t ntpError:1;
        uint16_t ddnsError:1;
        uint16_t smtpError:1;
        uint16_t :1;
        uint16_t supplyVoltageLevel:1;
        uint16_t :7;
    } homeServerFlag;
} eHomeServerStatus_t;

/// Type to store media type to save in memo file: values are used as bitmap
///  MEDIA_TYPE_AUDIO + MEDIA_TYPE_VIDEO = MEDIA_TYPE_AUDIO_VIDEO
typedef enum
{
    MEDIA_TYPE_AUDIO       = 1,
    MEDIA_TYPE_VIDEO       = 2,
    MEDIA_TYPE_AUDIO_VIDEO = 3
}MediaType;

///Enumerate of Safe device family
typedef enum
{
    DEVICE_VEDO_FAMILY,
    DEVICE_HUB_FAMILY,
    DEVICE_UNKNOWN_FAMILY
} SafeDeviceFamily;


///viper tcpsock channel data struct
typedef struct
{
    uint32_t       type;     ///< 0->rtsp
    char           hostname[128];
    uint32_t       port;
} __attribute__((__packed__)) tcpsockdata_t;

typedef struct
{
    uint32_t hwId;
    uint16_t hwBootVer;
    uint32_t versionCode;
    string   fwDesctiption;
} safeApplicationInfo_t;

#endif // LIBDEFS_H
