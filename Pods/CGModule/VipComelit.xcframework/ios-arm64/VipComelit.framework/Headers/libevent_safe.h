#ifndef LIBEVENT_SAFE_H
#define LIBEVENT_SAFE_H

///Typedef per la definizione della tipologia del messaggio (msg_type_id)
typedef enum {
    ActionGenReply,
    CfgWriterGenReply,
    LogInReply,
    LogOutReply,
    UsrInfoReply,
    CfgDbInfo,
    CfgRtAreasStatus,
    CfgRtZonesStatus,
    CfgRtOutputsStatus,
    CfgRtPalIpStatus,
    CfgRtUserSession,
    CfgRtZoneAlarms,
    CfgRtSysTamper,
    CfgRtSysAnomalies,
    CfgRtSysTimers,
    CfgRtSysGeneric,
    CfgRtLogEvent,
    CfgDbGroupElem,
    CfgDbSingleReq,
    AppNotification,
    CfgRtTextLogEvent,
    MediaHandlerReply,
    LiveViewReply,
    VideoCallStatusChange,
    VideoCallEvent,
    VideoCallOpenDoorEvent,
    VideoCallSetOutputEvent,
    VideoCallCfpEvent,
    MsgError,
    ActionCloudReply,
    CfgFirmwareInfo,
    CfgTabInfo,
    Rt485Packet,
    DateTimeReading,
    DateTimeWriting,
    CfgRtCCTVStatus,                 //Type message for cameras real time status.
    VideoTimelinesReply,
    CfgRtSysZoneOpen,
    EmulLedsStatus,
    EmulLcdFirstRowDisplay,
    EmulLcdSecondRowDisplay,
    EmulRequestStatus,
    CfgRtGenericFlag
}eMsgTypeSafe;

typedef enum {
    ActRepPerformed,                //Action completed
    //ActRepNotAllowed
    //ActRepUserAuthorized
    ActRepNeedUsrLogIn,             //Action requires code
    ActRepActivationBlocked,        //Activ. action locked
    ActRepActivationBlockedL2,      //Activ. action locked L2
    ActRepActivationBlockedL3,      //Activ. action locked L3
    ActRepActivationBlockedTot,     //Activ. action locked TOT
    ActRepActivationBlockedP1,      //Activ. action locked P1
    ActRepActivationBlockedP2,      //Activ. action locked P2
    ActRepUserUnknown,              //User not recognised
    ActRepInstallerNotAuthorized,   //Installer not authorised
    ActRepUserNotAuthorized,        //User not authorised
    ActRepUserNotAuthorizedToAct,   //User not authorised to activate
    ActRepUserNotAuthorizedToDeact, //User not authorised to deactivate
    ActRepUserNotAuthorizedToIndAct,//User not auth. ind. act.
    ActRepUserNotAuthorizedToForce, //User not auth. to force an activation
    ActRepUserNotAuthorizedOnDevice,//User not auth. on device
    ActRepUserNotAuthorizedOnAreas, //User not auth. on areas
    ActRepUserEnableTimerExpired,   //User enab. timer expired
    ActRepUserOperationsOverflow,   //User oper. overflow
    ActRepYouWon,                   //You have won a ctrl unit
    ActRepKeyNotValid,              //Key not valid
    ActRepActionUnknown,            //Act. action unknown
    ActRepDeviceUnknown,            //Device not recognised
    ActRepOutputAbsent,             //Output absent
    ActRepZoneAbsent,               //Zone absent
    ActRepZoneCantBeExcluded,       //Zone act. not for excl.
    ActRepZoneCantBeIsolated,       //Zone act. not for isol.
    ActRepDeviceError,              //Device error
    ActRepTechnicalProgramming,     //Technical programming
    ActRepUpgradeError,             //Upgrade error
    ActRepActivatedAreaError,       //Activated areas error
    ActRepQueueFull,                //Action queue full
    ActRepDevicedLocked,            //Device locked
    ActRepCommandUnknown,           //Command not recognised
    ActRepAlarmInProgress,          //Alarm in progress
    ActRepNotRecognized,            //Action reply not recognised
    //Start internal Error
    ActRepTxError,                  //Errore invio messaggio
    ActRepNoReply,                  //Mancata risposta dal remoto
    ActRepForcedLogOut,             //Logout forzato
    ActRepReinitHandler,            //Re-inizializzazione gestore
    ActRepDbInvalid,                //Database invalido
    ActRepGenericError,             //Errore generico
    ActRepStopHandler,              //Stop() gestore
    ActRepFatalError,               //Errore fatale
    //Start Media Device Error
    ActRepRxedMsgWithParamError,    //Errore nei parametri del messaggio ricevuto
    ActRepUnexpectedMsg,            //Ricezione messaggio non atteso
    ActRepErrorOnCameraReq,         //Errore nella richiesta effettuata al remoto
    ActRepCameraDisabled,           //Telecamera disabilitata
    ActRepCameraFailure,            //Telecamera in guasto
    ActRepImageNumbInvalid,         //Errore nel numero immagini ricevuto
    ActRepMediaDeviceBusy,          //Dispositivo gestione media occupato
    ActRepMediaDeviceError,         //Dispositivo gestione media in errore
    ActRepReqNotFound,              //Richiesta non trovata
    ActRepDeviceOnPendingOperation, //Dispositivo gestione media in attesa di chiusura operazione salvataggio media
    //Cloud Error
    ActRepCloudError                //Error cloud registration
} eActionReply;

