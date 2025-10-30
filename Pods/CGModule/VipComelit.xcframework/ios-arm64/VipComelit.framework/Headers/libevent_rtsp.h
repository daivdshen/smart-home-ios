#ifndef LIBEVENT_RTSP_H
#define LIBEVENT_RTSP_H

#ifndef __clang__
namespace Libcomelit {
namespace Rtsp       {
namespace Events     {
#endif

//Typedef per la definizione della tipologia del messaggio (msg_type_id)
typedef enum {
    RtspFsmStatusChange = 0,
    RtspFsmEvent
}eMsgTypeRtsp;


///Typedef per la segnalazione del cambiamento degli stati Call FSM (call_fsm_status_id)
typedef enum {
    RtspFsmStIdle = 0,
    RtspFsmStConnected,
    RtspFsmStWaitForConn,
    RtspFsmStWaitOpt,
    RtspFsmStWaitDsc,
    RtspFsmStWaitSet,
    RtspFsmStClosed
}eRtspFsmStates;

///Typedef for answer result
typedef enum {
	RtspOk,
	RtspErrInvalidUrl,
	RtspErrSocket,
	RtspErrNoRep,
	RtspErrCmd,
	RtspErrLogin,
	RtspErrGeneric,
	RtspErrUnsup,
	RtspErrStatus,
    RtspErrVideo
}eReplyRtsp;

#ifndef __clang__
}
}
}
#endif

#endif // LIBEVENT_RTSP_H
