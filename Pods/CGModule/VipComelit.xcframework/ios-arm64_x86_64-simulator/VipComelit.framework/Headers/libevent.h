#ifndef LIBEVENT_H
#define LIBEVENT_H

#include "libevent_vip.h"   ///Define relative al mondo videocitofonia VIP
#include "libevent_safe.h"  ///Define relative al mondo allarme SAFE
#include "libevent_tvcc.h"  ///Define relative al mondo TVCC
#include "libevent_rtsp.h"  ///Define relative ad una chiamata di tipo RTSP

///Typedef per la definizione della tipologia della Unit (unit_type_id)
typedef enum {
    None = -1,
    VipUnit = 0,
    AlarmUnit,
	HomeAutomationUnit,
	TvccUnit,
	RtspUnit
}eUnitType;

///Typedefs per la definizione della tipologia di messagio a livello di Sistema (msg_type_id)
typedef enum {
    ViperConnectionEvent = 0, ///< viper_connection_event
    ViperChannelOpenRes  ,    ///< viper_channel_open_res
    ViperChannelMsg,          ///< viper_channel_msg
    SystemStatusMsg,          ///< System status message
    LibraryStatysMsg,          ///< Library status message
	LogEventMsg,           ///< Log message
}eSystemMsgType;

typedef enum {
    ViperConnected = 0,
    ViperConnectionError,
    ViperConnectionLost,
    ViperP2PConnectionReady,
	ViperP2PUDPConnectionReady,
    ViperP2PAuthError,
    ViperP2PDeviceNotConnected,
    ViperP2PDeviceBadResponse,
    ViperP2PDeviceNotFound
}eViperConnection;

typedef enum {
    SystemCreated = 0,
    SystemStarted,
    SystemStopped,
    SystemDestroyed
} eSystemStatus;

///Typedefs per la definizione della tipologia di messagio a livello LogEventMsg (log_type_id)
typedef enum {
    LogEvent = 0,       ///< log_event
}eLogMsgType;

#endif // LIBEVENT_H