typedef enum {
    LogInLevel1,
    LogInLevel2,
    LogInLevel3,
    LogInLevel4,
    LogInLevelNone
}eLogInLevel;

typedef enum {
    UserType1,
    UserType2,
    UserType3,
    UserType4,
    UserTypeRound,
    UserTypeInst1,
    UserTypeInst2,
    UserTypeMaster,
    UserTypeManufacturer,
    UserTypeNotHandled,
    UserTypeMainUser,
    UserTypeEN50131,
    UserType5,
    UserType6
}eUserType;

typedef enum {
    CfgDbVersionAligned,        //Vedo DB Version == API
    CfgDbVersionMisaligned,     //Vedo DB Version > API, Forward compatibility is guaranteed
    CfgDbVersionError           //Vedo DB Version < API, Backward compatibility is NOT guaranteed
}eCfgDbVersionInfo;

typedef enum {
    AreaStatusDeactivated,
    AreaStatusTotActivated,
    AreaStatusPart1Activated,
    AreaStatusPart2Activated,
    AreaStatusNotReady,
    AreaStatusAlarm,
    AreaStatusSabotage,
    AreaStatusAnomaly,
    AreaStatusAlarmMemory,
    AreaStatusNotReadyTot,
    AreaStatusNotReadyPart1,
    AreaStatusNotReadyPart2,
    AreaStatusOnAlarm,
    AreaStatusZoneExclusion,
    AreaStatusIncomingTime,
    AreaStatusOutcomingTime,
    AreaStatusPart1Disabled,
    AreaStatusPart2Disabled,
    AreaStatusNoBurglarZone
}eCfgAreaStatus;

/* Enumerati per la decodifica Log Eventi / Allarme */

typedef enum {
    SecureLogType,      ///Log eventi
    AlarmLogType,       ///Log allarmi
    GenericLogType,
    FirstZoneLogType
}eLogType;

typedef enum {
    PreviousSearchDirType,
    FirstSearchDirType,
    NextSearchDirType
}eLogSearchDirectionType;

// Azioni log eventi
typedef enum {
    LogActionTypeNone,
    LogActionTypeSet,
    LogActionTypeReset,
    LogActionTypeAlarm,
    LogActionTypeReactivation,
    LogActionTypeForcedExclusion,
    LogActionTypeAlarmTest,
    LogActionTypeReactivationTest,
} eLogActionType;

//Tipologia eventi
typedef enum {
    LogEventNone,
    LogEventSystem,
    LogEventAreas,
    LogEventZone,
    LogEventMobileApp,
    LogEventKeyboard,
    LogEventRfid,
    LogEventSafetouch,
    LogEventGenericTouch,
    LogEventRemoteCom,
    LogEventUser,
    LogEventRadioController,
    LogEventGroups,
    LogEventTimer,
    LogEventOutput,
    LogEventAction,
    LogEventCodeChange,
    LogEventDbReactivation,
    LogEventHomeAutomBridge,
    LogEventReinit,
    LogEventNotHandled
} eLogEventType;

//Eventi di sistema o di area
typedef enum {
    LogSubEventZoneAlarm,
    LogSubEventTamperAlarm,
    LogSubEventMalfunction,
    LogSubEventOrAlarms,
    LogSubEventActivation,
    LogSubEventActivationTimers,
    LogSubEventGeneric
} eLogSubEventTypeSysArea;

