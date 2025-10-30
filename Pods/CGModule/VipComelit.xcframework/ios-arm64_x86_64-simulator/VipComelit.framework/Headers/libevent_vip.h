#ifndef LIBEVENTVIP_H
#define LIBEVENTVIP_H

///Typedef per la definizione della tipologia del messaggio (msg_type_id)
typedef enum {
    CallFsmStatusChange = 0,
    CallEvent,
    SubunitFsmStatusChange,
    SubunitEvent,
    OpenDoorEvent,
    SetOutputEvent,
    CfpEvent,
    ElevatorEvent,
    FireAlertEvent,
    SetVirtInp,
}eMsgType;


///Typedef per la segnalazione del cambiamento degli stati Call FSM (call_fsm_status_id)
typedef enum {
    CallFsmStIdle = 0,      ///<Stato transiente non segnalato (perchè immediatamente sostituito dal CallFsmStInAlerting o CallFsmStOutInitiated)
    CallFsmStInAlerting,
    CallFsmStConnected,
    CallFsmStOutInitiated,
    CallFsmStOutAlerting,
    CallFsmStClosed
}eCallFsmStates;


///Typedef per la definizione del tipo chiamata (call_type_id)
typedef enum {
    CallTypeCFP = 0,        ///< CFP (out-of-door call)
    CallTypeNORMAL,         ///< Normal door entry call
    CallTypeINTERCOM,       ///< Intercom call
    CallTypeTVCC,           ///< Video surveillance call
    CallTypeTEST,           ///< Connectivity/bandwidth test call
    CallTypeVIP             ///< ViP calls
}eCallType;


///Typedef per la definizione del tipo ruolo del chiamante (role_id)
typedef enum {
    RoleTypePorter = 0,
    RoleTypeIntUnit,
    RoleTypeCps,
    RoleTypeCfp,
    RoleTypeCamera,
    RoleTypeVidOut,
    RoleTypeIO,
    RoleTypeFire,
    RoleTypeUnknown = 100
}eRoleType;


///Typedef per la segnalazione degli eventi Call FSM (event_id - Call FSM)
typedef enum {
    CallFsmEventCapReport = 0,
    CallFsmEventInDiverted,
    CallFsmEventOutDiverted,
    CallFsmEventReleased,
    CallFsmEventConnectionError,
    CallFsmEventHandleMediareq,
    CallFsmEventTimerExpired,
    CallFsmEventVoiceStatus,
    CallFsmEventHandleMediaoffer
}eCallFsmEvents;

typedef enum {
    MediareqChannelAudio = 0,
    MediareqChannelVideo
}eMediareqChannel;

typedef enum {
    MediareqTypeStop = 0,
    MediareqTypeStart
}eMediareqType;

typedef enum {
    RelCauseNormal = 0,
    RelCauseUserBusy,
    RelCauseNonSelected,
    RelCauseRejected,
    RelCauseDiverted,
    RelCauseUserNotFound,
    RelCauseDeviceNotExist,
    RelCauseSystemBusy,
    RelCauseCallUnsupported,
    RelCauseMalformedMessage,
    RelCauseInternalError,
    RelCauseGenericFailure,
    RelCauseUnknown = 100
} eReleaseCause;

//Typedef per la segnalazione del cambiamento degli stati Subunit FSM (subunit_fsm_status_id)
typedef enum {
    SuFsmStRegistering=0,
    SuFsmStFailwait,
    SuFsmStRegistered
}eSubunitFsmStates;

typedef enum {
    SuFailCauseNotManaged = 0,
    SuFailCtpCidNone,
    SuFailTimeout,
    SuFailConnectionAborted
}eSubunitFailWaitCause;

//Typedef per la segnalazione degli eventi Subunit FSM (event_id - SubUnit)
typedef enum {
    SuFsmEventRegisterRes = 0
}eSuFsmEvents;

typedef enum {
    RegResOk = 0,
    RegResUnexpectedMsg,
    RegResRecipientErr,
    RegResIdErr,
    RegResGenericErr
}eRegisterRes;

//Typedef per la segnalazione degli eventi Out Of Call Opendoor e Set Output (event_id - OutOffCall)
typedef enum {
    OocEventRelease = 0,
    OocEventUnexpectedMsg,
    OocEventCtpDisconnected,
    OocEventReceivedStatus,
    OocEventReceivedODRequest,
    OocEventReceivedSORequest,
    OocEventCfpCall,
    OocEventFireAlert,
    OocEventSetVirtInp
}eOutOfCallEvents;

//Typedef per la direzione della chiamata in corso
typedef enum {
    DIR_OUTGOING = 0,
    DIR_INCOMING
}eCallDirection;

#endif // LIBEVENTVIP_H