//Tipi di allarme zone
typedef enum {
    LogZoneAlarmBurglar,
    LogZoneAlarmH24,
    LogZoneAlarmFire,
    LogZoneAlarmPanic,
    LogZoneAlarmRobbery,
    LogZoneAlarmFlood,
    LogZoneAlarmGas,
    LogZoneAlarmMedicalEmergency,
    LogZoneAlarmDuress,
    LogZoneAlarmNotHandled
} eLogSubEventDetailTypeZoneAlarm;

//Tipi di allarme sabotaggio
typedef enum {
    LogTamperZone,
    LogTamperRadioSiren,
    LogTamperRadioDeviceBlocked,
    LogTamperPhoneApp,
    LogTamperBusIoExp,
    LogTamperBusKeyboard,
    LogTamperBusRfid,
    LogTamperBusSafetouch,
    LogTamperBusGtouch,
    LogTamperBusPowerSupplyUnitOrInsulator,
    LogTamperBusRadioConc,
    LogTamperBusSiren,
    LogTamperBusAlarmComunicDevs,
    LogTamperBusIpExp,
    LogTamperBusIpVideoDevs,
    LogTamperRemoteCom,
    LogTamperGsmBlock,
    LogTamperCodeError,  // è stato raggiunto il numero massimo di errori di sistema
    LogTamperNotHandled
} eLogSubEventDetailTypeAlarmTamper;

//Tipi di allarme anomalie
typedef enum {
    LogMalfuncZone,
    LogMalfuncSiren,
    LogMalfuncRadioController,
    LogMalfuncBusPowerSupplyUnitFuse,
    LogMalfuncBusPowerSupplyUnitBattery,
    LogMalfuncRadioOutputBattery,
    LogMalfuncBusSirenBattery,
    LogMalfuncBusPowerSupplyUnitPwrSupply,
    LogMalfuncRadioZoneBattery,
    LogMalfuncRadioControllerBattery,
    LogMalfuncVoltageLevPbusIn,
    LogMalfuncVoltageLevPbusOut,
    LogMalfuncSupervRxZone,
    LogMalfuncSupervRxOutput,
    LogMalfuncOutput,
    LogMalfuncVideoCamera,
    LogMalfuncBusGsmGprsNetwork,
    LogMalfuncBusTelLine,
    LogMalfuncCommFailure,
    LogMalfuncRobberyZone,
    LogMalfuncSysNotActivated,
    LogMalfuncGenericBusIpExp,
    LogMalfuncMotionDetectDevBlocked,
    LogMalfuncBusIoExp,
    LogMalfuncBusKeyboard,
    LogMalfuncBusRfid,
    LogMalfuncBusSafetouch,
    LogMalfuncBusGtouch,
    LogMalfuncBusPowerSupplyUnitOrInsulator,
    LogMalfuncBusRadioConc,
    LogMalfuncBusSiren,
    LogMalfuncBusAlarmComunicDevs,
    LogMalfuncBusIpExp,
    LogMalfuncBusIpVideoDevs,
    LogMalfuncCtrlUnitBattery,
    LogMalfuncCtrlUnitPwrSupply,
    LogMalfuncPstnDev,
    LogMalfuncGsmDev,
    LogMalfuncIpDev,
    LogMalfuncAudioCard,
    LogMalfuncIntercomCard,
    LogMalfuncHomeAutomation,
    LogMalfuncCtrlUnitAuxFuse,
    LogMalfuncCtrlUnitBusFuse,
    LogMalfuncCtrlUnitSiren,
    LogMalfuncCtrlUnitLowVoltage,
    LogMalfuncCtrlUnitBus,
    LogMalfuncTelephoneLine,
    LogMalfuncGsmNetwork,
    LogMalfuncGprsNetwork,
    LogMalfuncDateLoss,
    LogMalfuncNotHandled
} eLogSubEventDetailTypeAlarmMalfunction;

//Tipi raggruppamenti allarmi aree e sistema
typedef enum {
    LogEventGroupOrAlarmZone,
    LogEventGroupOrAlarmZoneTamperZone,
    LogEventGroupOrAlarmTamper,
    LogEventGroupOrAlarmZoneTamper,
    LogEventGroupOrAlarmBusExpTamper,
    LogEventGroupOrAlarmGeneral,
    LogEventGroupOrAlarmMalfunct,
    LogEventGroupOrAlarmBusExp,
    LogEventGroupOrAlarmNotHandled
} eLogSubEventDetailTypeAlarmOrAlarm;

//Tipi inserimenti aree e sistema
typedef enum {
    LogActivationTotal,
    LogActivationPart1,
    LogActivationPart2,
    LogActivationEffective,
    LogActivationOff,
    LogActivationPart1AndPart2,
    LogActivationNotHandled
} eLogSubEventDetailTypeActivation;

//Tipi tempistiche aree e sistema
typedef enum {
    LogTimerEntryExit,
    LogTimerExit,
    LogTimerLastExit,
    LogTimerEntry1,
    LogTimerEntry2,
    LogTimerPreAlarm,
    LogTimerWarning,
    LogTimerNotHandled
} eLogSubEventDetailTypeTimers;

//Eventi di area generici
typedef enum {
    LogGenericInactivity,
    LogGenericNotReadyExtActivation,
    LogGenericLogin,
    LogGenericZoneTest,
    LogGenericZoneExclusion,
    LogGenericNotReady,
    LogGenericNotReadyForLevel2,
    LogGenericNotReadyForLevel3,
    LogGenericChime,
    LogGenericEventKey1,
    LogGenericEventKey2,
    LogGenericUserLogged,
    LogGenericForcedLevel2,
    LogGenericForcedLevel3,
    LogGenericNegligence,
    LogGenericAlarmMemory,
    LogGenericResetAlarm,
    LogGenericStopAlarm,
    LogGenericStopPhoneCalls,
    LogGenericTimedActivationError,
    LogGenericAutomAreasReset,
    LogGenericTechProgramming,
    LogGenericCtrlUnitInService,
    LogGenericSysAlarmReset,
    LogGenericSysAlarmStop,
    LogGenericSysPhoneCallsStop,
    LogGenericDateTimeChange,
    LogGenericSiteDataChange,
    LogGenericAutomSysReset,
    LogGenericBatteryTest,
    LogGenericOutgoingPhoneComm,
    LogGenericDigitalCommPeriodicalTest,
    LogGenericDigitalCommManualTest,
    LogGenericSmsTxOk,
    LogGenericEmailTxOk,
    LogGenericMmsTxOk,
    LogGenericIngoingPhoneComm,
    LogGenericCauseActivationBlocked,
    LogGenericPhoneCommOk,
    LogGenericDigitalCommFailed,
    LogGenericDigitalCommOk,
    LogGenericParameterChanged,
    LogGenericFirmwareUpdate,
    LogGenericEmailFailed,
    LogGenericNotHandled
} eLogSubEventDetailTypeGeneric;

//Tipo eventi di zona
typedef enum {
    LogSubEventZoneNotSpecified
} eLogSubEventTypeZone;

//Eventi di zona
typedef enum {
    LogZoneFollowStatus,
    LogZoneAlarm,
    LogZoneMalfunction,
    LogZoneTamper,
    LogZoneStatus,
    LogZoneActive,
    LogZoneChime,
    LogZoneExcluded,
    LogZoneIsolated,
    LogZoneAbsent,
    LogZoneTest,
    LogZoneBlocked,
    LogZoneBattery,
    LogZoneSupervision,
    LogZoneInactivity,
    LogZoneInhbit,
    LogZoneNotHandled
} eLogSubEventDetailTypeZone;

//Tipo eventi di App
typedef enum {
    LogSubEventAppNotSpecified
} eLogSubEventTypeApp;

//Eventi di APP
typedef enum {
    LogAppKeyCodeError,
    LogAppKeyCodeOk,
    LogAppBlocked,
    LogAppKeyEvent1,
    LogAppKeyEvent2,
    LogAppNotHandled
} eLogSubEventDetailTypeApp;

//Tipo eventi di Tastiera
typedef enum {
    LogSubEventKeyboardNotSpecified
} eLogSubEventTypeKeyboard;

//Eventi di Tastiera
typedef enum {
    LogKeyboardKeyCodeError,
    LogKeyboardKeyCodeOk,
    LogKeyboardBlocked,
    LogKeyboardKeyEvent1,
    LogKeyboardKeyEvent2,
    LogKeyboardNotHandled
} eLogSubEventDetailTypeKeyboard;

//Tipo eventi di Lettore RFID
typedef enum {
    LogSubEventRfidNotSpecified
} eLogSubEventTypeRfid;

//Eventi di Lettore RFID
typedef enum {
    LogRfidKeyError,
    LogRfidKeyOk,
    LogRfidBlocked,
    LogRfidNotHandled
} eLogSubEventDetailTypeRfid;

//Tipo eventi di Safetouch
typedef enum {
    LogSubEventSafetouchNotSpecified
} eLogSubEventTypeSafetouch;

//Eventi di Safetouch
typedef enum {
    LogSafetouchKeyCodeError,
    LogSafetouchKeyCodeOk,
    LogSafetouchBlocked,
    LogSafetouchKeyEvent1,
    LogSafetouchKeyEvent2,
    LogSafetouchNotHandled
} eLogSubEventDetailTypeSafetouch;

//Tipo eventi di Generic Touch
typedef enum {
    LogSubEventGenericTouchNotSpecified
} eLogSubEventTypeGenericTouch;

//Eventi di Generic Touch
typedef enum {
    LogGenTouchKeyCodeError,
    LogGenTouchKeyCodeOk,
    LogGenTouchBlocked,
    LogGenTouchKeyEvent1,
    LogGenTouchKeyEvent2,
    LogGenTouchNotHandled
} eLogSubEventDetailTypeGenTouch;

//Tipo eventi di Controllo Remoto
typedef enum {
    LogSubEventRemoteCommNotSpecified
} eLogSubEventTypeRemoteComm;

//Eventi di Controllo Remoto
typedef enum {
    LogRemoteCommKeyCodeError,
    LogRemoteCommKeyCodeOk,
    LogRemoteCommWebBlocked,
    LogRemoteCommPcBlocked,
    LogRemoteCommSmsBlocked,
    LogRemoteCommAudioCardBlocked,
    LogRemoteCommHomeAutomationBlocked,
    LogRemoteCommNotHandled
} eLogSubEventDetailTypeRemoteComm;

//Tipo eventi di Utente
typedef enum {
    LogSubEventUserNotSpecified
} eLogSubEventTypeUser;

// Eventi di Utente
typedef enum {
    LogUserKnown,
    LogUserExtraTime,
    LogUserNotHandled
} eLogSubEventDetailTypeUser;

//Tipo eventi di Radio Comando
typedef enum {
    LogSubEventRadioCtrlNotSpecified
} eLogSubEventTypeRadioCtrl;

// Eventi Radio Comando
typedef enum {
    LogRadioCommandKeyEvent1,
    LogRadioCommandKeyEvent2,
    LogRadioCommandNotHandled
} eLogSubEventDetailTypeRadioCommand;

//Tipo eventi di Gruppi
typedef enum {
    LogSubEventGroupsNotSpecified
} eLogSubEventTypeGroups;

//Tipo eventi di Timer
typedef enum {
    LogSubEventTimerNotSpecified
} eLogSubEventTypeTimer;

// Eventi Timer
typedef enum {
    LogTimerEventOnOff,
    LogTimerEventNotHandled
} eLogSubEventDetailTypeTimer;

//Tipo eventi Uscita
typedef enum {
    LogSubEventOutputNotSpecified
} eLogSubEventTypeOutput;

// Eventi Uscita
typedef enum {
    LogOutputOnOff,
    LogOutputManualOnOff,
    LogOutputNotHandled
} eLogSubEventDetailTypeOutput;

//Tipo eventi cambio codice
typedef enum {
    LogSubEventCodeChangeNotSpecified
} eLogSubEventTypeCodeChange;

// Eventi cambio codice
typedef enum {
    LogCodeChangeCodeAdded,
    LogCodeChangeCodeDeleted,
    LogCodeChangeCodeModified,
    LogCodeChangeKeyAdded,
    LogCodeChangeKeyDeleted,
    LogCodeChangeKeyModified,
    LogCodeChangeNotHandled
} eLogSubEventDetailTypeCodeChange;

//Tipo eventi di ripristino
typedef enum {
    LogSubEventDbReactivationNotSpecified
} eLogSubEventTypeDbReactivation;

// Eventi di ripristino
typedef enum {
    LogDbReactResetSoft,
    LogDbReactResetPartial,
    LogDbReactResetTotal,
    LogDbReactCopyDefault,
    LogDbReactNotHandled
} eLogSubEventDetailTypeDbReactivation;

//Tipo eventi bridge domotico
typedef enum {
    LogSubEventHomeAutomationBridgeNotSpecified
} eLogSubEventTypeHomeAutomationBridge;

// Eventi bridge domotico
typedef enum {
    LogHomeAutomBridgeKeyCodeError,
    LogHomeAutomBridgeKeyCodeOk,
    LogHomeAutomBridgeBlocked,
    LogHomeAutomBridgeKeyEvent1,
    LogHomeAutomBridgeKeyEvent2,
    LogHomeAutomBridgeNotHandled
} eLogSubEventDetailTypeHomeAutomationBridge;

//Tipo di eventi azione
typedef enum {
    LogActionNone,
    LogActionGetAreeToActivate,
    LogActionActivation,
    LogActionAlarmCmd,
    LogActionGenericCommand,
    LogActionActivationFromTimer,
    LogActionOutputActivation,
    LogActionOutputDeactivation,
    LogActionOutputToggle,
    LogActionOnZone,
    LogActionNotHandled
} eLogSubEventTypeAction;

//Enumerato per la definizione del tipo di inserimento
typedef enum {
    LogActTypeScenarioError,
    LogActTypeScenario1,
    LogActTypeScenario2,
    LogActTypeScenario3,
    LogActTypeScenario4,
    LogActTypeScenario5,
    LogActTypeScenario6,
    LogActTypeScenario7,
    LogActTypeScenario8,
    LogActTypeScenario9,
    LogActTypeScenario10,
    LogActTypeScenario11,
    LogActTypeScenario12,
    LogActTypeScenario13,
    LogActTypeScenario14,
    LogActTypeScenario15,
    LogActTypeScenario16,
    LogActTypeScenario17,
    LogActTypeScenario18,
    LogActTypeScenario19,
    LogActTypeScenario20,
    LogActTypeScenario21,
    LogActTypeScenario22,
    LogActTypeScenario23,
    LogActTypeScenario24,
    LogActTypeScenario25,
    LogActTypeScenario26,
    LogActTypeScenario27,
    LogActTypeScenario28,
    LogActTypeScenario29,
    LogActTypeScenario30,
    LogActTypeScenario31,
    LogActTypeScenario32,
    LogActTypeTotalActivation,        // Inserimento delle aree assegnate all'utente e controllabili dal dispositivo di controllo
    LogActTypeTotalDeactivation,      // Disinserimento delle aree assegnate all'utente e controllabili dal dispositivo di controllo
    LogActTypeDeviceActivation,       // Inserimento personalizzato dall'utente tramite un dispositivo di controllo
    LogActTypeNotHandled
} eLogSubEventDetailActivationType;

typedef enum {
    LogAlarmCmdBurglar,
    LogAlarmCmdH24,
    LogAlarmCmdFire,
    LogAlarmCmdPanic,
    LogAlarmCmdRobbery,
    LogAlarmCmdFlood,
    LogAlarmCmdGas,
    LogAlarmCmdMedicalEmergency,
    LogAlarmCmdDuress,
    LogAlarmCmdNotHandled
} eLogSubEventDetailAlarmCmd;

typedef enum {
    LogCommandKeyEvent1,
    LogCommandKeyEvent2,
    LogCommandExtraTime,
    LogCommandAlarmStop,
    LogCommandAlarmReset,
    LogCommandTelCallReset,
    LogCommandCtrlUnitReset,
    LogCommandAssistanceReq,
    LogCommandTechnicianEnable,
    LogCommandCodeConfirmation,
    LogCommandKeyConfirmation,
    LogCommandLoginUpgrade,
    LogCommandManufacturerEnable,
    LogCommandBypassAuto,
    LogCommandFireReset,
    LogCommandDefaultSystem,
    LogCommandNotHandled
} eLogSubEventDetailGenCommand;

typedef enum {
    LogActFromTimersNotSpecified
} eLogSubEventDetailActFromTimers;

typedef enum {
    LogActionOnZoneExclude,
    LogActionOnZoneInclude,
    LogActionOnZoneIsolate,
    LogActionOnZoneEnable,
    LogActionOnZoneNotHandled
} eLogSubEventDetailActionOnZone;

// enumerazione agenti / periferiche
typedef enum {
    AgentNone,
    AgentCtrlUnit,
    AgentExp4o,
    AgentExp8io,
    AgentRadioCon,
    AgentGsmBus,
    AgentSiren,
    AgentPowerSupplier,
    AgentBusIsolator,
    AgentPalIp,
    AgentHomeAutomationBridge,
    AgentKeyboard,
    AgentSafetouch,
    AgentPlanux,
    AgentRfid,
    AgentThumbprint,
    AgentMinitouch,
    AgentPhoneApp,
    AgentPcTouch,
    AgentIconaVip,
    AgentIconaSbc,
    AgentIpCtrlUnit,
    AgentIpWeb,
    AgentIpPc,
    AgentUsb,
    AgentPstnModem,
    AgentGsmModem,
    AgentAudioMenuPstn,
    AgentAudioMenuGsm,
    AgentSmsGsm,
    AgentHomeAutomation,
    AgentZone,
    AgentKey,
    AgentAreas,
    AgentRadioController,
    AgentUser,
    AgentTimer,
    AgentAutoActivation,
    AgentOutput,
    AgentPhoneBook,
    AgentContactIdBook,
    AgentHomeAutomationHomeServer,
    AgentMaxiVip,
    AgentV5tpSafetouch,
    AgentMaxiShman,
    AgentNvr,
    AgentNotHandled
} eAgentType;

typedef enum {
    LogExtraInfoTypeAreas,
    LogExtraInfoTypeZone,
    LogExtraInfoTypePhoneApp,
    LogExtraInfoTypeKeyboard,
    LogExtraInfoTypeRfid,
    LogExtraInfoTypeSafetouch,
    LogExtraInfoTypeGenTouch,
    LogExtraInfoTypeDomoSupervisor,
    LogExtraInfoTypeUser,
    LogExtraInfoTypeRadioCommand,
    LogExtraInfoTypeEventsGroup,
    LogExtraInfoTypeTimer,
    LogExtraInfoTypeOutput,
    LogExtraInfoTypeKey,
    LogExtraInfoNotPresent,
    LogExtraInfoTypeNotHandled
} eLogExtraInfoType;

typedef enum
{
    CfgParamsGroupAll,
    CfgParamsGroupSystem,
    CfgParamsGroupArea,
    CfgParamsGroupAutoActArea,
    CfgParamsGroupZone,
    CfgParamsGroupScenario,
    CfgParamsGroupOutput,
    CfgParamsGroupRadioConc,
    CfgParamsGroupPalIp,
    CfgParamsGroupTimers,
    CfgParamsGroupKeyboard,
    CfgParamsGroupRfidReader,
    CfgParamsGroupSafetouch,
    CfgParamsGroup8ioExp,
    CfgParamsGroupGenTouch,
    CfgParamsGroupApp,
    CfgParamsGroupHomeAutomBridge,
    CfgParamsGroupPowerSupplier,
    CfgParamsGroupBusIsolator,
    CfgParamsEventsGroup,
    CfgParamsGroupEthCard,
    CfgParamsGroupNvr,
    CfgParamsGroupUsers,
    CfgParamsGroupInstallers,
    CfgParamsGroupCodeType,
    CfgParamsGroupRfidKey,
    CfgParamsGroupNamesDirectory,
    CfgParamsGroupRadioCtrls,
    CfgParamsGroupService,
    CfgParamsGroupHoliday,
    CfgParamsGroupTimeProg,
    CfgParamsGroupProfiles,
    CfgParamsGroupGlobalSettings,
    CfgParamsGroupKeypads,
    CfgParamsGroupCCTVDev,
    CfgParamsGroupARCPhonebook,
    CfgParamsGroupArmDiarm,
    CfgParamsGroupOptionsMenu,
    CfgParamsGroupEventReportSettings,
    CfgParamsGroupEventReportOptions,
    CfgParamsGroupAudioOptions,
    CfgParamsGroupTCPIPSettings,
    CfgParamsGroupEventLog,
    CfgParamsGroupMacro,
    CfgParamsGroupLastEvents,
    CfgParamsGroupVoiceMSG,
    CfgParamsGroupEventsMap,
    CfgParamsGroupM3GSettings,
    CfgParamsGroupEmailSettings,
    CfgParamsGroupAPSettings,
    CfgParamsGroupOutputGlobalSettings,
    CfgParamsGroupDbSignatures,
    CfgParamsGroupNotHandled
} eCfgParamsGroup;

typedef enum
{
    CfgParamsSingleReqDeviceConf
} eCfgParamsSingleReq;

typedef enum {
    CfgLangDefault,
    CfgLangItalian,
    CfgLangSpanish,
    CfgLangFrench,
    CfgLangEnglish,
    CfgLangDutch,
    CfgLangGerman,
    CfgLangNotHandled
} eCfgLanguage;

typedef enum {
    CfgBaudRate9600,
    CfgBaudRate38400,
    CfgBaudRate56700,
    CfgBaudRate115200,
    CfgBaudRateNotHandled
} eCfgRs485BaudRate;

typedef enum {
    ActTypeNone,
    ActTypeTot,
    ActTypeP12,
    ActTypeP1,
    ActTypeP2,
    ActTypeDeactivation,
    ActTypeNotHandled
} eActType;

typedef enum
{
    StatusAbsent,
    StatusActive,
    StatusIsolated,
    StatusNotAvailable,
    StatusNotHandled
} eDeviceStatus;

//Tipi di bilanciamento
typedef enum {
    BalancNormallyOpen,
    BalancNormallyClose,
    BalancSingle,
    BalancDoubleBalancing,
    BalancTriple,
    BalancDoubleZone,
    BalancDoubleWithEol,
    BalancTripleWithMasking,
    BalancNotHandled
} eZoneBalancing;

typedef enum {
    KeyColorGrey,
    KeyColorBlue,
    KeyColorYellow,
    KeyColorOrange,
    KeyColorRed,
    KeyColorGreen,
    KeyColorGreyBlue,
    KeyColorGreyYellow,
    KeyColorGreyOrange,
    KeyColorGreyRed,
    KeyColorGreyGreen,
    KeyColorNotValid,
    KeyColorNotHandled
} eRfidKeyColor;

//Enumerato per la definizione del tipo di inserimento
typedef enum {
    TimerId1,
    TimerId2,
    TimerId3,
    TimerId4,
    TimerId5,
    TimerId6,
    TimerId7,
    TimerId8,
    TimerId9,
    TimerId10,
    TimerId11,
    TimerId12,
    TimerId13,
    TimerId14,
    TimerId15,
    TimerId16,
    TimerIdNotHandled
} eTimerId;

/* Enumerati per la decodifica Notifiche App */

typedef enum {
    AppNotificationNew,
    AppNotificationReset,
    AppNotificationAck,
    AppNotificationExtended,
    AppNotificationNotHandled
} eAppNotificationType;

typedef enum
{
    AppNotifGroupNone = 0,
    AppNotifGroupZones,
    AppNotifGroupAreasActivation,
    AppNotifGroupAreasAlarm,
    AppNotifGroupSysTamper,
    AppNotifGroupSysAnomaly,
    AppNotifGroupSysGeneric,
    AppNotifGroupKeyboard,
    AppNotifGroupRfidReader,
    AppNotifGroupRemoteCom,
    AppNotifGroupTimer,
    AppNotifGroupOutputs,
    AppNotifGroupUsers,
    AppNotifGroupActivationAction,
    AppNotifGroupAlarmAction,
    AppNotifGroupCommandsAction,
    AppNotifGroupOutputsAction,
    AppNotifGroupZonesAction,
    AppNotifGroupSafeTouch,
    AppNotifGroupGenericTouch,
    AppNotifGroupCodeChange,
    AppNotifGroupResetDatabase,
    AppNotifGroupMobileApp,
    AppNotifGroupAHomeAutomBridge,
    AppNotifGroupAlarm,
    AppNotifGroupCPMainFault,
    AppNotifGroupCPBatteryFault,
    AppNotifGroupDevicesBatteryFault,
    AppNotifGroupPartialActivationAction,
    AppNotifGroupDesactivationAction,
    AppNotifGroupForcedActivationAction,
    AppNotifGroupSysOperation
} eAppNotificationGroup;

typedef enum
{
    MediaCommandImageCheckDownload,
    MediaCommandImageStartDownload,
    MediaCommandImageStopDownload,
    MediaCommandMovieCheckStream,
    MediaCommandMoviePlayStream,
    MediaCommandMovieStopStream,
    MediaCommandNotHandled
} eMediaCommand;

typedef enum
{
    LiveViewCreateUnit,
    LiveViewPlayStream,
    LiveViewStopStream,
    LiveViewRemoveUnit,
    LiveViewNotHandled
} eLiveViewCommand;

// App association status
typedef enum
{
    Free = 0, // Not used from 2.13.x
    PinRequested,
    PinOk,
    Registered, // Not used from 2.13.x
    // Used from 2.13.x
    RegisteredAsUser,
    RegisteredAsInstaller,
    FreeForUserRegistration,
    FreeForInstallerRegistration
} eStatusApp;

typedef enum
{
    VideoTimelinesDays,
    VideoTimelinesTimeIntervals,
    VideoTimelinesNotHandled
} eVideoTimelinesCommand;

typedef enum
{
    MediaListNone,
    MediaListAlarm,
    MediaListContinuous,
    MediaListScheduled,
    MediaListArming,
    MediaListNotHandled
} eMediaListType;

typedef enum
{
    ActionCloudPinRequest,
    ActionCloudDevStateList,
    ActionCloudWrMailOnline
} eActionCloudType;

#endif // LIBEVENT_SAFE_H
