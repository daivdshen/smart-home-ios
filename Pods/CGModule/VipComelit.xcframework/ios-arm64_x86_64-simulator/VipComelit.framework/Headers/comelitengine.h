/**
 *
 * @file comelitengine.h
 *
 * Single Point Of Contact (SPOC) per l'accesso alla libreria e la vita di
 * un sistema Comelit.
 *
 * La libreria, e di conseguenza la suddetta classe, espongono i metodi atti
 * a comunicare con i vari elementi della famiglia Comelit.
 * L'utilizzo di questa libreria presuppone una minima conoscenza dei vari
 * mondi che possono comporre il sistema Comelit.
 *
 * ComelitEngine implementa il pattern Singleton, quindi esiste in singola
 * istanza.
 *
 * Le richieste verso la libreria vengono fatte attraverso chiamate a metodi.
 * Le comunicazioni provenienti dalla libreria vengono fornite in formato Json
 * su di un socket in localhost, il cui numero di porta deve essere fornito in
 * fase di inizializzazione della libreria.
 *
 * @attention L'inizializzazione della libreria non va a buon fine se non riesce a
 *            collegarsi alla porta del socket fornita
 *
 * @author Eros Magri, Claudio Bonomi
 *
 ***************************************************************************/

#ifndef COMELITENGINE_H
#define COMELITENGINE_H

#ifdef __GNUC__
#define DEPRECATED(func) func __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED(func) __declspec(deprecated) func
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED(func) func
#endif

#include <stdbool.h>
#include <time.h>
#include "cfg.h"
#include <string>
#include <functional>
#include <atomic>
#include <comelitutils/utils.h>

#include "libdefs.h"
#include "logger.h"

using std::string;
using namespace LibComelit;

/// Callback defined from userspace, to be called whenever an event occours, used only in callback mode
typedef std::function<void (void* obj, std::string payload, void* arg)>	CallbackOnEvt;

class ComelitEngine
{
public:

    // General Enum ======================================================================

    /// connection mode to setup systems connection
    enum SysConnectionMode {
        SYSTEMS_CONN_MODE_UNASSIGNED =  0,
        SYSTEMS_CONN_MODE_LOCAL      =  0x00000001,  ///< Systems connection in local mode
        SYSTEMS_CONN_MODE_VIPER      =  0x00000002,  ///< Systems connection in viper mode
    };

    /// unit type used in common methods across Comelit Research areas
    enum gUnitType{
        GU_VIP,
        GU_SAFE,
        GU_TVCC,
        GU_RTSP,
        GU_SAFE_PLAYBACK
    };

    /// direction of a media source used in common methods
    enum gDir{
        GD_RX,
        GD_TX
    };

    /// media source handled by libComelit
    enum gMediaType{
        GM_AUDIO,
        GM_VIDEO
    };

    /// engine parameters setup
    enum EngineParams
    {
        ENGINE_IP_ADDRESS,              // expect char* value
        ENGINE_IP_NETMASK,              // expect char* value
        ENGINE_IP_LIMITED_BROADCAST,    // expect int*  value
        ENGINE_RTP_ADU_MAX_SIZE,        // expect ulong* value
        ENGINE_CALL_LOG,                // expect callLog* value
        ENGINE_RUN_TIMER,               // expect int* value
        ENGINE_VM_SERVER                // expect vmServer* value
    };

    // Enum for VipUnit Messages =========================================================

    /// Type of call you want to make
    enum CallType{ T_CFP = 0,  ///< CFP (out-of-door call)
                   T_NORMAL,   ///< Normal door entry call
                   T_INTERCOM, ///< Intercom call
                   T_TVCC,     ///< Video surveillance call
                   T_TEST,     ///< Connectivity/bandwidth test call
                   T_VIP       ///< ViP calls
    };
    /// Call Priority Flags (normal call has no flags F_NONE)
    enum CallFlag{ F_PRIOCALL1 = 0,
                   F_PRIOCALL2,
                   F_PRIOCALL3,
                   F_PRIOCALL4,
                   F_PRIOCALL_ALL,
                   F_CAPTURED,
                   F_DIVERTED,
                   F_GROUP,
                   F_EXTERNAL,
                   F_NONE
    };

    enum PacketType{
        PT_MJPEG,
        PT_H264
    };

    enum VipUnitCapability{
        VU_CAPAB_AUDIO_DST =  0x00000001,  ///< Can receive an audio flow
        VU_CAPAB_AUDIO_SRC =  0x00000002,  ///< Can send an audio flow
        VU_CAPAB_VIDEO_DST =  0x00000004,  ///< Can receive a video flow
        VU_CAPAB_VIDEO_SRC =  0x00000008,  ///< Can send a video flow
        VU_CAPAB_OPENDOOR  =  0x00000010,  ///< Can open a door
        VU_CAPAB_MSTREAM   =  0x00000020,  ///< Can send multiple video/audio streams
        VU_CAPAB_NONE      =  0
    };

    enum VipUnitRole{
        VU_ROLE_INTUNIT,
        VU_ROLE_PORTER,
        VU_ROLE_ACTUATOR
    };

    enum AECParam{
        AEC_DELAY_ESTIMATED_MS,                 ///< delay time estimated between in and out audio pcks on which do echo cancellation (default 320ms)
        AEC_CONFORT_NOISE_ENABLE,               ///< enable comfort noise      [0;1] (default AEC_TRUE)        see #AecMode
        AEC_ECHO_LEVEL,                         ///< echo cancellation level   [0;4] (default AEC_ECHO_MODE_3) see #AecEchoMode
        NS_NOISE_SUPPRESSION_SPEAKER_ENABLE,    ///< enable noise suppression for speaker
        NS_NOISE_SPEAKER_MODE,                  ///< noise suppression type
        NS_NOISE_SUPPRESSION_MIC_ENABLE,        ///< enable noise suppressione for mic
        NS_NOISE_MIC_MODE,                      ///< noise suppression type
        AGC_MICROPHONE_ENABLE,                  ///< enable agc for mic
        AGC_MIN_LEVEL,                          ///< minimum agc level
        AGC_MAX_LEVEL,                          ///< maximum agc level
        AGC_TARGET_LEVEL_DBFS,                  ///< Change of this parameter will set the target peak |level| (or envelope) of the AGC in dBFs [0;31]
        AGC_COMPRESSION_GAIN_DB,                ///< imposta livello compressione dB    [0;90]
        AGC_LIMITER_ENABLE,                     ///< enable agc limiter
        AGC_START_LEVEL                         ///< agc starintg level
    };

    enum AecMode{
        AEC_FALSE = 0,
        AEC_TRUE
    };

    enum AecEchoMode{
        AEC_ECHO_MODE_0 = 0,
        AEC_ECHO_MODE_1,
        AEC_ECHO_MODE_2,
        AEC_ECHO_MODE_3,
        AEC_ECHO_MODE_4
    };

    enum NsMode{
        NS_MILD = 0,
        NS_MEDIUM,
        NS_AGGRESSIVE
    };
    
    enum CallTimeParam {
        CT_ALERTED,             ///< Maximum time in the state calls for Alerting
        CT_OUT_INITIATED,       ///< Maximum waiting time response to the connection request from the destination
        CT_CONNECTED,           ///< Maximum Connection Time
        CT_CLOSE,               ///< Waiting time before sending a release call
        CT_IC_CONNECTED,        ///< Maximum Connection Time in Intercom Calls
        CT_SL_CONNECTED,        ///< Maximum Connection Time in Self Igniction Calls
        CT_PROCEEDING,          ///< Maximum Proceeding Time waiting for alerting
        CT_MAX_ON_CLOSED        ///< Maximum Time waiting for call destroyed
    };

    enum AVBuffParam{
        AV_AUDIO_RX_PCK_BUFFER_LENGTH, ///< Circular Buffer Lengtgh for Audio RX pcks (default 3)
        AV_VIDEO_RX_PCK_BUFFER_LENGTH, ///< Circular Buffer Lengtgh for Video RX pcks (default 12)
        AV_AUDIO_RX_PCK_PRE_BUFFERING, ///< Audio RX packet buffering before beginning the removal from the queue (default 0)
        AV_VIDEO_RX_PCK_PRE_BUFFERING,  ///< Video RX packet buffering before beginning the removal from the queue (default 0)
        AV_RTP_RX_PCK_BUFFER_LENGTH, ///< Circular Buffer Lengtgh for RTP raw RX pcks (default 0 disabled)
        AV_RTP_RX_PCK_PRE_BUFFERING  ///< Video RX packet buffering before beginning the removal from the queue (default 0)
    };

    enum MemoVideoParam{
        VIDEO_MEMORY_MULTIPLE_SAVING_MODE   ///< Allow save and stop multiple video during a call (default false)
    };

    enum ActuatorAction{
        ACT_CMD_AUTO,           ///< Auto Switch
        ACT_CMD_ON,             ///< Switch ON
        ACT_CMD_OFF             ///< Switch OFF
    };

    enum DoorStatus {
        CLOSED  = 0,
        OPEN    = 1
    };

    /// Call paging flag
    enum CallFlag2 {
        F_UNASSIGNED = 0,
        F_PAGING     = 1
    };

    // Enumerativi per i messaggi verso la SafeUnit ===========================================
    
    /// Tipo di dispositivo SAFE
    enum SafeDeviceType{
        SAFE_DEVICE_APP = 0,        //App Device
        SAFE_DEVICE_PCTOUCH,        //PC Touch Device
        SAFE_DEVICE_PAL_IP,         //Vedocctv Device
        SAFE_DEVICE_ZONE,           //Alarm zones (wired + radio)
        SAFE_DEVICE_HS,             //HomeServer Device
        SAFE_DEVICE_MAXI_VIP,       //MAXI Manager Vip
        SAFE_DEVICE_CONTROL_PANEL,  //Control Panel Device
        SAFE_DEVICE_SAFETOUCH,      //SAFETOUCH Device
        SAFE_DEVICE_V5TP_SAFETOUCH, //V5TP SAFETOUCH Device
        SAFE_DEVICE_NOBODY,         //equivalent of agent type 0x00 -> AGENTE_NESSUNO
        SAFE_DEVICE_MAXI_SHMAN,     //MAXI Manager SHMAN
        //TODO...
        MAX_SAFE_DEVICE_TYPE    //Do NOT use!!!
    };
    
    /// Gruppi parametri di configurazione della centrale
    enum SafeCfgParamsGroupType {
        SAFE_CFG_GROUP_ALL,
        SAFE_CFG_GROUP_SYS,
        SAFE_CFG_GROUP_AREA,
        SAFE_CFG_GROUP_AUTO_ACT_AREA,
        SAFE_CFG_GROUP_ZONE,
        SAFE_CFG_GROUP_SCENARIO,
        SAFE_CFG_GROUP_OUTPUT,
        SAFE_CFG_GROUP_RADIO_CONC,
        SAFE_CFG_GROUP_PAL_IP,
        SAFE_CFG_GROUP_TIMERS,
        SAFE_CFG_GROUP_KEYBOARD,
        SAFE_CFG_GROUP_RFID_READER,
        SAFE_CFG_GROUP_SAFETOUCH,
        SAFE_CFG_GROUP_8IO_EXP,
        SAFE_CFG_GROUP_GEN_TOUCH,
        SAFE_CFG_GROUP_APP,
        SAFE_CFG_GROUP_HOME_AUTOM_BRIDGE,
        SAFE_CFG_GROUP_POWER_SUPPLIER,
        SAFE_CFG_GROUP_BUS_ISOL,
        SAFE_CFG_GROUP_EVENTS_GROUP,
        SAFE_CFG_GROUP_ETH_CARD,
        SAFE_CFG_GROUP_NVR,
        SAFE_CFG_GROUP_USERS,
        SAFE_CFG_GROUP_INSTALLERS,
        SAFE_CFG_GROUP_CODE_TYPE,
        SAFE_CFG_GROUP_RFID_KEY,
        SAFE_CFG_GROUP_NAMES_DIR,
        SAFE_CFG_GROUP_RADIO_CTRLS,
        SAFE_CFG_GROUP_SERVICE,
        SAFE_CFG_GROUP_HOLIDAY,
        SAFE_CFG_GROUP_TIME_PROGRAMS,
        SAFE_CFG_GROUP_USER_PROFILES,
        SAFE_CFG_GROUP_GLOBAL_SETTINGS,
        SAFE_CFG_GROUP_KEYPADS,
        SAFE_CFG_GROUP_CCTV_DEVICE,
        SAFE_CFG_GROUP_ARC_PHONEBOOK,
        SAFE_CFG_GROUP_ARM_DISARM,
        SAFE_CFG_GROUP_OPTIONS_MENU,
        SAFE_CFG_GROUP_EVENT_REPORT_SETTINGS,
        SAFE_CFG_GROUP_EVENT_REPORT_OPTIONS,
        SAFE_CFG_GROUP_AUDIO_OPTIONS,
        SAFE_CFG_GROUP_TCP_IP_SETTINGS,
        SAFE_CFG_GROUP_EVENT_LOG,
        SAFE_CFG_GROUP_MACRO,
        SAFE_CFG_GROUP_LAST_EVENT,
        SAFE_CFG_GROUP_VOICE_MSG,
        SAFE_CFG_GROUP_EVENTS_MAP,
        SAFE_CFG_GROUP_M3G_SETTINGS,
        SAFE_CFG_GROUP_EMAIL_SETTINGS,
        SAFE_CFG_GROUP_AP_SETTINGS,
        SAFE_CFG_GROUP_OUTPUT_GLOBAL_SETTINGS,
        SAFE_CFG_GROUP_DB_SIGNATURES,
        SAFE_CFG_MAX                    //Do NOT use!!!
    };
    
    /// Gruppi parametri di informazioni real-time della centrale
    enum SafeRtParamsGroupType {
        SAFE_RT_AREAS_STATUS,            //Lettura stato real time aree
        SAFE_RT_ZONES_STATUS,            //Lettura stato real time zone
        SAFE_RT_OUTPUTS_STATUS,          //Lettura stato real time uscite
        SAFE_RT_ZONES_STATUS_SPEC,       //Lettura stato specifico real time zone
        SAFE_RT_PARAM_LOGGED_USER_ID,	//Lettura stato real time id utente loggato
        SAFE_RT_PARAM_READ_LOG,			//Lettura allarme/evento tramite navigazione log allarmi/eventi
        SAFE_RT_PARAM_ZONE_ALARMS,      //Lettura informazioni realtime allarmi zone
        SAFE_RT_PARAM_SYS_TAMPER,       //Lettura informazioni realtime sabotaggi sistema
        SAFE_RT_PARAM_SYS_ANOMALIES,    //Lettura informazioni realtime anomalie sistema
        SAFE_RT_PARAM_SYS_TIMERS,       //Lettura informazioni realtime tempi di sistema
        SAFE_RT_PARAM_SYS_GENERIC,      //Lettura informazioni realtime varie di sistema
        SAFE_RT_PARAM_PALIP_STATUS,		//Lettura stato real time palip
        SAFE_RT_PARAM_READ_TEXT_LOG,    //Lettura testo allarme/evento tramite navigazione log allarmi/eventi
        SAFE_RT_PARAM_READ_TEXT_MULTI_CAMERA_LOG, //Read Text Log with Multi Camera
        SAFE_RT_PARAM_READ_CCTV_STATUS,
        SAFE_RT_MAX                     //Do NOT use!!!
    };
    
    /// Enumerato per la definizione del tipo specifico di stato zona allarme SAFE
    enum SafeRtSpecZoneStatus {
        SAFE_RT_ZONE_STATUS_OPEN = 1,               // zona aperta, chiusa
        SAFE_RT_ZONE_STATUS_ALARM = (1 << 1),       // zona in allarme, fino al reset
        SAFE_RT_ZONE_STATUS_FAULTY = (1 << 2),      // zona in guasto
        SAFE_RT_ZONE_STATUS_SABOTAGED = (1 << 3),   // zona in sabotaggio zona
        SAFE_RT_ZONE_STATUS_READY = (1 << 4),       // zona pronta / non pronta e allarme
        SAFE_RT_ZONE_STATUS_ACTIVE = (1 << 5),      // zona inserita
        SAFE_RT_ZONE_STATUS_CHIME = (1 << 6),       // zona campanello
        SAFE_RT_ZONE_STATUS_ESCLUDED = (1 << 7),    // zona esclusa
        
        SAFE_RT_ZONE_STATUS_ISOLATED = (1 << 8),    // zona isolata
        SAFE_RT_ZONE_STATUS_NOT_PRESENT = (1 << 9), // zona assente
        SAFE_RT_ZONE_STATUS_TEST = (1 << 10),       // zona in prova
        SAFE_RT_ZONE_STATUS_MASKED = (1 << 11),     // zona mascherata (solo per radio)
        SAFE_RT_ZONE_STATUS_LOW_BATT = (1 << 12),   // batteria bassa (solo per radio)
        SAFE_RT_ZONE_STATUS_SUPERV = (1 << 13),     // supervisione (solo per radio)
    };
    
    ///Enumerato per la definizione del tipo parametro db singolo rischiesto
    enum SafeCfgParamSingleType{
        SAFE_CFG_DEVICE,                        //Lettura parametro configurazione del dispositivo
        SAFE_CFG_SINGLE_MAX                     //Do NOT use!!!
    };
    
    ///Enumerato per la definizione del tipo del record del log eventi rischiesto
    enum SafeLogType{
        SAFE_LOG_SECURE,    ///Log eventi
        SAFE_LOG_ALARMS,    ///Log allarmi
        SAFE_LOG_GENERIC,
        SAFE_LOG_FIRST_ZONE_ALARMED,
        SAFE_LOG_MAX        //Do NOT use!!!
    };
    
    enum SafeLogMode{
        SAFE_LOG,
        SAFE_LOG_TEXT,
        SAFE_LOG_MULTI_MEDIA_TEXT,
        SAFE_LOG_MODE_MAX        //Do NOT use!!!
    };

    enum SafeLogMediaReq
    {
        SAFE_LOG_MEDIA_IMAGES_CHECK_DWL,
        SAFE_LOG_MEDIA_IMAGES_DWL,
        SAFE_LOG_MEDIA_IMAGES_STOP_DWL,
        SAFE_LOG_MEDIA_MOVIE_CHECK_STREAM,
        SAFE_LOG_MEDIA_MOVIE_PLAY_STREAM,
        SAFE_LOG_MEDIA_MOVIE_STOP_STREAM,
        SAFE_LOG_MEDIA_MAX  //Do NOT use!!!
    };
    
    enum SafeLiveViewReq
    {
        SAFE_LIVE_VIEW_CREATE_UNIT,
        SAFE_LIVE_VIEW_PLAY_STREAM,
        SAFE_LIVE_VIEW_STOP_STREAM,
        SAFE_LIVE_VIEW_REMOVE_UNIT,
        SAFE_LIVE_VIEW_MAX  //Do NOT use!!!
    };
    
    ///Enumerato per la definizione del tipo di azione
    enum SafeActionType{
        SAFE_ACTION_GET_AREAS_TO_ACT,   // richiesta delle aree che verranno inserite all'esecuzione dell'inserimento specificato in seguito
        SAFE_ACTION_ACTIVATION,         // Esecuzione di un inserimento/disinserimento
        SAFE_ACTION_ALARM,              // Invio di un evento di allarme
        SAFE_ACTION_COMMAND,            // Invio di un comando
        SAFE_ACTION_ACT_FROM_TIMER,     // Azione di inserimento eseguita da un timer (supervisore)
        SAFE_ACTION_ACTIVATE_OUTPUT,    // Attivazione di un'uscita
        SAFE_ACTION_DEACTIVATE_OUTPUT,  // Disattivazione di un'uscita
        SAFE_ACTION_TOGGLE_OUTPUT,      // Bascula di un'uscita
        SAFE_ACTION_ON_ZONE,            // Esclude/include/isola/attiva una zona
        SAFE_ACTION_TYPE_MAX            // Do NOT use!!!
    };
    
    ///Enumerato per la definizione tipo di allarme SAFE
    enum SafeAlarmType{
        SAFE_ALARM_BURGLAR,
        SAFE_ALARM_H24,
        SAFE_ALARM_FIRE,
        SAFE_ALARM_PANIC,
        SAFE_ALARM_ROBBERY,
        SAFE_ALARM_FLOOD,
        SAFE_ALARM_GAS,
        SAFE_ALARM_MEDICAL_EMERGENCY,
        SAFE_ALARM_DURESS,
        SAFE_ALARM_MAX                //Do NOT use!!!
    };
    
    ///Enumerato per la definizione tipo di comandi SAFE
    enum SafeCmdType{
        SAFE_CMD_EVENT_KEY_1,           // Viene generato solo l'evento di tasto evento 1 (serve per le uscite)
        SAFE_CMD_EVENT_KEY_2,           // Viene generato solo l'evento di tasto evento 2 (serve per le uscite)
        SAFE_CMD_EXTRA_TIME,            // Richiesta di straordinario
        SAFE_CMD_ALARMS_STOP,           // Consente lo stop di allarmi di sistema
        SAFE_CMD_ALARMS_RESET,          // Consente il reset allarmi di sistema
        SAFE_CMD_TELEPHONE_CALL_RESET,  // Consente all'utente di fermare le telefonate su allarme di sistema
        SAFE_CMD_MAIN_UNIT_RESET,       // Reset Centrale
        SAFE_CMD_ASSISTANCE_REQUEST,    // Genera una richiesta di assistenza
        SAFE_CMD_ENABLE_INSTALLER,      // Abilitazione installatore
        SAFE_CMD_CODE_CONFIRMATION,     // Richiesta di conferma codice su dispositivo di controllo
        SAFE_CMD_KEY_CONFIRMATION,      // Richiesta di conferma chiave su dispositivo di controllo
        SAFE_CMD_UPGRADE_LOGIN,         // Richiesta di upgrade login
        SAFE_CMD_ENABLE_MANUFACTURER,   // Abilitazione produttore
        SAFE_CMD_BYPASS_AUTO,           //
        SAFE_CMD_FIRE_ALARM_RESET,      //
        SAFE_CMD_RESTORE_DEFAULT_SYSTEM,//
        SAFE_CMD_MAX                    // Do NOT use!!!
    };
    
    ///Enumerato per la definizione del tipo di reset allarmi
    enum SafeCmdResetAlarmType{
        RESET_ALL,
        RESET_NO_AREAS,
        RESET_NO_SYS
    };
    
    ///Enumerato per la definizione del tipo di inserimento
    enum SafeActivationType {
        SAFE_CMD_ACT_SCENARIO_ERROR,
        SAFE_CMD_ACT_SCENARIO_1,
        SAFE_CMD_ACT_SCENARIO_2,
        SAFE_CMD_ACT_SCENARIO_3,
        SAFE_CMD_ACT_SCENARIO_4,
        SAFE_CMD_ACT_SCENARIO_5,
        SAFE_CMD_ACT_SCENARIO_6,
        SAFE_CMD_ACT_SCENARIO_7,
        SAFE_CMD_ACT_SCENARIO_8,
        SAFE_CMD_ACT_SCENARIO_9,
        SAFE_CMD_ACT_SCENARIO_10,
        SAFE_CMD_ACT_SCENARIO_11,
        SAFE_CMD_ACT_SCENARIO_12,
        SAFE_CMD_ACT_SCENARIO_13,
        SAFE_CMD_ACT_SCENARIO_14,
        SAFE_CMD_ACT_SCENARIO_15,
        SAFE_CMD_ACT_SCENARIO_16,
        SAFE_CMD_ACT_SCENARIO_17,
        SAFE_CMD_ACT_SCENARIO_18,
        SAFE_CMD_ACT_SCENARIO_19,
        SAFE_CMD_ACT_SCENARIO_20,
        SAFE_CMD_ACT_SCENARIO_21,
        SAFE_CMD_ACT_SCENARIO_22,
        SAFE_CMD_ACT_SCENARIO_23,
        SAFE_CMD_ACT_SCENARIO_24,
        SAFE_CMD_ACT_SCENARIO_25,
        SAFE_CMD_ACT_SCENARIO_26,
        SAFE_CMD_ACT_SCENARIO_27,
        SAFE_CMD_ACT_SCENARIO_28,
        SAFE_CMD_ACT_SCENARIO_29,
        SAFE_CMD_ACT_SCENARIO_30,
        SAFE_CMD_ACT_SCENARIO_31,
        SAFE_CMD_ACT_SCENARIO_32,
        SAFE_CMD_TOTAL_ACTIVATION,      // Inserimento delle aree assegnate all'utente e controllabili dal dispositivo di controllo
        SAFE_CMD_TOTAL_DEACTIVATION,    // Disinserimento delle aree assegnate all'utente e controllabili dal dispositivo di controllo
        SAFE_CMD_DEVICE_ACTIVATION,     // Inserimento personalizzato dall'utente tramite un dispositivo di controllo
        /// @todo Hub: check if SAFE_CMD_ARM_DISARM_ACTION is different to SAFE_CMD_DEVICE_ACTIVATION
        //SAFE_CMD_ARM_DISARM_ACTION,     // Command supported only Hub control panel
        SAFE_CMD_ACT_MAX                // Do NOT use!!!
    };
    
    ///Enumerato inserimenti specifici su area per ordine di degrado della sicurezza
    enum SafeAreaActivationType{
        SAFE_AREA_ACT_NONE,
        SAFE_AREA_ACT_TOTAL,
        SAFE_AREA_ACT_P12,
        SAFE_AREA_ACT_P1,
        SAFE_AREA_ACT_P2,
        SAFE_AREA_ACT_DEACT,
    };
    enum SafeAreaActivationFlag{
        SAFE_AREA_ACT_F_DELAY_OFF = (1 << (3)),         // activation 0:delayed 1:immediate
        SAFE_AREA_ACT_F_FORCE_L2  = (1 << (4)),     // forced l2 (Not used)
        SAFE_AREA_ACT_F_FORCE_L3  = (1 << (5)),     // forced l3 (Not used)
        SAFE_AREA_ACT_F_FORCE_ALL = (1 << (6)),     // forced all (Not used)
    };
    
    ///Enumerato per la definizione tipo di azioni sulle zone
    enum SafeZoneActionType{
        SAFE_ZONE_ACTION_EXCLUDE,
        SAFE_ZONE_ACTION_INCLUDE,
        SAFE_ZONE_ACTION_ISOLATION,
        SAFE_ZONE_ACTION_ACTIVATION,
        SAFE_ZONE_ACTION_MAX                 //Do NOT use!!!
    };
    
    ///Enumerato per la definizione delle flag opzionali della azione
    enum SafeActionOption{
        SAFE_ACT_OPT_ROBBERY = 1,                       // Richiesto invio allarme rapina (Da non usare)
        SAFE_ACT_OPT_FORCE_LEVEL = (1 << 1),            // Inserimento forzato dipendente dal livello (A normativa)
        SAFE_ACT_OPT_FORCE_ALL  = (1 << 2),             // Inserimento forzato indipendentemente dal livello (Non a normativa: lo si usa per inserimenti remoti)
        SAFE_ACT_OPT_IMMEDIATE_ACTIVATION = (1 << 3),   // Inserimento immediato
        SAFE_ACT_OPT_QUICK_ALARM = (1 << 4),            // Allarme rapido (Da non usare)
        SAFE_ACT_OPT_QUICK_ACTIVATION = (1 << 5),       // Inserimento rapido (Da non usare)
    };
    
    ///Enumerato che definisce il pulsante in home page cfg
    enum SafeBtnHomeType
    {
        SAFE_BTN_HOME_SCEN_1,
        SAFE_BTN_HOME_SCEN_2,
        SAFE_BTN_HOME_SCEN_3,
        SAFE_BTN_HOME_SCEN_4,
        SAFE_BTN_HOME_SCEN_5,
        SAFE_BTN_HOME_SCEN_6,
        SAFE_BTN_HOME_SCEN_7,
        SAFE_BTN_HOME_SCEN_8,
        
        SAFE_BTN_HOME_FUNCT_1,
        SAFE_BTN_HOME_FUNCT_2,
        SAFE_BTN_HOME_FUNCT_3,
        SAFE_BTN_HOME_FUNCT_4,
        SAFE_BTN_HOME_FUNCT_5,
        SAFE_BTN_HOME_FUNCT_6,
        SAFE_BTN_HOME_FUNCT_7,
        SAFE_BTN_HOME_FUNCT_8,
        
        SAFE_BTN_HOME_MAX                           //Do NOT use!!!
    };
    
    ///Enumerato che definisce i parametri di sistema di tipo booleano sui quali è possibile agire
    enum SafeSysParamType
    {
        SAFE_SYS_PARAM_ENABLE_VOICE_MENU,
        SAFE_SYS_PARAM_PERMANENTLY_ENABLE_INSTALLER,
        SAFE_SYS_PARAM_LANGUAGE,
        SAFE_SYS_PARAM_MAX                          //Do NOT use!!!
    };
    
    ///Enumerato che definisce i parametri dei record in rubrica sui quali è possibile agire
    enum SafeContactParamType
    {
        SAFE_CONTACTS_PARAM_NAME,
        SAFE_CONTACTS_PARAM_TEL,
        SAFE_CONTACTS_PARAM_EMAIL_ADDR,
        SAFE_CONTACTS_PARAM_STOP_CALLS,
        SAFE_CONTACTS_PARAM_MAX             //Do NOT use!!!
    };
    
    ///Enumerato che definisce i parametri degli utenti/installatori sui quali è possibile agire
    enum SafeUserParamType
    {
        SAFE_USER_PARAM_NAME,
        SAFE_USER_PARAM_CODE,
        SAFE_USER_PARAM_TYPE,
        SAFE_USER_PARAM_TIMER,
        SAFE_USER_PARAM_MAX                 //Do NOT use!!!
    };
    
    ///Enumerato che definisce i parametri delle chiavette rfid sui quali è possibile agire
    enum SafeRfidKeyParamType
    {
        SAFE_RFID_KEY_PARAMS_ALL,			//Tutti i parametri compresa descrizione, da usare per acquisizione ed eliminazione chiave
        SAFE_RFID_KEY_PARAMS_ACTIVATION,	//Solo parametro attivazione (Es. per disattivare/attivare la chiave senza modificarne i valori)
        SAFE_RFID_KEY_PARAMS_NAME,			//Solo parametro descrizione
        SAFE_RFID_KEY_PARAMS_MAX            //Do NOT use!!!
    };
    
    ///Enumerato che definisce il colore delle chiavette RFID
    enum SafeRfidKeyColourType
    {
        SAFE_RFID_KEY_COLOUR_GREY,
        SAFE_RFID_KEY_COLOUR_BLUE,
        SAFE_RFID_KEY_COLOUR_YELLOW,
        SAFE_RFID_KEY_COLOUR_ORANGE,
        SAFE_RFID_KEY_COLOUR_RED,
        SAFE_RFID_KEY_COLOUR_GREEN,
        SAFE_RFID_KEY_COLOUR_GREY_BLUE,
        SAFE_RFID_KEY_COLOUR_GREY_YELLOW,
        SAFE_RFID_KEY_COLOUR_GREY_ORANGE,
        SAFE_RFID_KEY_COLOUR_GREY_RED,
        SAFE_RFID_KEY_COLOUR_GREY_GREEN,
        SAFE_RFID_KEY_COLOUR_INVALID,
        SAFE_RFID_KEY_COLOUR_MAX    //Do NOT use!!!
    };
    
    ///Enumerato che definisce il livello di autenticazione dell'utente
    enum SafeUserLoginLevel
    {
        SAFE_USER_LOGIN_LEVEL_1,    //livello di autenticazione dispositivo
        SAFE_USER_LOGIN_LEVEL_2,    //livello di autenticazione utente
        SAFE_USER_LOGIN_LEVEL_3,    //livello di autenticazione installatore
        SAFE_USER_LOGIN_LEVEL_4,    //livello di autenticazione produttore
        SAFE_USER_LOGIN_LEVEL_MAX   //Do NOT use!!!
    };
    
    enum SafeMediaListType
    {
        SAFE_MEDIA_LIST_NONE,
        SAFE_MEDIA_LIST_ALARM,
        SAFE_MEDIA_LIST_CONTINUOUS,
        SAFE_MEDIA_LIST_SCHEDULED,
        SAFE_MEDIA_LIST_ARMING,
        SAFE_MEDIA_LIST_MAX //Do NOT use!!!
    };

    ///Define touch parameters we can write
    enum SafeGenTouchParamType
    {
        SAFE_GEN_TOUCH_PARAM_BUZZER_ON,
        SAFE_GEN_TOUCH_PARAM_BUZZER_INGRESSO,
        SAFE_GEN_TOUCH_PARAM_BUZZER_USCITA,
        SAFE_GEN_TOUCH_PARAM_BUZZER_ALLARME,
        SAFE_GEN_TOUCH_PARAM_BUZZER_CAMPANELLO,
        SAFE_GEN_TOUCH_PARAM_BUZZER_PREALLARME,
        SAFE_GEN_TOUCH_PARAM_BUZZER_PREAVVISO,
        SAFE_GEN_TOUCH_PARAM_BUZZER_BEEP_ON,
        SAFE_GEN_TOUCH_PARAM_BUZZER_ARM_DISARM,
        SAFE_GEN_TOUCH_PARAM_BUZZER_FAILED_ARM,
        SAFE_GEN_TOUCH_PARAM_MAX                          //Do NOT use!!!
    };

    ///Define allowed account role associations
    enum SafeAccountAssociationRole
    {
        SAFE_ACCOUNT_ASSOCIATION_ROLE_NONE,
        SAFE_ACCOUNT_ASSOCIATION_ROLE_USER,
        SAFE_ACCOUNT_ASSOCIATION_ROLE_INSTALLER,
        SAFE_ACCOUNT_ASSOCIATION_ROLE_MAX                //Do NOT use!!!
    };

    // ====================================================================================
	// ========================== ENUM CCTV================================================
	/// Family product
	enum TvccFamilyModel
	{
		TVCC_FAMILY_IP100,
		TVCC_FAMILY_IPEASY,
		TVCC_FAMILY_START,
		TVCC_FAMILY_AHD,
		TVCC_FAMILY_NONE,
		TVCC_FAMILY_MAX
	};

	/// Device type
	enum TvccDeviceType
	{
		TVCC_DEVICE_DVR,
		TVCC_DEVICE_NVR,
		TVCC_DEVICE_IPC,
		TVCC_DEVICE_NONE,
		TVCC_DEVICE_MAX
	};

	/// Stream capabilities
	enum TvccSourceType{
		TVCC_SOURCE_VIDEO,
		TVCC_SOURCE_AUDIOVIDEO,
		TVCC_SOURCE_NONE,
		TVCC_SOURCE_MAX
	};

	/// Stream resolution
	enum TvccResolution{
		TVCC_RESOLUTION_CIF,
		TVCC_RESOLUTION_QCIF,
		TVCC_RESOLUTION_HD1,
		TVCC_RESOLUTION_D1,
		TVCC_RESOLUTION_WCIF,
		TVCC_RESOLUTION_960H,
		TVCC_RESOLUTION_Q720P,
		TVCC_RESOLUTION_QVGA,
		TVCC_RESOLUTION_VGA,
		TVCC_RESOLUTION_Q1080P,
		TVCC_RESOLUTION_HD720P,
		TVCC_RESOLUTION_HD1080P,
		TVCC_RESOLUTION_NONE,
		TVCC_RESOLUTION_MAX
	};

	/// BitRate type
	enum TvccBitRateType{
		TVCC_BITRATETYPE_CBR,
		TVCC_BITRATETYPE_VBR,
		TVCC_BITRATETYPE_NONE,
		TVCC_BITRATETYPE_MAX
	};

	/// Stream BitRate
	enum TvccBitRate{
		TVCC_BITRATE_KBPS_16,
		TVCC_BITRATE_KBPS_32,
		TVCC_BITRATE_KBPS_48,
		TVCC_BITRATE_KBPS_64,
		TVCC_BITRATE_KBPS_80,
		TVCC_BITRATE_KBPS_96,
		TVCC_BITRATE_KBPS_128,
		TVCC_BITRATE_KBPS_160,
		TVCC_BITRATE_KBPS_192,
		TVCC_BITRATE_KBPS_224,
		TVCC_BITRATE_KBPS_256,
		TVCC_BITRATE_KBPS_320,
		TVCC_BITRATE_KBPS_384,
		TVCC_BITRATE_KBPS_448,
		TVCC_BITRATE_KBPS_512,
		TVCC_BITRATE_KBPS_640,
		TVCC_BITRATE_KBPS_768,
		TVCC_BITRATE_KBPS_896,
		TVCC_BITRATE_KBPS_1024,
		TVCC_BITRATE_KBPS_1280,
		TVCC_BITRATE_KBPS_1536,
		TVCC_BITRATE_KBPS_1792,
		TVCC_BITRATE_KBPS_2048,
		TVCC_BITRATE_KBPS_SELFD,
		TVCC_BITRATE_KBPS_NONE,
		TVCC_BITRATE_MAX
	};

	/// Stream Quality
	enum TvccQuality{
		TVCC_QUALITY_HIGHEST,
		TVCC_QUALITY_HIGH,
		TVCC_QUALITY_GOOD,
		TVCC_QUALITY_NORMAL,
		TVCC_QUALITY_LOW,
		TVCC_QUALITY_LOWEST,
		TVCC_QUALITY_NONE,
		TVCC_QUALITY_MAX
	};

	/// Stream Codec Type
	enum TvccCodecType{
		TVCC_CODEC_TYPE_PRIVATE_264,
		TVCC_CODEC_TYPE_STANDARD_264,
		TVCC_CODEC_TYPE_STANDARD_MPEG4,
		TVCC_CODEC_TYPE_MJPEG,
		TVCC_CODEC_TYPE_NONE,
		TVCC_CODEC_TYPE_MAX
	};

	/// Codec Profile
	enum TvccProfileStream{
		TVCC_CODEC_PROFILE_HIGH,
		TVCC_CODEC_PROFILE_MAIN,
		TVCC_CODEC_PROFILE_BASELINE,
		TVCC_CODEC_PROFILE_NONE,
		TVCC_CODEC_PROFILE_MAX
	};

	/// Stream Framerate
	enum TvccFrameRate{
		TVCC_FRAMERATE_FULL,
		TVCC_FRAMERATE_25,
		TVCC_FRAMERATE_22,
		TVCC_FRAMERATE_20,
		TVCC_FRAMERATE_18,
		TVCC_FRAMERATE_16,
		TVCC_FRAMERATE_15,
		TVCC_FRAMERATE_12,
		TVCC_FRAMERATE_10,
		TVCC_FRAMERATE_8,
		TVCC_FRAMERATE_6,
		TVCC_FRAMERATE_5,
		TVCC_FRAMERATE_4,
		TVCC_FRAMERATE_3,
		TVCC_FRAMERATE_2,
		TVCC_FRAMERATE_1,
		TVCC_FRAMERATE_NONE,
		TVCC_FRAMERATE_MAX
	};

	/// IFrame interval
	typedef int TvccIFrame;

	/// ptz commands
	enum TvccPtzCommand{
		TVCC_PTZ_ZOOMIN,
		TVCC_PTZ_ZOOMOUT,
		TVCC_PTZ_FOCUSNEAR,
		TVCC_PTZ_FOCUSFAR,
		TVCC_PTZ_IRISOPEN,
		TVCC_PTZ_IRISCLOSE,
		TVCC_PTZ_UP,
		TVCC_PTZ_DOWN,
		TVCC_PTZ_LEFT,
		TVCC_PTZ_RIGHT,
		TVCC_PTZ_UPLEFT,
		TVCC_PTZ_UPRIGHT,
		TVCC_PTZ_DOWNLEFT,
		TVCC_PTZ_DOWNRIGHT,
		TVCC_PTZ_AUTO,
		TVCC_PTZ_STOP,
		TVCC_PTZ_WIPPER_ON,
		TVCC_PTZ_WIPPER_OFF,
		TVCC_PTZ_LIGHT_ON,
		TVCC_PTZ_LIGHT_OFF,
		TVCC_PTZ_POWER_ON,
		TVCC_PTZ_POWER_OFF,
		TVCC_PTZ_GOTO_PRESET,
		TVCC_PTZ_SET_PRESET,
		TVCC_PTZ_CLEAR_PRESET,
		TVCC_PTZ_NONE,
		TVCC_PTZ_MAX
	};

    /// @enum codec_t
    /// contain a set of video codec type supported by libcomelit
    enum codec_t{
        CT_T_MPEG4= 131,    ///< MPEG4 codec
        CT_T_MJPEG= 129,    ///< MJPEG codec
        CT_T_H264 = 130,    ///< H264  codec
        CT_T_NONE = 255     ///< codec not set/found
    };

    /// Embedded player-recorder info avi file
    typedef struct {
        /// @{
        DEPRECATED(int codecVideo);      ///< @deprecated use @see codecType for codec of the recorded video
        codec_t      codecType;            ///< codec of the video inside the avi file @see codec_t
        unsigned int frameWidth;         ///< larghezza frame [pixels]
        unsigned int frameHeight;        ///< altezza frame [pixels]
        int          audioFormat;
        int          fps;                ///< video fps
        bool         hasAudioStream;     ///< true if has audio stream, false otherwise
        /// @}
    } AviInfo;
    // ====================================================================================

    /**
     * Set the value of the seed used by libcomelit to initialize the rand() PRNG by calling srand(seed).
     * This method must be called before using the library to ensure that rand() will be properly initialized.
     *
     * @attention     The seed should be different among different applications
     *                (and also among different instances of the same application) to
     *                avoid duplication in the random numbers generated by rand().
     *                Ideally it should be unique per application instance and time indipendent, so we can
     *                be sure that different applications (and also different instances of the same application)
     *                won't generate the same random sequence, even when they are started at the same time.
     *
     * @param seed    - The seed used by srand(seed) to initialize rand()
     *
     */
    static void initLibrary(uint32_t seed);

    /**
     * Restituisce il puntatore alla singola istanza di questa classe che è implementata
     * utilizzando il pattern Singleton. Se è la prima volta che si invoca questo metodo
     * l'istanza verrà anche creata.
     *
     * @return istanza della classe ComelitEngine
     */
    static ComelitEngine* getInstance();

    /**
     * Distruttore della singola instanza della classe
     */
    static void destroyInstance();

    /**
     * Creazione di un nuovo Sistema Senza Gateway (identificato univocamente nella libreria attraverso un id).
     * Un Sistema non è altro che un contenitore di uno o più mondi facenti parte della famiglia Comelit
     * appartententi ad un dato impianto. (es famiglie: Safe, ViP, ...)
     *
     * @deprecated: use sequence
     *              1) initSystemsConnMode(...)
     *              2) createBaseSystem()
     *
     * @attention La chiamata a questo metodo è subordinata al fatto che tra i setData si è dichiarato
     *            che non si sta usando un Gateway d'appartamento. In caso contrario questo metodo
     *            andrà in errore e restituirà -1
     *
     * @attention Allo stato attuale la libreria ammette l'esistenza di un solo sistema alla volta
     *
     * @return id univoco del sistema creato
     *         -1 nel caso di errore in fase di creazione dovuto al conflitto con ciò che è stato dichiarato
     *            nel setData(..., useViperTunnel)
     */
    DEPRECATED(int createSystem());

    /**
     * Create a new system with Gateway (uniquely identified in the library through an id).
     * A System is simply a container of one or more Comelit worlds belonging to a planr (Safe, ViP, ...)
     *
     * @deprecated: use sequence
     *              1) initSystemsConnMode(...)
     *              2) createBaseSystem()
     *              3) createViperTunnel(...)
     *
     * @attention To use this system mode the library must be initialized with useViperTunnel=true
     *
     * @attention The library supports one Viper System per time
     *
     * @param address       - HostName or IP of the Gateway
     * @param tcp_port      - TCP Port of the Gateway
     * @param udp_port      - UDP Port of the Gateway
     * @param conn_timeout  - Connection timeout (default @see VIPER_DEFAULT_CONN_TIMEOUT)
     *
     * @return   unique ID of the System
     *           -1 if the library is initialized with useViperTunnel=false
     *           -2 gateway not reachable
     *         -100 function not supported (missing VIPER_TUNNEL flag)
     */
    DEPRECATED(int createViperSystem(const char* address, int tcp_port, int udp_port, int conn_timeout = VIPER_DEFAULT_CONN_TIMEOUT));

    /**
     * Create a new system with Gateway (uniquely identified in the library through an id).
     * A System is simply a container of one or more Comelit worlds belonging to a plant (Safe, ViP, ...)
     *
     * @deprecated: use sequence
     *              1) initSystemsConnMode(...)
     *              2) createBaseSystem()
     *              3) createViperTunnel(...)
     *
     * @attention The application must create a run thread to take packet from the socket and inject into libcspstack
     *            via sysViperAddReceivedPacketFromSocket method
     *
     * @attention If comes some error from the TCP Socket, the System must be destroyed by the application
     *
     * @attention To use this mode the library must be initialized with useViperTunnel=true
     *
     * @attention The library supports one Viper System per time
     *
     * @param address       - HostName or IP of the Gateway
     * @param tcp_port      - TCP Port of the Gateway
     * @param udp_port      - UDP Port of the Gateway
     * @param fd            - File Descriptor of the TCP socket otherwise
     * @param conn_timeout  - Connection timeout (default @see VIPER_DEFAULT_CONN_TIMEOUT)
     *
     * @return   unique ID of the System
     *           -1 if the library is initialized with useViperTunnel=false
     *           -2 gateway not reachable
     *         -100 function not supported (missing VIPER_TUNNEL flag)
     */
    DEPRECATED(int createViperSystemWithoutRun(const char* address, int tcp_port, int udp_port, int* fd, int conn_timeout = VIPER_DEFAULT_CONN_TIMEOUT));

    /**
     * Create a new system with P2P connection (uniquely identified in the library with an id).
     * The status of the connection to the system will be notified asynchronously by an event, @see eViperConnection
     *
     * @deprecated: use sequence
     *              1) initSystemsConnMode(...)
     *              2) createBaseSystem()
     *              3) createViperTunnel(...)
     *
     * @param p2pInfo a structure contains all the various parameters for the p2p @see p2pParameters
     *
     * @return  unique id of the created system
     *          -1 in case of error
     *          -100 if the library is not compiled with VIPER_TUNNEL and VIPER_P2P flags
     */
    DEPRECATED(int createViperP2PSystem(p2pParameters p2pInfo));

    /**
     * Create a new system with the base characteristics which will be integrated by called ::openViperTunnel(),
     * ::openViperTunnelWithoutRun() and ::openViperTunnelP2P():
     *
     * @param sysId unique ID of the System
     */
    void createBaseSystem(int* sysId);

    /**
     * Create for a system (created using createBaseSystem(...)) the remote connection to gateway.
     * The connection mode depens on ViperConnMode param, inside ViperParameters structure.
     *
     * @param params    structure to setup connection, depending on ViperConnMode
     * @param retParam  return parameter, used or not depending on ViperConnMode
     *
     * @return    0 Request successful
     *           -1 if the library is initialized with useViperTunnel=false
     *           -2 gateway not reachable
     *         -100 function not supported or params errors
     */
    int createViperTunnel(ViperParameters &params, int* retParam = nullptr);

    /**
     * Open the first vipertunnel available
     * previously added calling createViperTunnel
     * @return   0 Request successful
     *           -1 otherwise
     */
    int startNextViperTunnel(int sys_id);

    /**
     * Create a new system with Gateway (uniquely identified in the library through an id).
     * A System is simply a container of one or more Comelit worlds belonging to a planr (Safe, ViP, ...)
     *
     * @deprecated: use createViperTunnel(...)
     *
     * @attention To use this system mode the library must be initialized with useViperTunnel=true
     *
     * @attention To use this mode must be created a base system through ::createBaseSystem()
     *
     * @attention The library supports one Viper System per time
     *
     * @param sys_id        - Unique ID of the System created with ::createBaseSystem()
     * @param address       - HostName or IP of the Gateway
     * @param tcp_port      - TCP Port of the Gateway
     * @param udp_port      - UDP Port of the Gateway
     * @param conn_timeout  - Connection timeout (default @see VIPER_DEFAULT_CONN_TIMEOUT)
     *
     * @return    0 Request successful
     *           -1 if the library is initialized with useViperTunnel=false
     *           -2 gateway not reachable
     *         -100 function not supported (missing VIPER_TUNNEL flag)
     */
    DEPRECATED(int openViperTunnel(int sys_id, const char* address, int tcp_port, int udp_port, int conn_timeout = VIPER_DEFAULT_CONN_TIMEOUT));

    /**
     * Create a new system with Gateway (uniquely identified in the library through an id).
     * A System is simply a container of one or more Comelit worlds belonging to a plant (Safe, ViP, ...)
     *
     * @deprecated: use createViperTunnel(...)
     *
     * @attention The application must create a run thread to take packet from the socket and inject into libcspstack
     *            via sysViperAddReceivedPacketFromSocket method
     *
     * @attention If comes some error from the TCP Socket, the System must be destroyed by the application
     *
     * @attention To use this mode the library must be initialized with useViperTunnel=true
     *
     * @attention To use this mode must be created a base system through ::createBaseSystem()
     *
     * @attention The library supports one Viper System per time
     *
     * @param sys_id        - Unique ID of the System created with ::createBaseSystem()
     * @param address       - HostName or IP of the Gateway
     * @param tcp_port      - TCP Port of the Gateway
     * @param udp_port      - UDP Port of the Gateway
     * @param fd            - File Descriptor of the TCP socket otherwise
     * @param conn_timeout  - Connection timeout (default @see VIPER_DEFAULT_CONN_TIMEOUT)
     *
     * @return    0 Request successful
     *           -1 if the library is initialized with useViperTunnel=false
     *           -2 gateway not reachable
     *         -100 function not supported (missing VIPER_TUNNEL flag)
     */
    DEPRECATED(int openViperTunnelWithoutRun(int sys_id, const char* address, int tcp_port, int udp_port, int* fd, int conn_timeout = VIPER_DEFAULT_CONN_TIMEOUT));

    /**
     * Create a new system with P2P connection (uniquely identified in the library with an id).
     * The status of the connection to the system will be notified asynchronously by an event, @see eViperConnection
     *
     * @deprecated: use createViperTunnel(...)
     *
     * @attention To use this mode must be created a base system through ::createBaseSystem()
     *
     * @param sys_id        - Unique ID of the System created with ::createBaseSystem()
     * @param p2pInfo       - A structure contains all the various parameters for the p2p @see p2pParameters
     *
     * @return  The File Descriptor of the P2P socket
     *           -1 if the library is initialized with useViperTunnel=false
     *           -2 gateway not reachable
     *          -100 if the library is not compiled with VIPER_TUNNEL and VIPER_P2P flags
     */
    DEPRECATED(int openViperTunnelP2P(int sys_id, p2pParameters p2pInfo));

    /**
     * Rimuove un dato Sistema
     *
     * @param sys_id - identificativo del Sistema da rimuovere
     *
     * @return -4 se il sitema non esiste
     *          0 altrimenti
     */
    int removeSystem(unsigned int sys_id);

    /**
     * Inserimento di un pacchetto ricevuto sul socket TCP di connessione col Gateway gestito dall'applicativo
     * nel caso si sia creato il sistema senza attivare la run di libreria, ovvero si è creato il sistema
     * tramite il metodo createViperSystemWithoutRun
     *
     * @param sys_id - identificativo del Sistema a cui è associato il socket
     * @param rxbuf  - buffer ricevuto sul socket
     * @param rxlen  - lunghezza del buffer ricevuto
     *
     * @return     0 se l'inserimento del pacchetto è andato a buon fine
     *            -1 in caso non esista il socket verso il dispositivo Vip Extender
     *            -4 se il Sistema in cui si vuole inserire il pacchetto non esiste
     *          -100 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIPER_TUNNEL)
     */
    int sysViperAddReceivedPacketFromSocket(int sys_id, char* rxbuf, int rxlen);

    /**
     * Invio di dati su un particolare canale viper di un dato sistema
     *
     * @param sys_id        - identificativo del Sistema
     * @param channel       - Canale su cui inviare i dati
     * @param data          - dati da inviare
     *
     * @return     0 se l'inserimento del pacchetto è andato a buon fine, -1 altrimenti
     *            -4 se il Sistema in cui si vuole inserire il pacchetto non esiste
     *          -100 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIPER_TUNNEL)
     */
    int sysViperSendOnChannel(int sys_id, ViperChannelType channel, const char* data);

    /**
     * Chiusura di un particolare canale viper di un dato sistema
     *
     * @param sys_id    - identificativo del Sistema
     * @param channel   - Canale da Chiudere
     *
     * @return    0 se la chiusura del canale è andato a buon fine, -1 altrimenti
     *           -4 se il Sistema in cui si vuole inserire il pacchetto non esiste
     *         -100 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIPER_TUNNEL)
     */
    int sysViperCloseChannel(int sys_id, ViperChannelType channel);

    /**
     * Apertura di un particolare canale viper di un dato sistema
     *
     * @param sys_id    - identificativo del Sistema
     * @param channel   - Canale da Aprire
     *
     * @return     0 se l'apertura del canale è andato a buon fine, -1 altrimenti
     *            -4 se il Sistema in cui si vuole inserire il pacchetto non esiste
     *          -100 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIPER_TUNNEL)
     */
    int sysViperOpenChannel(int sys_id, ViperChannelType channel);

    /**
     * Creazione di un elemento del mondo Vip "VipUnit" all'interno di un dato Sistema
     * L'elemento ViP che è possibile creare con questo metodo è di tipo "Posto Interno" nelle
     * due modalità: Master e Slave
     *
     * @attention Un Sistema (inteso a livello di libreria) può contenere un solo elemento Vip
     *             nel caso di utilizzo ViperTunnel
     *
     * @param sys_id  - identificativo del Sistema a cui associare l'elemento Vip
     * @param vipaddr - indirizzo Vip dell'elemento ViP da creare
     * @param subaddr - sottoindirizzo Vip dell'elemtno da creare. Può assumere i seguenti valori
     *                  0 - se da creare in modalità Master
     *                  [1 - 15] - se da creare in modalità Slave
     * @param role    - identifica il ruolo dell'unità Vip (internal unit, porter)
     * @param capab   - capability della unità vip (Video TX/RX, Audio TX/RX, Opendoor)
     *                  NB: da utilizzare una o più flag di VipUnitCapability in OR)
     * @param flags   - @see UnitFlags
     * @attention La coppia [indirizzo-sottoindirizzo] deve essere univoca all'interno di un Sistema
     *
     * @return  id univoco della VipUnit create
     *          -1 non è stato possibile creare una nuova ViPUnit (possibile causa è nel caso di
     *             ViperTunnel in cui ve ne può essere una sola)
     *          -4 se il Sistema a cui si vuole associare l'elemento non esiste
     *        -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
     */
    int sysCreateVipUnit(int sys_id, const char* vipaddr, const char* subaddr, VipUnitRole role, uint32_t capab, unsigned int flags);

    /**
     * Rimozione di un elemento del mondo Vip "VipUnit" all'interno di un dato Sistema.
     *
     * @attention Un Sistema (inteso a livello di libreria) può contenere un solo elemento Vip
     *             nel caso di utilizzo ViperTunnel
     *
     * @param sys_id     - identificativo del Sistema a cui è associata la VipUnit da Rimuovere
     * @param vipunit_id - identificativo della VipUnit da rimuovere
     *
     * @return   0 se la rimozione è andata a buon fine
     *          -4 se il Sistema indicato non esiste
     *          -6 se la VipUnit che si vuole eliminare non esiste nel sistema indicato
     *        -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
     */
    int sysRemoveVipUnit(int sys_id, int vipunit_id);

    /**
     * Metodo per richiedere il rinnovo della registrazione al dispositivo Master
     *
     * @attention Se il sistema è stato creato con createViperSystemWithoutRun questo metodo
     *            serve all'applicativo per gestire il timer di rinnovo della registrazione al master
     *
     * @attention Il Time To Live della registrazione al master è 1h, si consiglia quindi di chiamare
     *            questo metodo entro 50 minuti dopo la reisgrazione
     *
     * @attention E' importante assicurarsi di essere registrati al maste altrimenti non verranno più
     *            ricevute le chiamate in ingresso
     *
     * @attention Se il sistema è stato creato con createViperSystemWithoutRun e il sistema Host non
     *            permette di utilizzare il medesimo timer di registrazione anche in background si
     *            consiglia di chiamare questo metodo prima della chiamata ai metodi di start & stop e
     *            di chiamare tali metodi solo all'avvenuta registrazione al master (le notifiche di
     *            stato di registrazione le si leggono dal socket locale creato per i messaggi json).
     *            Questa procedura permette di creare un timer per il background con tempo fisso, ad
     *            esempio 50min e di non dover calcolare ogni volta il TTL rimanente. Un sistema host
     *            che richiede l'utilizzo di questa procedura è iOS.
     *
     * @param sys_id     - identificativo del Sistema di appartenenza
     * @param vipunit_id - identificativo della VipUnit appartenente al sistema
     *
     * @return  -1 se la SubUnit non esiste
     *          -4 se il Sistema non esiste
     *          -6 se la ViPUnit non esiste
     *           0 altrimenti
     *        -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
     */
    int sysVipRenewRegistrationToMaster(int sys_id, int vipunit_id);

    /**
     * Impostazione dei parametri per la deviazione di chiamata che possono essere associati ad una VipUnit
     * relativa ad un dato sistema
     *
     * @param sys_id     - identificativo del Sistema di appartenenza
     * @param vipunit_id - identificativo della VipUnit appartenente al sistema
     * @param addr       - indirizzo dell'elemento ViP a cui deviare la chiamata in ingresso
     * @param activated  - flag di abilitazione della deviazione di chiamata lato utente
     * @param ontime     - flag di identificazione della possibilità di abilitare la deviazione a tempo
     * @param time       - tempo dopo il quale verrà deviata la chiamata
     * @param onbusy     - flag di identificazione della possibilità di abilitare la deviazione su occupato
     *
     * @attention La deviazione su occupato può essere impostata solo se l'elemento ViP è stato creato in
     *            modalità Master
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la ViPUnit non esiste
     *           0 altrimenti
     *        -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
     */
    int sysVipSetDivertCall(int sys_id, int vipunit_id, const char* addr, bool activated, bool ontime, int time, bool onbusy);

    /**
     * @brief sysVipSetCtplocalhost
     * Force the usage of locahost interface for sending CSP messages
     * Can be used only when the CSPProxy service (provided by extender) is running on the device
     * Is the caller responsability to check whether the CSPProxy service is running ort not
     *
     * @param sys_id             - System Identifier
     * @param vipunit_id         - VipUnit Identifier
     * @param forceLocalHost     - true to force usage of localhost interface
     *
     * @return  -4 if the System does not exists
     *          -6 if the ViPUnit does not exists
     *           0 if succes
     *        -101 if the feature is not enable by the library configuration (missing VIP_SYSTEM flag)
     */
    int sysVipSetCtplocalhost(int sys_id, int vipunit_id, bool forceLocalHost);

    /**
     * Impostazione dei tempi di chiamata
     *
     * @param sys_id             - identificativo del Sistema di appartenenza
     * @param vipunit_id         - identificativo della VipUnit appartenente al sistema
     * @param param              - parametro da configurare
     * @param value              - valore di durata in secondi
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la ViPUnit non esiste
     *          -1 se si sta tentando di modificare un parametro non utilizzato dall'attuale tipologia di VipUnit
     *           0 altrimenti
     *        -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
     */
    int sysVipSetCallTimeParam(int sys_id, int vipunit_id, CallTimeParam param, int value);

    /**
     * Impostazione proprietà apriporta.
     *
     * @attention Metodo utile alle interna unit che hanno specificato la capability VU_CAPAB_OPENDOOR
     *
     * @param sys_id                 - identificativo del Sistema di appartenenza
     * @param vipunit_id             - identificativo della VipUnit appartenente al sistema
     * @param accept_in_idle         - true se si acceta il comando apriporta in idle
     * @param od_relays_array        - array degli id dei relay a cui è associata la funzione apriporta (verrà eseguita memcpy)
     * @param od_relays_array_size   - dimensione dell'array dei relay opendoor
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la ViPUnit non esiste
     *          -1 in caso di errore (dimensione array <= 0, oppure mancanza di CAPAB_OPENDOOR)
     *           0 altrimenti
     *        -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
     */
    int sysVipSetOpenDoorCapabProperities(int sys_id, int vipunit_id, bool accept_in_idle, int* od_relays_array, int od_relays_array_size);

    /**
     * Impostazione proprietà chiamate con autoaccensione (self ignition).
     *
     * @param sys_id                 - identificativo del Sistema di appartenenza
     * @param vipunit_id             - identificativo della VipUnit appartenente al sistema
     * @param enabled                - abilita/disabilita chiamate con autoaccensione
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la ViPUnit non esiste
     *          -1 in caso di errore
     *           0 altrimenti
     *        -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
     */
    int sysVipSetAcceptSelfIgnitionCalls(int sys_id, int vipunit_id, bool enabled);

    /**
     * Impostazione del bitrate che verrà o meno richiesto per il flusso video in ingresso. E' possibile decidere
     * di impostare un dato valore da chiedere od utilizzare un valore di default.
     *
     * @param sys_id     - identificativo del Sistema di appartenenza
     * @param vipunit_id - identificativo della VipUnit appartenente al sistema
     * @param enabled    - true se si desidera richiedere uno specifico bitrate, false se si vuole usare il default
     * @param value      - valore di bitrate da richiedere in kbps
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la ViPUnit non esiste
     *           0 altrimenti
     *        -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
     */
    int sysVipSetBitrate(int sys_id, int vipunit_id, bool enabled, int value);

    /**
     * Impostazione della dimensione massima dei pacchetti RTP che verrà usato per il flusso video in uscita.
     *
     * @param sys_id     - identificativo del Sistema di appartenenza
     * @param vipunit_id - identificativo della VipUnit appartenente al sistema
     * @param fsm_id     - identificativo della FSM di chiamata per cui impostare la massima dimensione dei pacchetti RTP
     * @param value      - dimensione massima payload RTP
     *
     * @return  -1 se la FSM di chiamata non esiste
     *          -4 se il Sistema non esiste
     *          -6 se la ViPUnit non esiste
     *           0 altrimenti
     *        -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
     */
    int sysVipSetMaxRtpPayloadSize(int sys_id, int vipunit_id, int fsm_id, int value);

    /**
     * Impostazione della dimensione massima dei pacchetti RTP che verrà usato per la unit.
     *
     * @param sys_id     - identificativo del Sistema di appartenenza
     * @param vipunit_id - identificativo della VipUnit appartenente al sistema,
     *                     per cui impostare la massima dimensione dei pacchetti RTP
     * @param value      - dimensione massima payload RTP
     *
     * @return  -1 se la FSM di chiamata non esiste
     *          -4 se il Sistema non esiste
     *          -6 se la ViPUnit non esiste
     *           0 altrimenti
     *        -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
     */
    int sysVipSetUnitMaxRtpPayloadSize(int sys_id, int vipunit_id, int value);

    /**
     * Set the Maximum Video resolution in pixels and Frames per Second
     *
     * @param sys_id     - System ID
     * @param vipunit_id - ViP unit
     * @param xres       - Video stream width in pixels
     * @param yres       - Video stream height in pixels
     * @param fps        - Video stream speed (in frames per second)
     *
     * @return  -1 If the parameters are invalid
     *          -4 If the system does not exist
     *          -6 If the ViPUnit does not exist
     *           0 if successful
     *        -101 if this functionality is not supported (missing VIP_SYSTEM compilation flag)
     */
    int sysVipSetRtpMaxVideoResolution(int sys_id, int vipunit_id, int xres, int yres, int fps);

    /**
     * Set the Preferred Video resolution in pixels and Frames per Second
     *
     * @param sys_id     - System ID
     * @param vipunit_id - ViP unit
     * @param xres       - Video stream width in pixels
     * @param yres       - Video stream height in pixels
     *
     * @return  -1 If the parameters are invalid
     *          -4 If the system does not exist
     *          -6 If the ViPUnit does not exist
     *           0 if successful
     *        -101 if this functionality is not supported (missing VIP_SYSTEM compilation flag)
     */
    int sysVipSetRtpPrefVideoResolution(int sys_id, int vipunit_id, int xres, int yres);

    /**
     * Retrieve the configured bitrate.
     *
     * @param sys_id     - System ID
     * @param vipunit_id - ViP unit
     *
     * @return  -4 If the system does not exist
     *          -6 If the ViPUnit does not exist
     *        -101 if this functionality is not supported (missing VIP_SYSTEM compilation flag)
     *             A positive value representing the bitrate in kbps
     */
    int sysVipGetBitrate(int sys_id, int vipunit_id);

    /**
     * Inizializza i dati necessari e prepara la engine per essere avviata
     *
     * @param ipaddr           - indirizzo IP attualmente utilizzato nel dispositivo
     * @param netmask          - netmask attualmente utilizzata nel dispositivo
     * @param evtSocketPort    - indicazione della porta su cui si desidera ricevere i file Json di notifica
     * @param useViperTunnel   - true se la connessione deve essere effettuata (per tutti i sistemi) attraverso
     *                           un Gateway d'appartemanto (art. della famiglia 1456)
     *
     * @attention E' strettamente necessario creare il ServerSocket in ascolto su evtSocketPort prima
     *            di effettuare la init()
     *
     * @deprecated: use sequence
     *              1) setupEvtSocket(...)
     *              2) setupParameter(s)(...)
     *              3) initEngineMode(...)
     *
     * @attention Da chiamare prima di invocare il metodo start()
     *
     * @return   -1 se la libreria non riesce a connettersi al socket che dovrebbe essere aperto in localhost
     *              sulla porta dichiarata nel metodo setData(...,evtSocketPort,...) per l'invio dei file JSON
     *              necessari per ricevere gli eventi di ritorno dal socket
     *            0 se l'inizializzazione è andata a buon fine
     *         -100 se si cerca di inizializzare con useViperTunnel a true ma la funzionalità non è supportata
     *              dall'attuale compilazione della libreria (manca flag VIPER_TUNNEL)
     */
    DEPRECATED(int init(const char* ipaddr, const char* netmask, int evtSocketPort, bool useViperTunnel));

    /**
     * Init version for callback mode
     * @param ipaddr         - indirizzo IP attualmente utilizzato nel dispositivo
     * @param netmask        - netmask attualmente utilizzata nel dispositivo
     * @param cbk            - callback da chiamare per notifiche su eventi
     * @param cbkArg         - Callback argument passato dalla libreria quando la callback viene chiamata
     * @param useViperTunnel - true se la connessione deve essere effettuata (per tutti i sistemi) attraverso
     *                         un Gateway d'appartemanto (art. della famiglia 1456)
     *
     * @attention E' strettamente necessario creare il ServerSocket in ascolto su evtSocketPort prima
     *            di effettuare la init()
     *
     * @deprecated: use sequence
     *              1) setupEvtCallback(...)
     *              2) setupParameter(s)(...)
     *              3) initEngineMode(...)
     *
     * @attention Da chiamare prima di invocare il metodo start()
     *
     * @return   -1 errore di setup parametri
     *            0 se l'inizializzazione è andata a buon fine
     *         -100 se si cerca di inizializzare con useViperTunnel a true ma la funzionalità non è supportata
     *              dall'attuale compilazione della libreria (manca flag VIPER_TUNNEL)
     * @param useViperTunnel
     * @return
     */
    DEPRECATED (int init(const char* ipaddr, const char* netmask, CallbackOnEvt cbk, void* cbkArg, bool useViperTunnel));

    /**
     * Inizializza il socket per ricezione eventi
     *
     * @param evtSocketPort  - indicazione della porta su cui si desidera ricevere i file Json di notifica
     *
     * @attention E' strettamente necessario creare il ServerSocket in ascolto su evtSocketPort prima
     *            di effettuare la setupEvtSocket()
     *
     * @attention Da chiamare prima di invocare il metodo initEngineMode()
     *
     * @return   -1 se la libreria non riesce a connettersi al socket che dovrebbe essere aperto in localhost
     *              sulla porta dichiarata nel metodo setData(...,evtSocketPort,...) per l'invio dei file JSON
     *              necessari per ricevere gli eventi di ritorno dal socket
     *            0 se l'inizializzazione è andata a buon fine
     */
    int setupEvtSocket(int evtSocketPort);

    /**
     * Setup event dispatcher callback
     * @param cbk     - callback used to notify whenever an event occours
     * @param cbkArg  - argument passed back to cbk
     */
    void setupEvtCallback(CallbackOnEvt cbk, void* cbkArg);

    /**
     * Inizializza parametri di rete e random libreria
     *
     * @param ipaddr         - indirizzo IP attualmente utilizzato nel dispositivo
     * @param netmask        - netmask attualmente utilizzata nel dispositivo
     * @param rand           - random number for rand library initialization
     *
     * @attention Da chiamare prima di invocare il metodo initEngineMode()
     *
     * @return    0 se l'inizializzazione è andata a buon fine
     */
    int setupParameters(const char* ipaddr, const char* netmask, int rand);

    /**
     * Function to init library parameters
     *
     * @param paramId     - engine parameter to set @see EngineParams
     * @param value       - a value to assign to paramId
     * @param sizeOfValue - size of the value to assign to paramId
     *
     * @return 0 if correct intialization, < 0 on errors
     */
    int setupParameter(EngineParams paramId, void* value, size_t sizeOfValue);

    /**
     * Inizializza modalità crp, se per viper o no
     *
     * @param useViperTunnel - true se la connessione deve essere effettuata (per tutti i sistemi) attraverso
     *                         un Gateway d'appartemanto
     *
     * @deprecated: use instead initSystemsConnMode(...)
     *
     * @attention Da chiamare dopo setupEvtSocket e setupParameters
     *
     * @attention Da chiamare prima di invocare il metodo start()
     *
     * @return    0 se l'inizializzazione è andata a buon fine
     *         -100 se si cerca di inizializzare con useViperTunnel a true ma la funzionalità non è supportata
     *              dall'attuale compilazione della libreria (manca flag VIPER_TUNNEL)
     *         -108 se inizializzazione errata
     */
    DEPRECATED(int initEngineMode(bool useViperTunnel));

    /**
     * Init systems connection mode, if crp mode or viper or both.
     *
     * @param connectionFlags - bitmask to setup the system connection @see SysConnectionMode
     *
     * @return   0 if correct initialization
     *        -100 if try using viper without compilation flag active
     *        -108 if wrong connection flag parameter
     */
    int initSystemsConnMode(int connectionFlags);

    /**
     * Avvia la engine e tutti i suoi thread.
     *
     * @attention Se si è creato il sistema con createViperSystemWithoutRun e si è legato il risveglio di
     *            ComelitEngine e quindi la chiamata di questo metodo alla ricezione di messaggi sul socket
     *            verificare che in un thread parallelo non sia stata invocata la destroyInstance
     *
     */
    void start();

    /**
     * Avvia la engine e tutti i suoi thread cambiando la porta di connessione del socket locale
     *
     * @attention Se si è creato il sistema con createViperSystemWithoutRun e si è legato il risveglio di
     *            ComelitEngine e quindi la chiamata di questo metodo alla ricezione di messaggi sul socket
     *            verificare che in un thread parallelo non sia stata invocata la destroyInstance
     *
     * @return   -1 se la libreria non riesce a connettersi al socket che dovrebbe essere aperto in localhost
     *              sulla porta dichiarata nel metodo setData(...,evtSocketPort,...) per l'invio dei file JSON
     *              necessari per ricevere gli eventi di ritorno dal socket
     *            0 altrimenti
     *
     */
    int start(int evtSocketPort);

    /**
     * Ferma la engine e quindi tutti i suoi thread
     *
     * @attention Porta a risultati solo nel caso non vi siano attivi Subproxy
     *
     * @return -1 se la engine è già ferma
     *         -2 se esiste una VipUnit con funzionalità di Subproxy che non può essere messa in pausa
     */
    int stop();

    /**
     * Metodo per verificare se la libreria è viva e quindi l'ultima azione era una start oppure spenta
     * come conseguenza di un'azione di stop
     *
     * @return Se la libreria è attiva o spenta
     */
    bool isEngineAlive();
	
    /**
     * Initialize call log parameters
     *
     * @param sys_id        - identifier of the system to which belongs
     * @param vipunit_id    - identifier of the VipUnit that belong to system
     * @param fsm_id        - call's id of the requested frame
     * @param relayId       - relay identifier
     *
     * @return 0 if correct intialization, < 0 on errors
     */
    int sysSendCallLogOpenDoor(int sys_id, int vipunit_id, int fsm_id, int relayId);

    /**
     * Send Call Log Audio packet
     *
     * @param sys_id    - system id where the requested call besides
     * @param unit_type - id of the comelit family @see gUnitType
     * @param unit_id   - id of the unit where the requested call besides
     * @param fsm_id    - call's id of the added frame
     * @param pck       - pointer to the audio packet to add
     * @param len       - pointer where is stored the length of the audio packet to be sent (bytes)
     * 
     * @return     0 success
     *            -1 if audio flow is off
     *            -3 if call fsm doesn't exists (fsm_id error)
     *            -4 if system doesn't exists (sys_id error)
     *            -6 if unit doesn't exists (unit_id error)
     *            -101 functionality not supported by the unit_type passed
     *
     */
    int sysSendCallLogAudioPck(int sys_id, gUnitType unit_type, int unit_id, int fsm_id, short* pck, int len);

    /**
     * Metodo per impostare il numero massimo di chiamate supportare per una data VipUnit
     *
     * @attention Non è possibile settare il numero massimo di chiamate a livello di engine ma il fatto
     *            di limitare tali chiamate è completamente demandato al livello applicativo
     *
     * @attention I valori massimi concessi sono: max_in_conn: 255 max_out_conn: 255 max_concurrent_conn: 255
     *
     * @attention Il default è: max_in_conn: 1 max_out_conn: 255 max_concurrent_conn: 255
     *
     * @param sys_id                - identificativo del Sistema di appartenenza
     * @param vipunit_id            - identificativo della VipUnit appartenente al sistema
     * @param max_in_conn           - numero massimo di chiamate in ingresso supportate
     * @param max_out_conn          - numero massimo di chiamate in uscita supportate
     * @param max_concurrent_conn   - numero massimo di chiamate totali
     *
     * @return   0 se è stato possibile impostare i parametri
     *          -1 se non è stato possibile impostare i parametri perché i vaolri sono troppo elevati
     *          -4 se il Sistema non esiste
     *          -6 se la ViPUnit non esiste
     *        -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
     */
    int sysVipSetCallNumberRestriction(int sys_id, int vipunit_id, int max_in_conn, int max_out_conn, int max_concurrent_conn);

    /**
     * Metodo per richiedere di avviare una chiamata ViP verso una data controparte ViP.
     *
     * @attention La chiamata verrà inizializzata dalla ViPUnit facente parte del sistema passato come
     *            parametro
     *
     * @param sys_id     - identificativo del Sistema di appartenenza
     * @param vipunit_id - identificativo della VipUnit appartenente al sistema
     * @param vipaddr    - indirizzo Vip della controparte da contattare
     * @param subaddr    - sottoindirizzo Vip della controparte da contattare.
     *                     Può assumere i seguenti valori:
     *                           0           - dispositivo master
     *                           [1 - 15]    - dispoditivi slave
     *                           null        - intero appartamnento (quindi tutti i componenti con il dato vipaddr),
     *                                         da notare che bisogna usare questo valore anche quando si vuole contattare
     *                                         un Posto Esterno
     * @param type       - tipo di chiamata che si vuole effettuare
     * @param flags      - flag che va a definire la priorità da dare alla chiamata
     * @param max_out_call_override          - replace max_out_call only for this call
     * @param max_concurrent_conn_override   - replace max_concurrent_conn only for this call
     * @param capab_override                 - replace capab only for this call
     * @param flag2                          - flag specific for paging call
     *
     * @return  ID della Macchina a Stati che andrà a gestire la chiamata (questo ID dovrà essere utilizzato ogni
     *             qual volta si vuole fare richieste relative a questa chiamata) o ID della Macchina a stati che
     *             sta gestendo la chimata con lo stesso vipaddr e subaddr
     *          -4 se il Sistema non esiste
     *          -6 se la ViPUnit non esiste
     *          -2 se è impossibile stabilire la connessione con la controparte
     *         -10 non è stato possibile gestire la richiesta perché sono già presenti un numero di chiamate uguale
     *             a quello massimo dichiarato col metodo sysVipSetCallNumberRestriction
     *          -1 se l'inizializzazione della chiamata non è andata a buon fine
     *        -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
     *          -5 se l'indirizzo VIP da chiamare sia fuori dal range
     */
    int sysVipStartOutgoingCall(int sys_id,
                                int vipunit_id,
                                const char* vipaddr,
                                const char* subaddr,
                                CallType type,
                                CallFlag flags,
                                uint32_t *max_out_call_override = nullptr,
                                uint32_t *max_concurrent_conn_override = nullptr,
                                uint32_t *capab_override = nullptr,
                                CallFlag2 flag2 = F_UNASSIGNED
                               );
    /**
     * Metodo per richiedere di avviare una chiamata ViP verso una data controparte ViP.
     *
     * @attention La chiamata verrà inizializzata dalla ViPUnit facente parte del sistema passato come
     *            parametro
     *
     * @param sys_id     - identificativo del Sistema di appartenenza
     * @param vipunit_id - identificativo della VipUnit appartenente al sistema
     * @param vipaddr    - indirizzo di destinazione
     *
     * @return   0 con esito positivo
     *          -4 se il Sistema non esiste
     *          -6 se la ViPUnit non esiste
     *        -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
     */
    int sysVipGenerateCfp(int sys_id, int vipunit_id, const char* vipaddr = nullptr);


    /**
     * Method to send a vip message
     *
     * @param sys_id        - identifier of the system to which belongs
     * @param vipunit_id    - identifier of the VipUnit that belong to system
     * @param message       - message
     *
     * @return  -4 system does not exists
     *          -6 vipunit does not exists
     *           0 otherwise
     *        -101 if funcionality is not yet supported on the current compilation of the library (flag VIP_SYSTEM missing)
     */
    int sysVipGenerateTechMessage(int sys_id, int vipunit_id, const std::string &message);


    /**
     * Method to request elevator through gateway in VIP system
     *
     * @param sys_id        - identifier of the system to which belongs
     * @param vipunit_id    - identifier of the VipUnit that belong to system
     * @param main_addr     - recipient VipAddress
     * @param src_addr      - elevator source VipAddress
     * @param dst_addr      - elevator destination VipAddress
     * @param dir           - direction of landing call
     * @param land          - elevator function type
     *
     * @return  -4 system does not exists
     *          -6 vipunit does not exists
     *           0 otherwise
     *        -101 if funcionality is not yet supported on the current compilation of the library (flag VIP_SYSTEM missing)
     */
    int sysVipSendElevatorCmd(int sys_id, int vipunit_id, const char *main_addr, const char *src_addr, const char *dst_addr, int dir, int land);

    /**
     * Metodo per richiedere di azionare uno o più relay relativi ad una data controparte facente parte dell'impianto ViP
     * fuori dalla gestione di una chiamata. Tale controparte avrà quindi con un proprio indirizzo ViP
     *
     * @attention L'azione verrà eseguita dalla ViPUnit facente parte del sistema passato come parametro
     *
     * @param sys_id     - identificativo del Sistema di appartenenza
     * @param vipunit_id - identificativo della VipUnit appartenente al sistema
     * @param vipaddr    - indirizzo ViP della controparte su cui si trova il realay da azionare
     * @param relay      - puntatore all'array con gli identificativi dei relay da azionare
     * @param num_relay  - numero di relay contenuti nell'array (sizeof array)
     *
     * @return  -1 se la connessione verso la controparte non è andata a buon fine
     *          -4 se il Sistema non esiste
     *          -6 se la ViPUnit non esiste
     *           0 se l'azionamento è andata a buon fine
     *        -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
     */
    int sysVipSendOpenDoorCmd(int sys_id, int vipunit_id, const char* vipaddr, int* relay, int num_relay);

    /**
     * Metodo per richiedere di azionare uno o più relay relativi alla controparte facente parte dell'impianto ViP
     * che stiamo contattando con una chiamata di tipo ViP in corso.
     * Tale controparte verrà quindi identificata dall'ID della Macchina a Stati che sta gestendo la chiamata.
     *
     * @attention L'azione verrà eseguita dalla ViPUnit facente parte del sistema passato come parametro ed
     *            impegnata nella chiamata fsm_id
     *
     * @param sys_id     - identificativo del Sistema di appartenenza
     * @param vipunit_id - identificativo della VipUnit appartenente al sistema
     * @param fsm_id     - identificativo della Macchina a Stati che sta gestendo la chiamata
     * @param relay      - puntatore all'array con gli identificativi dei relay da azionare
     * @param num_relay  - numero di relay contenuti nell'array (sizeof array)
     *
     * @return  -3 se la Macchina a stati non esiste
     *          -4 se il Sistema non esiste
     *          -6 se la ViPUnit non esiste
     *           0 se l'azionamento è andata a buon fine
     *        -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
     */
    int sysVipSendOpenDoorCmdOnCall(int sys_id, int vipunit_id, int fsm_id, int* relay, int num_relay);

    /**
     * Metodo per azionare un relé a bordo di un dato Attuatore ViP o di una scheda di espansione collegata a tale
     * Attuatore ViP
     *
     * @param sys_id     - identificativo del Sistema di appartenenza
     * @param vipunit_id - identificativo della VipUnit appartenente al sistema
     * @param vipaddr    - indirizzo vip dell'attuatore
     * @param module     - numero di modulo / esansione
     *                       255        - se l'azione dev'essere eseguita su un relay a bordo dell'attuatore
     *                       [1 - 10]   - se l'azione dev'essere eseguita su una delle 10 possibilie espansioni
     *                                    che possono essere connesse
     * @param relay      - numero identificativo del relay da azionare
     *                       [1 - 2]    - nel caso si voglia azionare uno dei realy a bordo dell'attuatore
     *                       [1 - 10]   - nel caso si voglia azionare uno dei relay a bordo di un'espansione
     *
     * @return  -1 se la connessione verso l'attuatore non è andata a buon fine
     *          -4 se il Sistema non esiste
     *          -6 se la ViPUnit non esiste
     *           0 se l'azionamento è andata a buon fine
     *        -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
     */
    int sysVipSendActuatorCmd(int sys_id, int vipunit_id, const char *vipaddr, int module, int relay);

    /**
     * Metodo per azionare un relé a bordo di un dato Attuatore ViP o di una scheda di espansione collegata a tale
     * Attuatore ViP
     *
     * @param sys_id     - identificativo del Sistema di appartenenza
     * @param vipunit_id - identificativo della VipUnit appartenente al sistema
     * @param vipaddr    - indirizzo vip dell'attuatore
     * @param module     - numero di modulo / esansione
     *                       255        - se l'azione dev'essere eseguita su un relay a bordo dell'attuatore
     *                       [1 - 10]   - se l'azione dev'essere eseguita su una delle 10 possibilie espansioni
     *                                    che possono essere connesse
     * @param relay      - numero identificativo del relay da azionare
     *                       [1 - 2]    - nel caso si voglia azionare uno dei realy a bordo dell'attuatore
     *                       [1 - 10]   - nel caso si voglia azionare uno dei relay a bordo di un'espansione
     * @param action     - azione da eseguire sull'attuatore
     *
     * @return  -1 se la connessione verso l'attuatore non è andata a buon fine
     *          -4 se il Sistema non esiste
     *          -6 se la ViPUnit non esiste
     *           0 se l'azionamento è andata a buon fine
     *        -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
     */
    int sysVipSendActuatorCmd(int sys_id, int vipunit_id, const char *vipaddr, int module, int relay, ActuatorAction action);

    /**
     * Method to notify the status of the door
     *
     * @param sys_id        - identifier of the system to which belongs
     * @param vipunit_id    - identifier of the VipUnit that belong to system
     * @param vipaddr       - recipient VipAddress
     * @param status        - status of the door
     *
     * @return  -4 system does not exists
     *          -6 vipunit does not exists
     *           0 otherwise
     *        -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
     */
    int sysVipSendDoorStatus(int sys_id, int vipunit_id, const char *vipaddr, DoorStatus status);

    /**
     * Metodo per richiedere di rubare il flusso Video in Ingresso durante una chiamata in attesa di risposta.
     *
     * @attention Il flusso video viene inviato automaticamente dal dispositivo Master verso i primi 3 slaves.
     *            Utilizzando questo metodo si sottrae il video ad uno di tali slaves.
     *
     * @param sys_id     - identificativo del Sistema di appartenenza
     * @param vipunit_id - identificativo della VipUnit appartenente al sistema
     * @param fsm_id     - identificativo della Macchina a Stati che sta gestendo la chiamata
     *
     * @return  -3 se la Macchina a stati non esiste
     *          -4 se il Sistema non esiste
     *          -6 se la ViPUnit non esiste
     *           0 se l'azionamento è andata a buon fine
     *        -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
     */
    int sysVipRequestVideo(int sys_id, int vipunit_id, int fsm_id);

    /**
     * Metodo per comunicare ai vari dispositivi di uno stesso appartamento e di conseguenza al Master l'intenzione
     * di rispondere alla chiamata. Solitamente legato lla pressione del tasto "prendi fonica".
     *
     * @attention Dopo la chiamata a questo metodo verrà creato un canale diretto di comunicazione verso la
     *            controparte e verrà terminata la fase di "attesa di risposta" su tutti i dispositivi aventi
     *            lo stesso VipAddress ma sottoindirizzo diverso
     *
     * @param sys_id     - identificativo del Sistema di appartenenza
     * @param vipunit_id - identificativo della VipUnit appartenente al sistema
     * @param fsm_id     - Identificativo della macchina a stati di chiamata a cui fare la richiesta
     * @param start_video_flow - flag per avviare flusso video. Necessario, ad esempio per slave senza video.
     *
     * @return  -3 se la Macchina a stati non esiste
     *          -4 se il Sistema non esiste
     *          -6 se la ViPUnit non esiste
     *           0 se l'azionamento è andata a buon fine
     *        -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
     */
    int sysVipAnswerCall(int sys_id, int vipunit_id, int fsm_id, bool start_video_flow = true);

    /**
     * Function to handle audio communication in rx direction after playing the away message
     *
     * @param sys_id - identifier of the system
     * @param vipunit_id - identifier of the VipUnit that belongs to the system
     * @param fsm_id - identifier of finite state machine that is handling the call
     * @param start - if true requests start audio reception, if false requests interruption
     *
     * @return   -3 if FSM does not exists
     *            0 if setting is successfull
     *         -101 if the functionality is not supported by current build configuration (flag VIP_SYSTEM is missing)
     *         -200 if engine is not alive
     */
    int sysVipRequestRxAudio(int sys_id, int vipunit_id, int fsm_id, bool start);

    /**
     * Metodo per richiedere di rifiutare una chiamata e quindi uscire dallo stato "in attesa di risposta"
     *
     * @attention Dopo aver rilasciato la chiamata non sarà più possibile recuperarla su questo dispositivo.
     *            Gli altri dispositivi con stesso VipAddress ma sottoindirizzo diverso continueranno a
     *            a rimanere nello stato di "in attesa di risposta"
     *
     * @param sys_id     - identificativo del Sistema di appartenenza
     * @param vipunit_id - identificativo della VipUnit appartenente al sistema
     * @param fsm_id     - Identificativo della macchina a stati di chiamata a cui fare la richiesta
     * @param[in] cause  - Release cause (default is RelCauseNormal)
     *
     * @return  -3 se la Macchina a stati non esiste
     *          -4 se il Sistema non esiste
     *          -6 se la ViPUnit non esiste
     *           0 se l'azionamento è andata a buon fine
     *        -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
     */
    int sysVipReleaseCall(int sys_id, int vipunit_id, int fsm_id, int cause = 0);

    /**
     * Set \p sub_address like backup device of \p sub_address_backupped
     * @param sys_id             - System ID
     * @param vipunit_id         - ViP unit
     * @param slave_id           - slave identifier [0:(#MAX_SUBUNITS-1)]
     * @param slave_id_backupped - slave identifier of device to call in case of backup call [0:(#MAX_SUBUNITS-1)]
     * @param enable             - enable or disable the backup lines
     *
     * @retval -101 functionality not supported by the unit passed (VIP_SYSTEM isnt' enabled)
     * @retval -6 if unit doesn't exists (unit_id error)
     * @retval -4 if system doesn't exists (sys_id error)
     * @retval -3 if subproxy isn't active
     * @retval -1 \p slave_id or \p slave_id_backupped out of range
     * @retval  0 success
     */
    int sysVipBackupLineAdd(int sys_id, int vipunit_id, int slave_id, int slave_id_backupped, bool enable );

    /**
     * Release the FSM to the VipUnit, allowing it to be destroyed.
     *
     * @attention MUST be called if the ENABLE_FSM_REF_COUNT flag has been set in the called.
     *            MUST NOT be called otherwise.
     *
     * @param sys_id    - identificativo del Sistema di appartenenza
     * @param unit_type - id of the comelit family @see gUnitType
     * @param unit_id   - identificativo della VipUnit appartenente al sistema
     * @param fsm_id    - Identificativo della macchina a stati di chiamata a cui fare la richiesta
     *
     * @return  -3 se la Macchina a stati non esiste
     *          -4 se il Sistema non esiste
     *          -6 se la Unit non esiste
     *          -1 se il flag waitReleaseCallFromApplication non è abilitato
     *           0 se l'azionamento è andata a buon fine
     *        -101 se la funzionalità non è supportata dall'attuale compilazione della libreria
     */
    int releaseFsm(int sys_id, gUnitType unit_type, int unit_id, int fsm_id);

    /**
     * Set the vip address range
     * The vip address range is used to filter the outgoing calls
     * When the address range is configured, the outgoing calls to vip address, falling out of range, must fail
     *
     * @param sys_id              - System ID
     * @param vipunit_id          - ViP unit
     * @param authList            - A list of pair (vip address min - vip address max) to allow out calls
     *
     * @return  -6 If the ViPUnit does not exist
     *           0 if successful
     *        -101 if this functionality is not supported (missing VIP_SYSTEM compilation flag)
     */
    int sysVipSetVipAddressRangeParams(int sys_id, int vipunit_id, const std::vector<std::pair<std::string, std::string> > &authList);

    /**
     * Set a white list for dooropen action during call
     *
     * @param sys_id              - System ID
     * @param vipunit_id          - ViP unit
     * @param whiteList           - A list of pair (vip address min - vip address max) to allow dooropen during call
     *
     * @return  -6 If the ViPUnit does not exist
     *           0 if successful
     *        -101 if this functionality is not supported (missing VIP_SYSTEM compilation flag)
     */
    int sysVipSetDoorOpenWhiteList(int sys_id, int vipunit_id, const std::vector<std::pair<std::string, std::string> > &whiteList);

    // wrapper mediamanager CLASS =========================================================
    
    /**
     * Metodo statico (di Classe) per decidere se si desidera o meno la codifica e decodifica Audio effettuata
     * attraverso FFMpeg.
     * Encode  to  AV_CODEC_ID_PCM_ALAW  AV_SAMPLE_FMT_S16 (kbps: 128000, sample-rate: 8000, channels: 1)
     * Decode from AV_CODEC_ID_PCM_ALAW  AV_SAMPLE_FMT_S16 (kbps: 128000, sample-rate: 8000, channels: 1)
     *
     * @attention Se non si chiama questo metodo il default per la libreria è di non utilizzare né la codifica
     *            né la decodifica
     *
     * @param useDecodeAudio - true se si desidera che l'audio in ingresso venga decodificato
     * @param useEncodeAudio - true se si desidera che l'audio in uscita venga codificato
     * @param useFFMpegAudio - true se si desidera usare encode e decode di FFMpeg per A-Law (per avere effetto
     *                         la libreria deve esssere compilata con il flag USE_FFMPEG)
     *
     * @return   0 in caso l'operazione sia stata eseguita con successo
     *        -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
     *
     */
    int static setAudioEncodeDecode(bool useDecodeAudio, bool useEncodeAudio, bool useFFMpegAudio);

    /**
     * Metodo statico (di Classe) per decidere se si desidera o meno utilizzare la cancellazione dell'Echo
     * effettuata tramite un sottoinsieme della libreria WebRTC di Google
     *
     * @attention Per utilizzare l'echo canceller bisogna compilare la libreria con il flag WEBRTC_AEC
     * @attention Se non si chiama questo metodo il default per la libreria è di non utilizzare la cancellazione
     *            dell'echo
	 *
     * @param useEchoCanceller - true se di desidera utilizzare la funzione di cancellazione Echo, false altrimenti
	 *
     * @return   0 se è stato possiblie abilitare l'echo canceller
     *          -1 nel caso sia impossibile abilitarlo perché in fase di compilazioen si è omesso il falg WEBRTC_AEC
     *        -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
	 */
    int static setUseEchoCanceller(bool useEchoCanceller);

    /**
     * Metodo statico (di Classe) per settare il livello dell'Echo canceller
     *
     * @attention Per utilizzare il livello dell'echo canceller bisogna compilare la libreria con il flag WEBRTC_AEC
     * @attention Se non si chiama questo metodo il default per la libreria è 3
     *
     * @param levelEchoCanceller - livello da settare per la cancellazione Echo
     *
     * @return  0 se è stato possiblie settare il livello dell'echo canceller
     *         -1 nel caso sia impossibile settare il livello dell'echo canceller perché in fase di compilazioen si è omesso il falg WEBRTC_AEC
     *         -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
     */
    int static setLevelEchoCanceller(int levelEchoCanceller);

    /**
     * Metodo statico (di Classe) per settare il tempo di delay dell'Echo canceller
     *
     * @attention Per utilizzare il delay dell'echo canceller bisogna compilare la libreria con il flag WEBRTC_AEC
     * @attention Se non si chiama questo metodo il default per la libreria è 320
     *
     * @param delayEchoCanceller - tempo di delay da settare per la cancellazione Echo
     *
     * @return  0 se è stato possiblie settare il delay dell'echo canceller
     *         -1 nel caso sia impossibile settare il delay dell'echo canceller perché in fase di compilazioen si è omesso il falg WEBRTC_AEC
     *         -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
     */
    int static setDelayEchoCanceller(int delayEchoCanceller);

	/**
     * Metodo statico (di Classe) per impostare i valori di riferimento in caso si desidera applicare un fattore
     * moltiplicativo ai campioni audio in ingresso o in uscita.
     *
     * @attention Se non si chiama questo metodo il default per la libreria è il fattore moltiplicativo 1, ovvero
     *            vengono lasciati i campioni audio invariati
	 *
     * @param micVolumeShift - fattore moltiplicativo per i campioni audio estratti dal microfono
     * @param spkVolumeShift - fattore moltiplicativo per i campioni audio ricevuti sul canale rtp
     *
     * @return   0 in caso l'operazione sia stata eseguita con successo
     *        -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
	 */
    int static setAudioShiftVolume(float micVolumeShift, float spkVolumeShift);

    /**
     * Metodo statico (di Classe) per impstare quale decodifica dei frame video si desidera utilizzare.
     *
     * @attention Il metodo ha valenza solo se si è compilata la libreria con il flag USE_FFMPEG
     *            altrimenti torna -1 poiché la decodifica video è eseguita unicamente dalla libreria
     *            FFMpeg.
     *
     * @attention Se non si chiama questo metodo il default per la libreria è PIX_FMT_RGB565
     * @attention La libreria permette di utilizzare il formato RGB o BGR e un bitcolor che può essere a 24bit(24)
     *            o 16bit(565) e quindi si può scegliere una delle seguenti codifiche:
     *              PIX_FMT_RGB565
     *              PIX_FMT_RGB24
     *              PIX_FMT_BGR565
     *              PIX_FMT_BGR24
     *
     * @param format    - 0   per impostare RGB_bitcolor
     *                  - 1   per impostare BGR_bitcolor
     * @param bitcolor  - 24  per impostare format_24
     *                  - 565 per impostare foramt_565
     *
     * @return   0 se i parametri passati come argomento identificano un formato supportato
     *          -1 se non è stato trovato il formato desiderato ed è stato messo il default PIX_FMT_RGB56
     *        -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
     */
    static int setVideoPixelFormat(int format, int bitcolor);

    /**
     * Metodo satico (di Classe) per impostare la dimensione desiderata per i frame video restituiti dalla
     * libreria.
     *
     * @attention Il metodo ha valenza solo se si è compilata la libreria con il flag USE_FFMPEG
     *            altrimenti non porterà cambiamenti poiché la decodifica video è eseguita unicamente dalla libreria
     *            FFMpeg.
     *
     * @attention Se non si chiama questo metodo il default per la libreria è 320*240 (h*w)
     * @attention La libreria per variare la dimensione del frme utilizza sws_scale di FFMpeg con uno scale
     *            di tipo SWS_BICUBIC
     *
     * @param width  - larghezza desiderata del frame video
     * @param height - altezza desiderata del frame video
     *
     * @return   0 in caso l'operazione sia stata eseguita con successo
     *        -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
     */
    static int setVideoFrameSize(int width, int height);

    /**
     * Metodo statico (di Classe) per configurare la cancellazione echo, effettuata tramite un sottoinsieme della libreria WebRTC di Google.
     *
     * @attention Per utilizzare l'echo canceller bisogna compilare la libreria con il flag WEBRTC_AEC
     *
     * @param param - parametro che si desidera configurare
     * @param value - valore da impostare per il parametro
     *
     * @return   0 se è stato possibile impostare il valore desiderato
     *          -1 nel caso il valore sia fuori dal range o il parametro non esiste
     *          -2 nel caso sia impossibile effettuare l'impostazione perché in fase di compilazioen si è omesso il falg WEBRTC_AEC
     *        -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
     */
    int static setAECDefaultParameter(AECParam param, int value);

    /**
     * Metodo statico (di Classe) per configurare i parametri di buffering per pacchetti Audio e Video
     *
     * @param param - parametro che si desidera configurare
     * @param value - valore da impostare per il parametro
     *
     * @return   0 se è stato possibile impostare il valore desiderato
     *          -1 nel caso il valore sia fuori dal range o il parametro non esiste
     *        -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
     */
    int static setAVBufferParam(AVBuffParam param, int value);

    /**
     * @brief setMemoVideoMode
     * @param param - parametro che si desidera configurare
     * @param value - valore da impostare per il parametro
     * @return   0 se è stato possibile impostare il valore desiderato
     *          -1 nel caso il valore sia fuori dal range o il parametro non esiste
     *        -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
     */
    int static setMemoVideoMode(MemoVideoParam param, int value);

    // wrapper mediamanager INSTANCE =========================================================

    /**
     * Request a frame for a specific call, popping it out from a circular buffer (if available)
     *
     * @param sys_id    - system id where the requested frame's call besides
     * @param unit_type - id of the comelit family @see gUnitType
     * @param unit_id   - id of the unit where the call besides
     * @param fsm_id    - call's id of the requested frame
     * @param len       - pointer where length of the frame requested will be store
     *                    -1 if error on retrieving frame
     * @param err       - pointer where will be store an error on function call
     *                    -3 if call fsm doesn't exists (fsm_id error)
     *                    -4 if system doesn't exists (sys_id error)
     *                    -6 if unit doesn't exists (unit_id error)
     *                     0 success
     *                    -101 functionality not supported by the unit_type passed
     *
     * @param fr_info     - pointer to a struct that contains frame's information (@see frameInfo)
     *
     * @return pointer to the requested video frame
     *         NULL if error or buffer is empty
     */
    unsigned char* sysGetVideoFrame(int sys_id, gUnitType unit_type, int unit_id, int fsm_id, int* len, int* err, frameInfo* fr_info);

    /**
     * Request to decode Video Frame
     *
     * @param sys_id      - system id where the requested frame's call besides
     * @param unit_type   - id of the comelit family @see gUnitType
     * @param unit_id     - id of the unit where the call besides
     * @param fsm_id      - call's id of the requested frame
     * @param buffer_in   - pointer of the input encoded frame
     * @param buffer_out  - address in which store the pointer to the decodec frame
     * @param size_in     - size of the input buffer
     *
     * @return decoded frame buffer size
     *           -1 on error
     *           -3 if call fsm doesn't exists (fsm_id error)
     *           -4 if system doesn't exists (sys_id error)
     *           -6 if unit doesn't exists (unit_id error)
     *         -101 functionality not supported by the unit_type passed
     */
    DEPRECATED(int sysDecodeFrame(int sys_id, gUnitType unit_type, int unit_id, int fsm_id, unsigned char* buffer_in, unsigned char **buffer_out, int size_in));

    /**
     * Supply raw packet data as input to the decoder
     *
     * @param sys_id    - system id where the requested frame's call besides
     * @param unit_type - id of the comelit family @see gUnitType
     * @param unit_id   - id of the unit where the call besides
     * @param fsm_id    - call's id of the requested frame
     * @param buffer_in - pointer of the input encoded frame
     * @param size_in   - size of the input buffer
     * @param fr_info   - frame info
     * @return 0 on success, otherwise negative error code
     */
    int sysSendPacket(int sys_id, gUnitType unit_type, int unit_id, int fsm_id, unsigned char* buffer_in, int size_in, frameInfo *fr_info);

    /**
     * Return decoded output data from a decoder
     *
     * @param sys_id     - system id where the requested frame's call besides
     * @param unit_type  - id of the comelit family @see gUnitType
     * @param unit_id    - id of the unit where the call besides
     * @param fsm_id     - call's id of the requested frame
     * @param buffer_out - address in which store the pointer to the decodec frame
     * @param frame_size - size of the frame stored in buffer_out
     * @return 0: success, a frame was returned, otherwise negative error code
     */
    int sysGetFrame(int sys_id, gUnitType unit_type, int unit_id, int fsm_id, unsigned char **buffer_out, int* frame_size);

    /**
     * Request Stream Info for a specific call
     *
     * @param sys_id        - system id where the requested frame's call besides
     * @param unit_type     - id of the comelit family @see gUnitType
     * @param unit_id       - id of the unit where the call besides
     * @param fsm_id        - call's id of the requested frame
     * @param stream_info   - pointer to a struct that contains stream's information (@see streamInfo)
     * @return   -1 on error (stream_info == NULL)
     *           -3 if call fsm doesn't exists (fsm_id error)
     *           -4 if system doesn't exists (sys_id error)
     *           -6 if unit doesn't exists (unit_id error)
     *            0 success
     *         -101 functionality not supported by the unit_type passed
     */
    int sysGetStreamInfo(int sys_id, gUnitType unit_type, int unit_id, int fsm_id, streamInfo* stream_info);

    /**
     * return the status of the requsted media thread
     *
     * @param sys_id    - system id where the requested media status besides
     * @param unit_type - id of the comelit family @see gUnitType
     * @param unit_id   - id of the unit where the media requested besides
     * @param fsm_id    - call's id of the requested media status
     * @param err       - pointer where will be store an error on function call
     *                    -3 if call fsm doesn't exists (fsm_id error)
     *                    -4 if system doesn't exists (sys_id error)
     *                    -6 if unit doesn't exists (unit_id error)
     *                     0 success
     *                    -101,-102,-103,-105 functionality not supported by the unit passed
     * @param dir       - direction of the media (media received/media transmitted) @see gDir
     * @param media     - media source type (audio/video) @see gMediaType
     *
     * @return true if the requested media thread is active
     *         false if the requested media thread is off or if an error occur
     */
    bool sysIsOnSource(int sys_id, gUnitType unit_type, int unit_id, int fsm_id, int* err, gDir dir, gMediaType media);

    /**
     * Save in a file (absolute path) a frame in jpeg format
     *
     * @attention to use this method the library needs to be compiled with USE_FMFPEG and USE_FFMPEG_REC flag enabled
     *
     * @param sys_id     - system id where the requested frame besides
     * @param unit_type  - id of the comelit family @see gUnitType
     * @param unit_id    - id of the unit where the frame requested besides
     * @param fsm_id     - call's id of the requested frame
     * @param path       - file path where to save the jpeg frame (absolute path)
     * @param err        - pointer where will be store an error on function call
     *                     -3 if call fsm doesn't exists (fsm_id error)
     *                     -4 if system doesn't exists (sys_id error)
     *                     -6 if unit doesn't exists (unit_id error)
     *                      0 success
     *                     -101 functionality not supported by the unit passed
     *
     * @return   0 if the save jpeg frame request has been successfully queued
     *          -1 if the library has been compiled with USE_FFMPEG and USE_FFMPEG_REC flags off
     *        -101 functionality not supported by the unit_type passed
     */
    int sysSaveFrame(int sys_id, gUnitType unit_type, int unit_id, int fsm_id, const char *path, int* err);

    /**
     * Send a Video frame
     *
     * @param sys_id     - system id where the requested call besides
     * @param unit_type  - id of the comelit family @see gUnitType
     * @param unit_id    - id of the unit where the requested call besides
     * @param fsm_id     - call's id of the added frame
     * @param pck        - pointer to the video frame to add
     * @param len        - pointer where is stored the length of the frame to be sent (bytes)
     * @param ts         - Time Stamp of packet
     * @param packetType - frame's video codec compression (@see PacketType)
     * @param err        - pointer where will be store an error on function call
     * @param flowid     - optional CSP flow ID (MEDIAREQ_FLOWID)
     *                     -1 if packet type doesn't exist (packetType error)
     *                     -3 if call fsm doesn't exists (fsm_id error)
     *                     -4 if system doesn't exists (sys_id error)
     *                     -6 if unit doesn't exists (unit_id error)
     *                      0 success
     *                     -101 functionality not supported by the unit_type passed
     *
     */
    void sysSendVideoPck(int sys_id, gUnitType unit_type, int unit_id, int fsm_id, unsigned char* pck, int len, uint32_t ts, PacketType packetType, int* err, int flowid = 0);

    /**
     * Send an Audio packet
     *
     * @param sys_id    - system id where the requested call besides
     * @param unit_type - id of the comelit family @see gUnitType
     * @param unit_id   - id of the unit where the requested call besides
     * @param fsm_id    - call's id of the added frame
     * @param pck       - pointer to the audio packet to add
     * @param len       - pointer where is stored the length of the audio packet to be sent (bytes)
     * @param ts        - Time Stamp of packet
     * @param err       - pointer where will be store an error on function call
     *                     -3 if call fsm doesn't exists (fsm_id error)
     *                     -4 if system doesn't exists (sys_id error)
     *                     -6 if unit doesn't exists (unit_id error)
     *                      0 success
     *                     -101 functionality not supported by the unit_type passed
     *
     */
    void sysSendAudioPck(int sys_id, gUnitType unit_type, int unit_id, int fsm_id, short* pck, int len, uint32_t ts, int* err);
    
    /**
     * Send a formatted rtp packet
     * @param sys_id    - system id where the requested call besides
     * @param unit_type - id of the comelit family @see gUnitType
     * @param unit_id   - id of the unit where the requested call besides
     * @param fsm_id    - call's id to which send the rtp packet
     * @param pck       - pointer to the rtp packet to send
     * @param len       - length of the rtp packet to be sent (bytes)
     * @param err       - pointer where will be store an error on function call
     *                     -3 if call fsm doesn't exists (fsm_id error)
     *                     -4 if system doesn't exists (sys_id error)
     *                     -6 if unit doesn't exists (unit_id error)
     *                      0 success
     *                     -101 functionality not supported by the unit_type passed
     * @param flowid    - specify flowid on video if multistream enabled
     */
     void sysSendRtpPck(int sys_id, gUnitType unit_type, int unit_id, int fsm_id, const char* pck, int len, int* err, int flowid = 0);

    /**
     * Start a video recording that will be saved in the specified file path
     *
     * @param sys_id      - system id where the requested call to be recorded besides
     * @param unit_type   - id of the comelit family @see gUnitType
     * @param unit_id     - id of the unit where the frequested call to be recorded besides
     * @param fsm_id      - call's id to be recorded
     * @param path        - file path where to save the video (absolute path)
     * @param err         - pointer where will be store an error on function call
     *                      -3 if call fsm doesn't exists (fsm_id error)
     *                      -4 if system doesn't exists (sys_id error)
     *                      -6 if unit doesn't exists (unit_id error)
     *                       0 success
     *                      -101 functionality not supported by the unit passed
     * @param direction   - Registration direction RX(0) TX(1)
     * @param picWidth    - picture width
     * @param picHeight   - picture height
     * @param mediaToSave - media type to save (audio and video, only audio, only video)
     *
     * @retval    0 if the request to start recording has been executed successfully
     * @retval   -1 if the library has been compiled with @see USE_FFMPEG, @see USE_FFMPEG_REC or @see EMBEDDEDREC flags off
     * @retval   -101 functionality not supported by the unit_type passed
     */
    int sysStartRecording(int sys_id, gUnitType unit_type, int unit_id, int fsm_id, const char *path, int* err, int direction = 0, int picWidth = 0, int picHeight = 0, MediaType mediaToSave = MEDIA_TYPE_AUDIO_VIDEO);

    /**
     * Stop a video recording
     *
     * @attention to use this method the library needs to be compiled with USE_FMFPEG and USE_FFMPEG_REC flag enabled
     *
     * @param sys_id     - system id where the requested recording to be stopped besides
     * @param unit_type  - id of the comelit family @see gUnitType
     * @param unit_id    - id of the unit where the requested recording to be stopped besides
     * @param fsm_id     - call id of the recording in progress
     * @param err        - pointer where will be store an error on function call
     *                     -3 if call fsm doesn't exists (fsm_id error)
     *                     -4 if system doesn't exists (sys_id error)
     *                     -6 if unit doesn't exists (unit_id error)
     *                      0 success
     *                     -101 functionality not supported by the unit passed
     *
     * @return   0 successfully stopped recording
     *          -1 if the library has been compiled with USE_FFMPEG, USE_FFMPEG_REC or EMBEDDEDREC flags off
     *        -101 functionality not supported by the unit_type passed
     */
    int sysStopRecording(int sys_id, gUnitType unit_type, int unit_id, int fsm_id, int* err);

    /**
     * Set a thumbnail for the video
     *
     * @param sys_id         - system id where the requested recording to be stopped besides
     * @param unit_type      - id of the comelit family @see gUnitType
     * @param unit_id        - id of the unit where the requested recording to be stopped besides
     * @param fsm_id         - identificativo della macchina a stati di chiamata a cui fare la richiesta
     * @param image          - thumbnail's pointer
     * @param width          - thumbnail's width
     * @param height         - thumbnail's height
     * @param bitsDephtImage - Image bit's depth (32 - 24 ...)
     * @param err            - pointer where will be store an error on function call
     *                         -3 if call fsm doesn't exists (fsm_id error)
     *                         -4 if system doesn't exists (sys_id error)
     *                         -6 if unit doesn't exists (unit_id error)
     *                          0 success
     *                         -101 functionality not supported by the unit passed
     * @param imageFormat    - idenitfy type of image raw or compressed, view ERM_IMAGE_ from libembrecman
     * @param thumbnailSize  - size of image in case of compressed image
     * @return   0 successfully
     *          -1 if the library has been compiled with USE_FFMPEG, USE_FFMPEG_REC or EMBEDDEDREC flags off
     *        -101 functionality not supported by the unit_type passed
     */
    int sysSetThumbnail(int sys_id, gUnitType unit_type, int unit_id, int fsm_id, char * image, int width, int height, int bitsDephtImage, int* err, int imageFormat = 0, int thumbnailSize = 0);

    /**
     * Retrieve an audio packet from the circular buffer of a specific call (if it's not empty)
     *
     * @param sys_id    - system's id where call besides
     * @param unit_type - id of the comelit family @see gUnitType
     * @param unit_id   - id of the unit to retrieve audio packet
     * @param fsm_id    - call's id to retrieve audio packet
     * @param len       - pointer to audio packet lenght(in bytes), -1 in case of error
     * @param ts        - pointer to audio packet Time Stamp
     * @param err       - pointer where will be stored an error code on function call
     *                    -3 if call fsm doesn't exists (fsm_id error)
     *                    -4 if system doesn't exists (sys_id error)
     *                    -6 if unit doesn't exists (unit_id error)
     *                     0 success
     *                    -101 functionality not supported by the unit passed
     *
     * @return pointer to audio packet, NULL in case of error or empty buffer
     */
    short* sysGetAudioPck(int sys_id, gUnitType unit_type, int unit_id, int fsm_id, int* len, uint32_t* ts, int* err);
    
    /**
     * Get Raw rtp audio packet from receiving queue
     *
     * @param sys_id    - system's id where call besides
     * @param unit_type - id of the comelit family @see gUnitType
     * @param unit_id   - id of the unit to retrieve audio packet
     * @param fsm_id    - call's id to retrieve audio packet
     * @param len       - pointer to audio packet lenght(in bytes), -1 in case of error
     * @param err       - pointer where will be stored an error code on function call
     *                    -3 if call fsm doesn't exists (fsm_id error)
     *                    -4 if system doesn't exists (sys_id error)
     *                    -6 if unit doesn't exists (unit_id error)
     *                     0 success
     *                    -101 functionality not supported by the unit passed
     *
     * @return pointer to audio packet, NULL in case of error or empty buffer
     *
     * NOTE: the returned buffer, if not NULL, must not be released by the caller.
     *       The callee (the engine) is in charge of managing memory.
     */
    unsigned char *sysGetRtpAudioPck(int sys_id, gUnitType unit_type, int unit_id, int fsm_id, int* len, int* err);

    /**
     * Get Raw rtp video packet from receiving queue
     *
     * @param sys_id    - system's id where call besides
     * @param unit_type - id of the comelit family @see gUnitType
     * @param unit_id   - id of the unit to retrieve video packet
     * @param fsm_id    - call's id to retrieve video packet
     * @param len       - pointer to video packet lenght(in bytes), -1 in case of error
     * @param err       - pointer where will be stored an error code on function call
     *                    -3 if call fsm doesn't exists (fsm_id error)
     *                    -4 if system doesn't exists (sys_id error)
     *                    -6 if unit doesn't exists (unit_id error)
     *                     0 success
     *                    -101 functionality not supported by the unit passed
     *
     * @return pointer to video packet, NULL in case of error or empty buffer
     *
     * NOTE: the returned buffer, if not NULL, must not be released by the caller.
     *       The callee (the engine) is in charge of managing memory.
     */
    unsigned char *sysGetRtpVideoPck(int sys_id, gUnitType unit_type, int unit_id, int fsm_id, int* len, int* err);

    /**
     * Annulla il salvataggio su file della memo
     *
     * @attention Per utilizzare questa fuzione bisogna aver compilato la libreria con i flags USE_FFMPEG ed
     *            USE_FFMPEG_REC o EMBEDDEDREC
     *
     * @brief sysVipDiscardSaveRecording
     * @param sys_id     - identificativo del Sistema di appartenenza
     * @param vipunit_id - identificativo della VipUnit appartenente al sistema
     * @param fsm_id     - identificativo della macchina a stati di chiamata a cui fare la richiesta
     * @param path       - path assoluto del file in cui annullare il salvataggio
     * @param err        - puntatore alla variabile intera in cui la libreria riporta il valore di un evenutale errore
     *                     nella ricerca del sistema, della vipunit o della fsm
     *                      -3 se la Macchina a stati non esiste
     *                      -4 se il Sistema non esiste
     *                      -6 se la ViPUnit non esiste
     *                       0 altrimenti
     *                    -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
     * @return  0 se è stato possibile interrompere la registrazione
     *          -1 nel caso sia impossibile effettuare la richiesta perché in fase di compilazione si sono omessi i
     *             falgs USE_FFMPEG ed USE_FFMPEG_REC o EMBEDDEDREC
     *        -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
     */
    int sysVipDiscardSaveRecording(int sys_id, int vipunit_id, int fsm_id, const char *path, int* err);

    /**
     * Free data previous got out from a circular buffer
     * (using functions sysGetAudioPck or sysGetVideoFrame).
     * Using this function allow to check for memory leaks
     *
     * @param sys_id    - system id where the requested frame's call besides
     * @param unit_type - id of the comelit family @see gUnitType
     * @param unit_id   - id of the unit where the call besides
     * @param fsm_id    - call's id of the requested frame
     * @param err       - pointer where will be store an error on function call
     *                    -3 if call fsm doesn't exists (fsm_id error)
     *                    -4 if system doesn't exists (sys_id error)
     *                    -6 if unit doesn't exists (unit_id error)
     *                     0 success
     *                    -101 functionality not supported by the unit_type passed
     * @param dataPtr   - pointer to free
     */
    void freeDataPtr(int sys_id, gUnitType unit_type, int unit_id, int fsm_id, int* err, void* dataPtr);

    /**
     * Resolve a logical address into an <ip_addr,udp_port> pair.
     *
     * - If the request is for one of our identities, our <ip,port> are returned;
     * - if the requested logaddr has recently been resolved, it may still be
     *   cached: the cached <ip,port> is returned;
     * - otherwise a request is sent and 0 is returned: upon receiving a reply
     *   the result is cached, so try again later.
     *
     * @param sys_id    - System identifier
     * @param vipaddr   - Vip address of the device to be resolved.
     * @param subaddr   - Sub address of the device to be resolved.
     *                    Valid values
     *                          0           - master device
     *                          [1 - 15]    - slave devices
     * @param ip    Location for resolved IP address.
     * @param port  Location for resolved UDP port.
     * @param err   Location for error value
     *                  -4 system not found
     *                  -5 VIP-SUB address Error
     *                   0 altrimenti
     *                -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
     *
     * @retval true if the address has been resolved.
     * @retval false if the address has not been resolved.
     */
    bool sysVipAddrResolve(int sys_id, const char* vipaddr, const char* subaddr, struct in_addr *ip, int *port, int* err);

    /**
     * Richiesta di generare un nuovo key-frame
     *
     * @param sys_id     - identificativo del Sistema di appartenenza
     * @param vipunit_id - identificativo della VipUnit appartenente al sistema
     * @param fsm_id     - identificativo della macchina a stati di chiamata a cui fare la richiesta
     *
     * @return     0 se è stato possibile interrompere la registrazione
     *            -3 se la Macchina a stati non esiste
     *            -4 se il Sistema non esiste
     *            -6 se la ViPUnit non esiste
     *          -101 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag VIP_SYSTEM)
     */
    int sysVipRequestVideoKeyFrameGeneration(int sys_id, int vipunit_id, int fsm_id);

    /**
     * Allow stop media flow(s) (audio or video) for specific call.
     * @param sys_id
     * @param vipunit_id
     * @param fsm_id
     * @param channel
     * @return     0 on success
     *          -200 if engine not alive
     *            -3 if fsm not exists
     *            -4 if call cid is invalid
     *            -5 if request stop audio, but audio rx not alive
     *            -6 if request stop video, but video rx not alive
     */
    int sysVipStopMediaFlow(int sys_id, int vipunit_id, int fsm_id, MediaType channel);
    
    /**
     * Inform call FSM about the readiness to receive audio packets
     * @param sys_id     system identifier
     * @param vipunit_id vip unit identifier
     * @param fsm_id     call state machine identifier
     * @param enable     true if ready to receive audio packets; false otherwise
     *
     * @return     0 on success; < 0 on error
     */
    int sysVipSetAudioRxEnable(int sys_id, int vipunit_id, int fsm_id, bool enable);

    /**
     * Inform call FSM about starting of available media (for example if away audio msg is available)
     * @param sys_id     system identifier
     * @param vipunit_id vip unit identifier
     * @param fsm_id     call state machine identifier
     * @param channel    audio/video for channel
     * @return     0 on success; < 0 on error
     */
    int sysVipSendMediaOfferStart(int sys_id, int vipunit_id, int fsm_id, MediaType channel);

    /**
     * Inform call FSM about stopping of available media (for example if away audio msg is available)
     * @param sys_id     system identifier
     * @param vipunit_id vip unit identifier
     * @param fsm_id     call state machine identifier
     * @param channel    audio/video for channel
     * @return     0 on success; < 0 on error
     */
    int sysVipSendMediaOfferStop(int sys_id, int vipunit_id, int fsm_id, MediaType channel);
        
    // SAFE - Inizializzazione ======================================================================
    
    /**
     * Creation of an element of the Safe world "SafeUnit" within a given System
     *
     * @attention This method was deprecated, now must be used ::sysCreateSafeUnitWithTcpConnection
     * @attention A System (intended as library level) can contain only one Safe element in the case of Viper Tunnel use
     * @attention The pair [agentType-agentID] must be unique within a Safe System
     *
     * @param sys_id         - System identifier to which to associate the Vip element
     * @param deviceType     - Type of device to be used in communication with the Safe control panel
     * @param deviceID       - Unique ID for type of device to be used in communication with the Safe control panel [1 to N]
     * @param devicePassword - Device passowrd used to do the login of LEVEL 1
     * @param rtpMtu         - Maximum MTU size for the RTP protocol
     *
     * @return  Unique id of the created SafeUnit
     *          -1 it wasn'o't possible to create a new SafeUnit (possible cause is in the case of Viper Tunnel where there can be only one)
     *          -4 the ID system doesn't exist
     *        -102 if the functionality isn't supported by the current compilation of the library (missing flag SAFE_SYSTEM)
     */
    DEPRECATED(int sysCreateSafeUnit(int sys_id, SafeDeviceType deviceType, unsigned int deviceID, 
                                     unsigned int devicePassword, unsigned int rtpMtu=MAX_RTP_MTU));

    /**
     * Creation of SafeUnit inside of system which will communication via TCP.
     *
     * @attention A system (inside of library) can contain only one SafeUnit if ViperTunnel is used.
     *
     * @param sys_id      - ID of system at which to associate the SafeUnit
     * @param deviceType  - Device type to use during communication RemoteCom
     * @param deviceID    - ID device to use during communication RemoteCom (ID range: 1 to N)
     * @param devicePassword - Device passowrd used to do the login of LEVEL 1
     * @param address     - IP control panel
     * @param port        - TCP port of control panel
     * @param rtpMtu      - Max size of MTU for RTP protocol
     * @param control_panel_info - Info of the control panel
     *
     * @attention The AgenTyp-AgentId(deviceType-deviceID) pair must be unique inside the Safe system
     *
     * @return   it's succeeded, the ID of Safe Unit created
     *          -1 it wasn't possibile to create the SafeUnit
     *          -4 the system doesn't exist
     *        -102 the feature isn't supported from current library compilation (lost flag SAFE_SYSTEM)
     */
    int sysCreateSafeUnitWithTcpConnection(int sys_id, SafeDeviceType deviceType, unsigned int deviceID,
                                           unsigned int devicePassword, const char* address, int port,
                                           unsigned int rtpMtu=MAX_RTP_MTU,
                                           struct_control_panel_info* control_panel_info=NULL);

    /**
     * Creation of SafeUnit inside of system which will communication via MQTT.
     *
     * @attention A system (inside of library) can contain only one SafeUnit if ViperTunnel is used.
     *
     * @param sys_id      - ID of system at which to associate the SafeUnit
     * @param deviceType  - Device type to use during communication RemoteCom
     * @param deviceID    - ID device to use during communication RemoteCom (ID range: 1 to N)
     * @param devicePassword - Device password used to do the login of LEVEL 1
     * @param mqttTriangulationParameters - The @see(SafeMqttTriangulationParameters) needed to establish the connection
     * @param p2pParameters - The @see(p2pParameters) needed to connect to the cameras via P2P (supported only for HUB control panel)
     * @param rtpMtu     - Max size of MTU for RTP protocol
     * @param control_panel_info - Info of the control panel
     * @param isCloudNextCompatible - If true the dynamic topics is enabled, otherwise false
     *
     * @attention The AgenTyp-AgentId(deviceType-deviceID) pair must be unique inside the Safe system
     *
     * @return   it's succeeded, the ID of Safe Unit created
     *          -1 it wasn't possibile to create the SafeUnit
     *          -4 the system doesn't exist
     *        -102 the feature isn't supported from current library compilation (lost flag SAFE_SYSTEM)
     */
    int sysCreateSafeUnitWithMqttConnection(int sys_id, SafeDeviceType deviceType, unsigned int deviceID,
                                            unsigned int devicePassword,
                                            const SafeMqttTriangulationParameters &mqttTriangulationParameters,
                                            const p2pParameters &p2pParameters,
                                            unsigned int rtpMtu=MAX_RTP_MTU,
                                            struct_control_panel_info* control_panel_info=NULL,
                                            bool isCloudNextCompatible=false);

    /**
     * Rimozione di un elemento del mondo Vip "SafeUnit" all'interno di un dato Sistema.
     *
     * @attention Un Sistema (inteso a livello di libreria) può contenere un solo elemento Safe
     *             nel caso di utilizzo ViperTunnel
     *
     * @param sys_id - identificativo del Sistema a cui è associata la VipUnit da Rimuovere
     * @param safeunit_id - identificativo della SafeUnit da rimuovere
     *
     * @return   0 se la rimozione è andata a buon fine
     *          -4 se il Sistema indicato non esiste
     *          -6 se la SafeUnit che si vuole eliminare non esiste nel sistema indicato
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysRemoveSafeUnit(int sys_id, int safeunit_id);
    
    /**
     * Impostazione della password utente utilizzata per la lettura della configurazione dalla centrale
     *
     * @attention assicurarsi che la password si riferisca ad un utente nella configurazione della centrale
     *      con tutte le autorizzazioni necessarie per la lettura dei parametri desiderati
     *
     * @param sys_id        - identificativo del Sistema di appartenenza
     * @param safeunit_id   - identificativo della VipUnit appartenente al sistema
     * @param password      - password utente
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *           0 impostazione andata a buon fine
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeSetCfgUserPassword(int sys_id, int safeunit_id, const char* password);
    
    /**
     * @brief sysSafeSetApplicationInfo
     * Define all info about software/firmware and hardware relate to the applicantion level
     *
     *
     * @param sysId   - ID of the System to which it belongs
     * @param safeUnitId   - ID of the SafeUnit to which it belongs
     * @param applicationInfo   - Data about the firmware version of application
     */
    int sysSafeSetApplicationInfo(int sysId, int safeUnitId, const safeApplicationInfo_t &applicationInfo);
    /**
     * Setting of the local connection parameters with the control panel
     *
     * @attention This method was deprecated, now must be used ::sysCreateSafeUnitWithTcpConnection
     *
     * @param sys_id        - System identifier to which the unit is associated
     * @param safeunit_id   - Identification of the SafeUnit belonging to the system
     * @param tcpPort       - TCP port
     * @param netMask       - Netmask of local network
     * @param address       - IP address / Hostname
     *
     * @return  -1 if the connection failed
     *          -4 the ID system doesn't exist
     *          -6 if the SafeUnit doesn't exist
     *           0 setting successful
     *        -102 if the functionality isn't supported by the current compilation of the library (missing flag SAFE_SYSTEM)
     */
    DEPRECATED(int sysSafeSetMainBoardConnParams(int sys_id, int safeunit_id, const char* tcpPort, const char* netMask, const char* address));
    
    //SAFE - wrapper Config Reader =========================================================

    /**
     * Attiva il gestore che si occupa di recuperare dalla centrale le informazioni realtime richieste
     *
     * @param sys_id identificativo del Sistema di appartenenza
     * @param safeunit_id identificativo della VipUnit appartenente al sistema
     * @param groupType gruppo di parametro real time richiesto
     * @param paramReqMask bit mask che indica quale stato specifico viene richiesto alla centrale (MAX 1 stato -> un solo bit a 1)
     * @param paramReqMaskSize dimensioni in byte della maschera significativi
     * @param ret_param_id  (4byte) id del parametro che verrà ritornato alla callback come passato alla richiesta senza subire alcuna modifica
     *
     * @param cyclicReading indica se la richiesta parametri è di tipo ciclico o una tantum
     *
     * @attention le richieste di lettura di tipo ciclico vanno bloccate dall'applicativo che ne richiede l'avvio!!!
     *
     * @attention il campo paramReqMask viene utilizzato solo per SAFE_RT_ZONE_STATUS_SPEC, deve essere valorizzato con uno o più degli stati (SafeRtSpecZoneStatus in OR) da richiedere alla centrale
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *           0 richiesta rifiutata
     *           1 richiesta presa in carico correttamente
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeReadRtParams(int sys_id, int safeunit_id, SafeRtParamsGroupType groupType, unsigned long paramReqMask, unsigned int paramReqMaskSize, bool cyclicReading, unsigned int ret_param_id);
    
    /**
     * Disattiva il gestore che si occupa di recuperare dalla centrale
     * 	le informazioni realtime richieste NB: la funzione non ferma immediatamente la lettura parametri,
     * 	in realtà il ciclo verrà terminato con la fine della procedura di lettura in corso
     *
     * @param sys_id identificativo del Sistema di appartenenza
     * @param safeunit_id identificativo della VipUnit appartenente al sistema
     * @param groupType tipo di parametro real time richiesto
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *           0 altrimenti
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeStopReadRtParams(int sys_id, int safeunit_id, SafeRtParamsGroupType groupType);
    
    /**
     * Richiede al gestore se le informazioni real time sono state modificate dall'ultima chiamata di questa funzione
     *
     * @param sys_id identificativo del Sistema di appartenenza
     * @param safeunit_id identificativo della VipUnit appartenente al sistema
     * @param groupType tipo di parametro real time sul quale effettuare il controllo
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *           0 se le informazioni NON sono state modificate
     *           1 se le informazioni sono state modificate
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeRtParamsChanged(int sys_id, int safeunit_id, SafeRtParamsGroupType groupType);
    
    /**
     * Attiva il gestore che si occupa di recuperare dalla centrale
     * 	le informazioni database singole richieste
     *
     * @param sys_id identificativo del Sistema di appartenenza
     * @param safeunit_id identificativo della VipUnit appartenente al sistema
     * @param paramType tipo di parametro database richiesto
     * @param reqParam parametro dipendente dalla tipologia del parametro richiesto (DB_EVENT_LOG -> ID logico del record da 1 a XX)
     * @param ret_param_id  (4byte) id del parametro che verrà ritornato alla callback come passato alla richiesta senza subire alcuna modifica
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *           0 richiesta rifiutata
     *           1 richiesta presa in carico correttamente
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeReadSingleDbParams(int sys_id, int safeunit_id, SafeCfgParamSingleType paramType, unsigned int reqParam, unsigned int ret_param_id);
    
    /**
     * Attiva il gestore che si occupa di recuperare dalla centrale
     * 	le informazioni database del gruppo di parametri richiesto
     *
     * @param sys_id identificativo del Sistema di appartenenza
     * @param safeunit_id identificativo della VipUnit appartenente al sistema
     * @param groupType tipo di gruppo di parametri database richiesto
     * @param reqParam parametro dipendente dalla tipologia del parametro richiesto (Per ora non utilizzato, 0)
     * @param ret_param_id  (4byte) id del parametro che verrà ritornato alla callback come passato alla richiesta senza subire alcuna modifica
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *           0 richiesta rifiutata
     *           1 richiesta presa in carico correttamente
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeReadGroupDbParams(int sys_id, int safeunit_id, SafeCfgParamsGroupType groupType, unsigned int reqParam, unsigned int ret_param_id);
    
    /**
     * Attiva il gestore che si occupa di recuperare automaticamente dalla centrale
     * 	le informazioni database del gruppo di parametri ogni qualvolta venga rilevata una modifica 
     *  ATTENZIONE: NON attivabile per il gruppo SAFE_CFG_GROUP_DB_SIGNATURES
     *
     * @param sys_id      - identificativo del Sistema di appartenenza
     * @param safeunit_id - identificativo della VipUnit appartenente al sistema
     * @param groupType   - gruppo di parametri database da tenere aggiornato automaticamente
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *           0 richiesta rifiutata
     *           1 richiesta presa in carico correttamente
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeEnableAutoCheckGroupDbParams(int sys_id, int safeunit_id, SafeCfgParamsGroupType groupType);
    
    /**
     * Disattiva il gestore che si occupa di recuperare automaticamente dalla centrale
     * 	le informazioni database del gruppo di parametri ogni qualvolta venga rilevata una modifica
     *
     * @param sys_id identificativo del Sistema di appartenenza
     * @param safeunit_id identificativo della VipUnit appartenente al sistema
     * @param groupType gruppo di parametri database da NON tenere aggiornato automaticamente
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *           0 richiesta rifiutata
     *           1 richiesta presa in carico correttamente
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeDisableAutoCheckGroupDbParams(int sys_id, int safeunit_id, SafeCfgParamsGroupType groupType);
    
    // SAFE - wrapper Gestione Log Eventi / Allarme ========================
    
    /**
     * Invia la richiesta dell'ultimo record del log eventi richiesto
     *
     * @param sys_id       - identificativo del Sistema di appartenenza
     * @param safeunit_id  - identificativo della VipUnit appartenente al sistema
     * @param logType      - tipo di log richiesto
     * @param log_mode     - required log mode @see SafeLogMode
     * @param ret_param_id - (4byte) id del parametro che verrà ritornato alla callback come passato alla richiesta senza subire alcuna modifica
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *           0 richiesta rifiutata
     *           1 richiesta presa in carico correttamente
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeGetFirstLog(int sys_id, int safeunit_id, SafeLogType logType, SafeLogMode log_mode, unsigned int ret_param_id);
    
    /**
     * Invia la richiesta del successivo record del log eventi rispetto a quello passato
     *
     * @param sys_id       - identificativo del Sistema di appartenenza
     * @param safeunit_id  - identificativo della VipUnit appartenente al sistema
     * @param logType      - tipo di log richiesto
     * @param logId        - indice del log corrente
     * @param log_mode     - required log mode @see SafeLogMode
     * @param ret_param_id - (4byte) id del parametro che verrà ritornato alla callback come passato alla richiesta senza subire alcuna modifica
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *           0 richiesta rifiutata
     *           1 richiesta presa in carico correttamente
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeGetNextLog(int sys_id, int safeunit_id, SafeLogType logType, unsigned int logId, SafeLogMode log_mode, unsigned int ret_param_id);
    
    /**
     * Invia la richiesta del precedente record del log eventi rispetto a quello passato
     *
     * @param sys_id       - identificativo del Sistema di appartenenza
     * @param safeunit_id  - identificativo della VipUnit appartenente al sistema
     * @param logType      - tipo di log richiesto
     * @param logId        - indice del log corrente
     * @param log_mode     - required log mode @see SafeLogMode
     * @param ret_param_id - (4byte) id del parametro che verrà ritornato alla callback come passato alla richiesta senza subire alcuna modifica
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *           0 richiesta rifiutata
     *           1 richiesta presa in carico correttamente
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeGetPrevLog(int sys_id, int safeunit_id, SafeLogType logType, unsigned int logId, SafeLogMode log_mode, unsigned int ret_param_id);
    
    //SAFE - wrapper Action Handler =========================================================
    
    /**
     * Funzione che accoda la richiesta di azioni singole di ottenimento aree da inserire
     *
     * @param sys_id identificativo del Sistema di appartenenza
     * @param safeunit_id identificativo della VipUnit appartenente al sistema
     * @param act_type sotto tipo azione che definisce il tipo di inserimento (SafeActivationType) per le quali si richiede le aree che verranno inserite
     * @param action_flag flag che specificano il comportamento dell'azione (vedi SafeActionOption)
     * @param param_len numero di byte significativi che portano l'informazione parameter (sempre a 0 oppure
     * 		MAX_AREAS per inserimenti di tipo CMD_INS_DEV)
     * @param parameter_buff buffer di MAX_AREAS u8 contenenti l'informazione parameter (sempre a NULL oppure buffer di
     * 		MAX_AREAS byte di tipo per inserimenti di tipo "ins_f" CMD_INS_DEV, uno per ciascuna area)
     * @param ret_param_id  (4byte) id del parametro che verrà ritornato alla callback come passato alla richiesta senza subire alcuna modifica
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *           0 richiesta rifiutata
     *           1 richiesta presa in carico correttamente
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeGetAreeToIns(int sys_id, int safeunit_id, SafeActivationType act_type, unsigned char action_flag, unsigned char param_len, unsigned char* parameter_buff, unsigned int ret_param_id);
    
    /**
     * Funzione che accoda la richiesta di azioni singole di inserimento/disinserimento
     *
     * @param sys_id identificativo del Sistema di appartenenza
     * @param safeunit_id identificativo della VipUnit appartenente al sistema
     * @param act_type sotto tipo azione che definisce il tipo di inserimento richiesto SafeActivationType
     * @param action_flag flag che specificano il comportamento dell'azione (vedi SafeActionOption)
     * @param param_len numero di byte significativi che portano l'informazione parameter (sempre a 0 oppure
     * 		MAX_AREAS per inserimenti di tipo CMD_INS_DEV)
     * @param parameter_buff buffer di MAX_AREAS u8 contenenti l'informazione parameter (sempre a NULL oppure buffer di
     * 		MAX_AREAS byte di tipo per inserimenti di tipo "ins_f" CMD_INS_DEV, uno per ciascuna area)
     * @param ret_param_id  (4byte) id del parametro che verrà ritornato alla callback come passato alla richiesta senza subire alcuna modifica
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *           0 richiesta rifiutata
     *           1 richiesta presa in carico correttamente
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeSendIns(int sys_id, int safeunit_id, SafeActivationType act_type, unsigned char action_flag, unsigned char param_len, unsigned char* parameter_buff, unsigned int ret_param_id);
    
    /**
     * Funzione che accoda la richiesta di azioni singole di allarme
     *
     * @param sys_id       - identificativo del Sistema di appartenenza
     * @param safeunit_id  - identificativo della VipUnit appartenente al sistema
     * @param alarm_type   - sotto tipo azione che definisce il tipo di allarme (SafeAlarmType)
     * @param ret_param_id - (4byte) id del parametro che verrà ritornato alla callback come passato alla richiesta senza subire alcuna modifica
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *           0 richiesta rifiutata
     *           1 richiesta presa in carico correttamente
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeSendAlarm(int sys_id, int safeunit_id, SafeAlarmType alarm_type, unsigned int ret_param_id);
    
    /**
     * Funzione che accoda la richiesta di azioni singole di comando
     *
     * @param sys_id identificativo del Sistema di appartenenza
     * @param safeunit_id identificativo della VipUnit appartenente al sistema
     * @param cmd_type sotto tipo azione che definisce il tipo di comando (e_cmd_azioni)
     * @param arg Parametro da fornire solo per i comandi reset ::CMD_RESET_CENTRALE e ::CMD_RESET_ALL.
     * @param ret_param_id  (4byte) id del parametro che verrà ritornato alla callback come passato alla richiesta senza subire alcuna modifica
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *           0 richiesta rifiutata
     *           1 richiesta presa in carico correttamente
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeSendCommand(int sys_id, int safeunit_id, SafeCmdType cmd_type, unsigned int arg, unsigned int ret_param_id);
    
    /**
     * Funzione che accoda la richiesta di azioni singole di attivazione uscite
     *
     * @param sys_id identificativo del Sistema di appartenenza
     * @param safeunit_id identificativo della VipUnit appartenente al sistema
     * @param phy_id sotto tipo azione che definisce l'id fisico dell'uscita sulla quale è richiesta l'azione
     * @param ret_param_id  (4byte) id del parametro che verrà ritornato alla callback come passato alla richiesta senza subire alcuna modifica
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *           0 richiesta rifiutata
     *           1 richiesta presa in carico correttamente
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeSendEnableOutput(int sys_id, int safeunit_id, unsigned int phy_id, unsigned int ret_param_id);
    
    /**
     * Funzione che accoda la richiesta di azioni singole di disattivazione uscite
     *
     * @param sys_id identificativo del Sistema di appartenenza
     * @param safeunit_id identificativo della VipUnit appartenente al sistema
     * @param phy_id sotto tipo azione che definisce l'id fisico dell'uscita sulla quale è richiesta l'azione
     * @param ret_param_id  (4byte) id del parametro che verrà ritornato alla callback come passato alla richiesta senza subire alcuna modifica
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *           0 richiesta rifiutata
     *           1 richiesta presa in carico correttamente
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeSendDisableOutput(int sys_id, int safeunit_id, unsigned int phy_id, unsigned int ret_param_id);
    
    /**
     * Funzione che accoda la richiesta di azioni singole di bascula uscite
     *
     * @param sys_id identificativo del Sistema di appartenenza
     * @param safeunit_id identificativo della VipUnit appartenente al sistema
     * @param phy_id sotto tipo azione che definisce l'id fisico dell'uscita sulla quale è richiesta l'azione
     * @param ret_param_id  (4byte) id del parametro che verrà ritornato alla callback come passato alla richiesta senza subire alcuna modifica
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *           0 richiesta rifiutata
     *           1 richiesta presa in carico correttamente
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeSendToggleOutput(int sys_id, int safeunit_id, unsigned int phy_id, unsigned int ret_param_id);
    
    /**
     * Funzione che accoda la richiesta di azioni singole su zone (Esclude/include/isola/attiva una zona)
     *
     * @param sys_id       - identificativo del Sistema di appartenenza
     * @param safeunit_id  - identificativo della VipUnit appartenente al sistema
     * @param act_type     - sotto tipo azione che definisce il tipo di azione da effettuarsi sulla zona (e_azioni_zona)
     * @param id_phy_zone  - id fisico della zona sulla quale è richiesta l'azione
     * @param ret_param_id - (4byte) id del parametro che verrà ritornato alla callback come passato alla richiesta senza subire alcuna modifica
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *           0 richiesta rifiutata
     *           1 richiesta presa in carico correttamente
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeSendZoneAction(int sys_id, int safeunit_id, SafeZoneActionType act_type, unsigned int id_phy_zone, unsigned int ret_param_id);
    
    /**
     * Funzione che accoda la richiesta di azione singola generica
     *
     * @param sys_id identificativo del Sistema di appartenenza
     * @param safeunit_id identificativo della VipUnit appartenente al sistema
     * @param action_type tipo azione
     * @param action_sub_type sottotipo azione, dipende direttamente dal tipo azione
     * @param action_flag flag che specificano il comportamento dell'azione (Da utilizzare una o più flag di SafeActionOption in OR)
     * @param utf8_param_buff Parametro aggiuntivo dell'azione:
     *    E' un array di [SafeAreaActivationType + SafeAreaActivationFlag] per inserimenti di tipo SAFE_CMD_DEVICE_ACTIVATION, 
     *      uno per ciascuna area, i valori numerici convertiti in stringa sono separati da ";"
     * 	  E' l'id logico di zona per azioni tipo SAFE_ACTION_ON_ZONE, numero convertito in stringa
     *    E' l'enumerato SafeCmdResetAlarmType per i soli comandi SAFE_CMD_ALARMS_RESET e SAFE_CMD_MAIN_UNIT_RESET, numero convertito in stringa]
     * @param ret_param_id  (4byte) id del parametro che verrà ritornato alla callback come passato alla richiesta senza subire alcuna modifica
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *          -1 parametri immessi errati
     *           0 richiesta rifiutata
     *           1 richiesta presa in carico correttamente
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeSendGenericAction(int sys_id, int safeunit_id, SafeActionType action_type, unsigned int action_sub_type, unsigned int action_flag, const char* utf8_param_buff, unsigned int ret_param_id);
    
    /**
     * Funzione che accoda la richiesta di azione di scrittura data ora in centrale
     *
     * @param sys_id identificativo del Sistema di appartenenza
     * @param safeunit_id identificativo della VipUnit appartenente al sistema
     * @param info_tm informazione temporale
     * @param ret_param_id  (4byte) id del parametro che verrà ritornato alla callback come passato alla richiesta senza subire alcuna modifica
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *           0 richiesta rifiutata
     *           1 richiesta presa in carico correttamente
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeSendWriteTimeAction(int sys_id, int safeunit_id, struct tm info_tm, unsigned int ret_param_id);
    
    /**
     * Funzione che accoda la richiesta di azione di lettura data ora in centrale
     *
     * @param sys_id identificativo del Sistema di appartenenza
     * @param safeunit_id identificativo della VipUnit appartenente al sistema
     * @param ret_param_id  (4byte) id del parametro che verrà ritornato alla callback come passato alla richiesta senza subire alcuna modifica
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *           0 richiesta rifiutata
     *           1 richiesta presa in carico correttamente
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeSendReadTimeAction(int sys_id, int safeunit_id, unsigned int ret_param_id);
    
    // wrapper Config Writer =========================================================
    
    /**
     * Adds the request of update related to flags of auto-arming for each specified areas
     *
     * @param sys_id Identification of the system to which the command belongs.
     * @param safeunit_id Identification of the unit to which the command belongs.
     * @param aree_bitmask Areas bitmask to select the areas which must be updated (bit #0: area #1 / bit #1:area #2 ...)
     * @param value_bitmask Areas bitmask to set the values of each areas selected (value of bit #0 = value of flag about area #1 ... )
     * @param num_aree number of areas to be enhanced (areas from 1 to num_aree will be considered)
     * @param ret_param_id Request ID to allow the application to associate the response to the request
     *
     * @return  -4 The system doesn't exist
     *          -6 The unit doesn't exist
     *           0 Refiused request
     *           1 Accepted request
     *        -102 If the functionality is not supported by the current compilation of the library (missing flag SAFE_SYSTEM)
     */
    int sysSafeUpdateAutoEnableTimers(int sys_id, int safeunit_id, unsigned int aree_bitmask, unsigned int value_bitmask, unsigned char num_aree, unsigned int ret_param_id);
    
    /**
     * Funzione che accoda la richiesta di aggiornamento dell'azione associata ad un tasto in homepage
     *
     * @param sys_id identificativo del Sistema di appartenenza
     * @param safeunit_id identificativo della VipUnit appartenente al sistema
     * @param btn_home_page_type identificativo del tasto in home page
     * @param action_type tipo azione
     * @param action_sub_type sottotipo azione, dipende direttamente dal tipo azione
     * @param ret_param_id id del parametro che verrà ritornato come passato alla richiesta senza subire alcuna modifica
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *           0 richiesta rifiutata
     *           1 richiesta presa in carico correttamente
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeUpdateBtnHomePage(int sys_id, int safeunit_id, SafeBtnHomeType  btn_home_page_type, SafeActionType action_type, unsigned int action_sub_type, unsigned int ret_param_id);
    
    /**
     * Funzione che accoda la richiesta di aggiornamento di un parametro di sistema
     *
     * @param sys_id identificativo del Sistema di appartenenza
     * @param safeunit_id identificativo della VipUnit appartenente al sistema
     * @param sys_params_type identificativo del parametro di sistema
     * @param param_value valore dipendente dalla tipologia di parametro passato
     * @param ret_param_id id del parametro che verrà ritornato come passato alla richiesta senza subire alcuna modifica
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *           0 richiesta rifiutata
     *           1 richiesta presa in carico correttamente
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeUpdateSysParams(int sys_id, int safeunit_id, SafeSysParamType sys_params_type, unsigned int param_value, unsigned int ret_param_id);
    
    /**
     * Funzione che accoda la richiesta di aggiornamento di un parametro di un record in rubrica
     *
     * @param sys_id identificativo del Sistema di appartenenza
     * @param safeunit_id identificativo della VipUnit appartenente al sistema
     * @param contact_params_type identificativo del parametro del record rubrica
     * @param utf8_param_value stringa valore del parametro (NB: Sono da convertire in stringa anche i parametri numerici)
     * @param record_index indice di database del record in rubrica (da 0 a MAX_RECORD_IN_RUBRICA-1)
     * @param ret_param_id id del parametro che verrà ritornato come passato alla richiesta senza subire alcuna modifica
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *          -1 parametri immessi errati
     *           0 richiesta rifiutata
     *           1 richiesta presa in carico correttamente
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeUpdateRubricaParams(int sys_id, int safeunit_id, SafeContactParamType contact_params_type, const char* utf8_param_value, unsigned short record_index, unsigned int ret_param_id);
    
    /**
     * Funzione che accoda la richiesta di aggiornamento dei parametri degli utenti/installatori
     *
     * @param sys_id identificativo del Sistema di appartenenza
     * @param safeunit_id identificativo della VipUnit appartenente al sistema
     * @param user_params_type identificativo del parametro
     * @param utf8_param_value stringa valore del parametro (NB: Sono da convertire in stringa anche i parametri numerici)
     * @param user_logical_id indice logico dell'utente, va da 1 a (MAX_CODICI_UTE + MAX_CODICI_INST), parametro id della db_utenti_safe_t
     * @param ret_param_id id del parametro che verrà ritornato come passato alla richiesta senza subire alcuna modifica
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *          -1 parametri immessi errati
     *           0 richiesta rifiutata
     *           1 richiesta presa in carico correttamente
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeUpdateUserParams(int sys_id, int safeunit_id, SafeUserParamType user_params_type, const char* utf8_param_value, unsigned short user_logical_id, unsigned int ret_param_id);
    
    /**
     * Funzione che accoda la richiesta di aggiornamento dei parametri delle chiavette RFID
     *
     * @param sys_id                identificativo del Sistema di appartenenza
     * @param safeunit_id           identificativo della VipUnit appartenente al sistema
     * @param rfid_key_params_type  identificativo del parametro
     * @param utf8_descr            descrizione in utf8 della chiavetta (MAX 15 caratteri)
     * @param key_code              stringa contenente il codice univoco associato alla chiavetta, così come letto dalla chiavetta, non elaborato se is_key_code_from_cfg = false, già elaborato così come ricevuto dalla configurazione se is_key_code_from_cfg = true.
     * @param is_key_code_from_cfg  booleano che indica se il codice arriva dalla configurazione della centrale o è un codice chiave da elaborare per il salvataggio in configurazione della centrale.
     * @param key_id                identificativo della chiave
     * @param user_id               utente possessore della chiave (Associazione biettiva ad un utente una chiave)
     * @param active_status         stato della chiave true:Attiva, false:Assente
     * @param colour                colore associato alla chiavetta
     * @param ret_param_id          id del parametro che verrà ritornato come passato alla richiesta senza subire alcuna modifica
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *           0 richiesta rifiutata
     *           1 richiesta presa in carico correttamente
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeUpdateRfidKeyParams(int sys_id, int safeunit_id, SafeRfidKeyParamType rfid_key_params_type, const char* utf8_descr,
                                    const char* key_code, bool is_key_code_from_cfg, int key_id, unsigned int user_id, bool active_status, SafeRfidKeyColourType colour, unsigned int ret_param_id);
    
    /**
     * Funzione che accoda la richiesta di aggiornamento delle flag di attivazione
     * timer scheduler (NB: viene passato il valore di tutti i timer e cosÏ viene scritto nel DB della centrale)
     *
     * @param sys_id identificativo del Sistema di appartenenza
     * @param safeunit_id identificativo della VipUnit appartenente al sistema
     * @param selection_bitmask bitmask per l'indicazione del valore delle flag da passare (valore bit #0 = valore flag di attivazione timer ID DB 0 ... )
     * @param ret_param_id id del parametro che verrà ritornato come passato alla richiesta senza subire alcuna modifica
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *           0 richiesta rifiutata
     *           1 richiesta presa in carico correttamente
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeUpdateEnableTimersScheduler(int sys_id, int safeunit_id, unsigned int selection_bitmask, unsigned int ret_param_id);

    /**
     * Funzione che accoda la richiesta dei parametri dei dispositivi touch
     *
     * @param sys_id identificativo del Sistema di appartenenza
     * @param safeunit_id identificativo della VipUnit appartenente al sistema
     * @param device_id id dispositivo touch
     * @param gen_touch_params_type parametro da modificare
     * @param ret_param_id id del parametro che verrà ritornato come passato alla richiesta senza subire alcuna modifica
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *           0 richiesta rifiutata
     *           1 richiesta presa in carico correttamente
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeUpdateGenericTouchParameter(int sys_id, int safeunit_id, unsigned int device_id, SafeGenTouchParamType gen_touch_params_type, unsigned int param_value, unsigned int ret_param_id);

    /**
     * Funzione che accoda la richiesta di aggiornamento dei parametri relativi alle psh notification del dispositivo corrente.
     *
     * @note Per tutti i parametri della struttura @see pushNotificationInfo deve essere eseguito il padding a 0.
     *
     * @param sys_id identificativo del Sistema di appartenenza.
     * @param safeunit_id identificativo della SafeUnit appartenente al sistema.
     * @param push_notification_params Struttura che contiene tutti i parametri relativi alla configurazione delle push notification in centrale.
     * @param ret_param_id Id del parametro che verrà ritornato alla callback come passato alla richiesta senza subire alcuna modifica.
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *          -1 se la pushNotificationInfo non esiste
     *           0 richiesta rifiutata
     *           1 richiesta presa in carico correttamente
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeUpdatePushNotificationConfiguration(int sys_id, int safeunit_id, pushNotificationInfo* push_notification_params, unsigned int ret_param_id);
    
    // wrapper Log Media handler =========================================================
    
    /**
     * Ritorna lo stato del gestore di scaricamento immagini
     *
     * @param sys_id        identificativo del Sistema di appartenenza
     * @param safeunit_id   identificativo della VipUnit appartenente al sistema
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *           0 Log image downloader inattivo
     *           1 Log image downloader attivo
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeLogImagesDownloaderIsRunning(int sys_id, int safeunit_id);
    
    /**
     * Richiesta al gestore di scaricamento media di effettuare una operazione di verifica, scaricamento immagini o video
     *
     * @param sys_id                identificativo del Sistema di appartenenza
     * @param safeunit_id           identificativo della VipUnit appartenente al sistema
     * @param port                  porta TCP del gestore media (connessione locale) oppure del router usato per il forwording al gestore media (connessione da remoto)
     * @param address               indirizzo IP del gestore media (connessione locale) oppure del router usato per il forwording al gestore media (connessione da remoto)
     * @param command_req           tipo di comando da richiedere al gestore media
     * @param date_time_info        informazione temporale associata all'immagine
     * @param log_device_type       tipo di dispositivo associato all'immagine
     * @param log_device_id         iD univoco per tipo di dispositivo associato all'immagine [da 1 a N]
     * @param camera_device_type    tipo dispositivo di gestione telecamere
     * @param camera_device_id      id modulo telecamera
     * @param camera_id             id telecamera
     * @param do_holepunch          flag per indicare, se necessario, di inviare il paccketto di holepunch per bucare i NAT degli operatori di rete (usato solo per lo streaming video con connessione da remoto)
     * @param holepunch_port        porta alla quale inviare il pacchetto di holepunch, va usata solo da remoto (SAFE_LOG_MEDIA_MOVIE_START_STREAM)
     * @param fsm_id                id della chiamata a cui si riferisce il comando (SAFE_LOG_MEDIA_MOVIE_STOP_STREAM)
     * @param ret_param_id          parametro ritornato dalla callback im modo da poter discriminare sull'esito del comando ricevuto
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *          -1 errore nei parametri immessi
     *           0 richiesta rifiutata
     *           1 richiesta presa in carico correttamente
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */

    int sysSafeLogMediaCommandRequest(int sys_id, int safeunit_id, const char* port, const char* address, SafeLogMediaReq command_req, time_t date_time_info, SafeDeviceType log_device_type,
                                      unsigned short log_device_id, SafeDeviceType camera_device_type, unsigned short camera_device_id, unsigned short camera_id,
                                      bool do_holepunch, const char* holepunch_port, unsigned int fsm_id, unsigned int ret_param_id);
    
    /**
     * Metodo per richiedere di estrarre una nuova immagine relativa ad un processo di scaricamento immagini in corso
     * (se ve ne sono di presenti)
     *
     * @param sys_id                identificativo del Sistema di appartenenza
     * @param safeunit_id           identificativo della SafeUnit appartenente al sistema
     * @param len                   puntatore ad una cella di memoria di una variabile di tipo int in cui verrà scritta la
     *                              dimensione in numero di byte del frame restituito
     * @param ret_param_id          valore di ritorno corrispondente alla procedura di scaricamento in corso
     * @param err    puntatore alla variabile intera in cui la libreria riporta il valore di un evenutale errore
     *               nella ricerca del sistema, della vipunit o della fsm
     *                  -3 se la Macchina a stati non esiste
     *                  -4 se il Sistema non esiste
     *                  -6 se la SafeUnit non esiste
     *                   0 altrimenti
     *                -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     *
     * @return puntatore dell'immagine jpeg
     *         NULL in caso di errore o buffer vuoto
     */
    unsigned char* sysSafeLogGetImage(int sys_id, int safeunit_id, int* len, unsigned int* ret_param_id, int* err);
    
    // wrapper Live View handler =========================================================

    /**
     * Richiesta al gestore di live view delle telecamere analogiche del Pal-ip di effettuare una operazione di inizializzazione, start o stop visualizzazione video live
     *
     * @param sys_id                identificativo del Sistema di appartenenza
     * @param safeunit_id           identificativo della VipUnit appartenente al sistema
     * @param host_port             rete locale: porta CTP del dispositivo sorgente (SAFE_LIVE_VIEW_PLAY_STREAM)
     *                              rete remota: porta sul router di casa che esegue il forwarding verso il servizio del dispositivo sorgente (SAFE_LIVE_VIEW_PLAY_STREAM)
     * @param host_ip_address       rete locale: indirizzo IP del dispositivo sorgente (SAFE_LIVE_VIEW_PLAY_STREAM)
     *                              rete remota: indirizzo IP del router di casa (ottenuto dal DDNS) che esegue il forwarding verso il servizio del dispositivo sorgente (SAFE_LIVE_VIEW_PLAY_STREAM)
     * @param host_type             tipologia del dispositivo sorgente (SAFE_LIVE_VIEW_PLAY_STREAM / SAFE_LIVE_VIEW_STOP_STREAM)
     * @param host_id               identificativo del dispositivo sorgente (SAFE_LIVE_VIEW_PLAY_STREAM / SAFE_LIVE_VIEW_STOP_STREAM)
     * @param vipaddr               vip address interno (SAFE_LIVE_VIEW_CREATE_UNIT) o del destinatario (SAFE_LIVE_VIEW_PLAY_STREAM)
     * @param subaddr               sub address interno (SAFE_LIVE_VIEW_CREATE_UNIT) o del destinatario [Camera ID: 0-3 analogiche; 4-7 ip] (SAFE_LIVE_VIEW_PLAY_STREAM)
     * @param command_req           tipo di comando da richiedere al gestore media
     * @param do_holepunch          indica se è necessario inivare il pacchetto di holepunch, è necessario quando non si è in rete locale (SAFE_LIVE_VIEW_PLAY_STREAM)
     * @param holepunch_port        porta alla quale inviare il pacchetto di holepunch, va usata solo da remoto (SAFE_LIVE_VIEW_PLAY_STREAM)
     * @param call_id               id della chiamata a cui si riferisce il comando (SAFE_LIVE_VIEW_STOP_STREAM)
     * @param ret_param_id          parametro ritornato dalla callback im modo da poter discriminare sull'esito del comando ricevuto
     * @param camera_id             index of the camera that we wan
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *          -1 errore nei parametri immessi
     *           0 richiesta rifiutata
     *           1 richiesta presa in carico correttamente
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeLiveViewCommandReq(int sys_id, int safeunit_id, const char* host_port, const char* host_ip_address, SafeDeviceType host_type, unsigned char host_id,
                                  const char* vipaddr, const char* subaddr, SafeLiveViewReq command_req, bool do_holepunch, const char* holepunch_port,
                                  int call_id, unsigned int ret_param_id, int camera_id);
    
    // wrapper Logged Device / User handler =========================================================
    
    /**
     * Richiesta di effettuare l'operazione di autenticazione utenti/dispositivo nel sistema SAFE
     *
     * @param sys_id            identificativo del Sistema di appartenenza
     * @param safeunit_id       identificativo della VipUnit appartenente al sistema
     * @param user_device_pwd   password utente/dispositivo
     * @param login_level       livello di log in richiesto
     * @param is_rfid_key       segnala se il login è legato al passaggio di una chiavetta rfid
     * @param ret_param_id      (4byte) id del parametro che verrà ritornato alla callback come passato alla richiesta senza subire alcuna modifica
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *           0 richiesta rifiutata
     *           1 richiesta presa in carico correttamente
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeLogIn(int sys_id, int safeunit_id, const char* user_device_pwd, SafeUserLoginLevel login_level, bool is_rfid_key, unsigned int ret_param_id);

    
    /**
     * Richiesta di effettuare l'operazione di uscita dalla condizione di autenticazione utenti/dispositivo nel sistema SAFE
     *
     * @param sys_id                identificativo del Sistema di appartenenza
     * @param safeunit_id           identificativo della VipUnit appartenente al sistema
     * @param login_level           livello di login per cui richiedere logout
     * @param ret_param_id          id of the parameter that will be returned to the callback as passed to the request
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *           0 richiesta rifiutata
     *           1 richiesta presa in carico correttamente
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeLogOut(int sys_id, int safeunit_id, SafeUserLoginLevel login_level, unsigned int ret_param_id = 0);
    
    /**
     * Richiesta al gestore sessioni di log in delle informazioni legate all'utente correntemente loggato
     *
     * @param sys_id                identificativo del Sistema di appartenenza
     * @param safeunit_id           identificativo della VipUnit appartenente al sistema
     * @param ret_param_id          id of the parameter that will be returned to the callback as passed to the request
     *
     * @return  -4 se il Sistema non esiste
     *          -6 se la SafeUnit non esiste
     *           0 richiesta rifiutata
     *           1 richiesta presa in carico correttamente (utente loggato)
     *        -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     */
    int sysSafeCurrentUserReqInfo(int sys_id, int safeunit_id, unsigned int ret_param_id = 0);
    
    /**
     * Parser of the "data" field for push notification
     *
     * @param buf           data buffer (bytes) of received notification
     * @param json          JSON message corresponding at passed notification
     * @param deviceFamily  device family of the control panel that sent the notification
     */
    void sysSafeParsePushNotification(unsigned char* buf, char* json, SafeDeviceFamily deviceFamily);
    
    /**
     * Perform the request to register a given user email.
     * This is used during the Cloud registration with the control panel.
     *
     * @param sys_id                system identifier
     * @param safeunit_id           safe system identifier related to the system
     * @param user_email            user email to register
     */
    int sysSafeCloudSendEmail(int sys_id, int safeunit_id, const char* user_email);

    /**
     * Request pin for App.
     *
     * @param sys_id                system identifier
     * @param safeunit_id           safe system identifier related to the system
     * @param id_app                index of the requested App
     * @param role                  request pin for this role
     * @param ret_param_id          id of the parameter that will be returned to the callback as passed to the request
     *
     * @return  >0 if request is sent otherwise 0.
     *
     */
    int sysSafePinRequest(int sys_id, int safeunit_id, int id_app, SafeAccountAssociationRole role, unsigned int ret_param_id);

    /**
     * Request device association state list.
     *
     * @param sys_id                system identifier
     * @param safeunit_id           safe system identifier related to the system
     *
     * @return  >0 if request is sent otherwise 0.
     *
     */
    int sysSafeDevStateList(int sys_id, int safeunit_id);

    /**
     * Disassociate app.
     *
     * @param sys_id                system identifier
     * @param safeunit_id           safe system identifier related to the system
     * @param id_app                index of the requested App
     * @param role                  request pin for this role
     * @param ret_param_id          id of the parameter that will be returned to the callback as passed to the request
     *
     * @return  >0 if request is sent otherwise 0.
     *
     */
    int sysSafeDisassociateApp(int sys_id, int safeunit_id,int id_app, SafeAccountAssociationRole role, unsigned int ret_param_id);

    /**
     * @brief sysSafeVideoTimelinesDaysCommandReq
     * Sends a command to retrieve the days with at least one recorded video,
     * for the given camera, within the provided time range.
     *
     * @param sys_id            id of the system
     * @param safeunit_id       id of the safe unit in the provided system
     * @param camera_id         id of the camera for which the video time intervals are requested
     * @param start_time        start timestamp of the search time range
     * @param stop_time         stop timestamp of the search time range
     * @param ret_param_id      id associated with this command, will be returned unmodified in the json response for this command
     *
     * @return  -1 invalid parameters
     *          -4 if the system doesn't exists
     *          -6 if the safe unit doesn't exists
     *           0 if the request is refused
     *           1 if the request is accepted
     *        -102 functionality not supported by the current library compilation (missing SAFE_SYSTEM flag)
     */
    int sysSafeVideoTimelinesDaysCommandReq(int sys_id, int safeunit_id, unsigned short camera_id, time_t start_time, time_t stop_time, unsigned int ret_param_id);

    /**
     * @brief sysSafeVideoTimelinesTimeIntervalsCommandReq
     * Sends a command to retrieve the detailed video time intervals, for the given camera, within the provided
     * time range.
     * It allows also to apply filters on agent and media type.
     *
     * @param sys_id            id of the system
     * @param safeunit_id       id of the safe unit in the provided system
     * @param camera_id         id of the camera for which the video time intervals are requested
     * @param filter_list_type  filter the resultis on media list type
     * @param filter_agent_type filter the results on agent type
     * @param filter_agent_id   filter the results on agent id
     * @param start_time        start timestamp of the search time range
     * @param stop_time         stop timestamp of the search time range
     * @param ret_param_id      id associated with this command, will be returned unmodified in the json response for this command
     *
     * @return  -1 invalid parameters
     *          -4 if the system doesn't exists
     *          -6 if the safe unit doesn't exists
     *           0 if the request is refused
     *           1 if the request is accepted
     *        -102 functionality not supported by the current library compilation (missing SAFE_SYSTEM flag)
     */
    int sysSafeVideoTimelinesTimeIntervalsCommandReq(int sys_id, int safeunit_id, unsigned short camera_id, SafeMediaListType filter_list_type, SafeDeviceType filter_agent_type, unsigned short filter_agent_id, time_t start_time, time_t stop_time, unsigned int ret_param_id);

    /**
     * Send the info about HomeServer status to contrl panel.
     *
     * @param sys_id            system identifier
     * @param safeunit_id       safe system identifier related to the system
     * @param data              keepalive data
     * @param size              Dimension of keepalive data
     * @param ret_param_id      (4byte) id del parametro che verrà ritornato alla callback come passato alla richiesta senza subire alcuna modifica
     *
     * @return  -1 the data are not valid
     *          -4 if the system doesn't exist
     *          -6 if the SafeUnit doesn't exist
     *           0 if the request is refused
     *           1 if the request is accepted
     *        -102 if the feature isn't supported from current library compilation (lost flag SAFE_SYSTEM)
     */
    int sysSafeKeepalive(int sys_id, int safeunit_id, void *data, unsigned int size, unsigned int ret_param_id);
    
    /**
	 * Creazione di un elemento del mondo Tvcc "TvccUnit" all'interno di un dato Sistema
	 *
	 *
	 * @param sys_id	- identificativo del Sistema a cui associare l'elemento Tvcc
	 * @param model		- Sigla che identifica la famiglia di prodotto del dispositivo ip
	 * @param name		- nome da dare al dispositivo ip
	 * @param ip		- ip sul quale contattare il dispositivo
	 * @param http_port - porta sulla quale effettuare le richieste
	 * @param rtsp_port	- porta streaming rtsp
	 * @param devid		- id del dispositivo
	 * @param username	- username da fornire al dispositivo per autenticarsi
	 * @param password	- password da fornire al dispositivo per autenticarsi
	 *
	 *
	 * @return  id univoco della TvccUnit creata
	 *          -1 non è stato possibile creare una nuova TvccUnit
	 *          -4 se il Sistema a cui si vuole associare l'elemento non esiste
	 */
	int sysCreateTvccUnit(int sys_id, TvccFamilyModel model, string name, string ip,
						  int http_port, int rtsp_port, string devid, string username, string password);
	/**
	 * Rimozione di un elemento del mondo Tvcc "TvccUnit" all'interno di un dato Sistema.
	 *
	 *
	 * @param sys_id		- identificativo del Sistema a cui è associata la TvccUnit da Rimuovere
	 * @param tvccunit_id	- identificativo della TvccUnit da rimuovere
	 *
	 * @return   0 se la rimozione è andata a buon fine
	 *          -4 se il Sistema indicato non esiste
	 *          -6 se la TvccUnit che si vuole eliminare non esiste nel sistema indicato
	 */
	int sysRemoveTvccUnit(int sys_id, int tvccunit_id);
	/**
	 * @brief sysTvccGetDevice	- fornisce i parametri fondamentali di un dispositivo ip
	 * @param sys_id			- identificativo del Sistema in cui è presente l'elemento Tvcc desiderato
	 * @param tvccunit_id		- identificativo della TvccUnit
	 * @return   0 se la richiesta è andata a buon fine
	 *          -4 se il Sistema indicato non esiste
	 *          -6 se la TvccUnit che si vuole eliminare non esiste nel sistema indicato
	 */
	int sysTvccGetDevice(int sys_id, int tvccunit_id);
	/**
	 * @brief sysTvccGetChannel	- fornisce i parametri fondamentali del canale associato ad un dispositivo ip
	 * @param sys_id			- identificativo del Sistema in cui è presente l'elemento Tvcc desiderato
	 * @param tvccunit_id		- identificativo della TvccUnit
	 * @param ch				- canale desiderato
	 * @return   0 se la richiesta è andata a buon fine
	 *          -4 se il Sistema indicato non esiste
	 *          -6 se la TvccUnit che si vuole eliminare non esiste nel sistema indicato
	 */
	int sysTvccGetChannel(int sys_id, int tvccunit_id, int ch);
	/**
	 * @brief sysTvccGetChannelList - fornisce la lista dei canali (ed i rispettivi parametri) di un dispositivo ip
	 * @param sys_id				- identificativo del Sistema in cui è presente l'elemento Tvcc desiderato
	 * @param tvccunit_id			- identificativo della TvccUnit
	 * @return   0 se la richiesta è andata a buon fine
	 *          -4 se il Sistema indicato non esiste
	 *          -6 se la TvccUnit che si vuole eliminare non esiste nel sistema indicato
	 */
	int sysTvccGetChannelList(int sys_id, int tvccunit_id);
	/**
	 * @brief sysTvccGetStream		- fornisce i parametri fondamentali di uno stream
	 * @param sys_id				- identificativo del Sistema in cui è presente l'elemento Tvcc desiderato
	 * @param tvccunit_id			- identificativo della TvccUnit
	 * @param ch					- canale in cui risiede lo stream desiderato
	 * @param st					- stream di cui si vogliono sapere i parametri
	 * @return   0 se la richiesta è andata a buon fine
	 *          -4 se il Sistema indicato non esiste
	 *          -6 se la TvccUnit che si vuole eliminare non esiste nel sistema indicato
	 */
	int sysTvccGetStream(int sys_id, int tvccunit_id, int ch, int st);
	/**
	 * @brief sysTvccGetStreamList	- fornisce la lista degli stream (ed i rispettivi parametri) associati ad un canale
	 * @param sys_id				- identificativo del Sistema in cui è presente l'elemento Tvcc desiderato
	 * @param tvccunit_id			- identificativo della TvccUnit
	 * @param ch					- canale di cui si vuole sapere la lista degli stream
	 * @return   0 se la richiesta è andata a buon fine
	 *          -4 se il Sistema indicato non esiste
	 *          -6 se la TvccUnit che si vuole eliminare non esiste nel sistema indicato
	 */
	int sysTvccGetStreamList(int sys_id, int tvccunit_id, int ch);
	/**
	 * @brief sysTvccSetStreamResolution - imposta la risoluzione di uno stream
	 * @param sys_id					 - identificativo del Sistema in cui è presente l'elemento Tvcc desiderato
	 * @param tvccunit_id				 - identificativo della TvccUnit
	 * @param ch						 - canale in cui risiede lo stream di cui si vuol modificare la risoluzion
	 * @param st						 - stream di cui si vuole modificare la risoluzione
	 * @param res						 - risoluzione che si vuole impostare
	 * @return   0 se la richiesta è andata a buon fine
	 *          -4 se il Sistema indicato non esiste
	 *          -6 se la TvccUnit che si vuole eliminare non esiste nel sistema indicato
	 */
	int sysTvccSetStreamResolution(int sys_id, int tvccunit_id, int ch, int st, TvccResolution res);
	/**
	 * @brief sysTvccSetStreamSource	 - imposta le sorgenti di uno stream (solo video, video+audio ecc..)
	 * @param sys_id					 - identificativo del Sistema in cui è presente l'elemento Tvcc desiderato
	 * @param tvccunit_id				 - identificativo della TvccUnit
	 * @param ch						 - canale in cui risiede lo stream di cui si vuol modificare il parametro
	 * @param st						 - stream di cui si vuole modificare il parametro
	 * @param src						 - sorgente che si vuole impostare
	 * @return   0 se la richiesta è andata a buon fine
	 *          -4 se il Sistema indicato non esiste
	 *          -6 se la TvccUnit che si vuole eliminare non esiste nel sistema indicato
	 */
	int sysTvccSetStreamSource(int sys_id, int tvccunit_id, int ch, int st, TvccSourceType src);
	/**
	 * @brief sysTvccSetStreamBitRateType - imposta il tipo di bitrate dello stream (variabile o costante)
	 * @param sys_id					  - identificativo del Sistema in cui è presente l'elemento Tvcc desiderato
	 * @param tvccunit_id				  - identificativo della TvccUnit
	 * @param ch						  - canale in cui risiede lo stream di cui si vuol modificare il parametro
	 * @param st						  - stream di cui si vuole modificare il parametro
	 * @param brt						  - bitarte type che si vuole impostare
	 * @return   0 se la richiesta è andata a buon fine
	 *          -4 se il Sistema indicato non esiste
	 *          -6 se la TvccUnit che si vuole eliminare non esiste nel sistema indicato
	 */
	int sysTvccSetStreamBitRateType(int sys_id, int tvccunit_id, int ch, int st, TvccBitRateType brt);
	/**
	 * @brief sysTvccSetStreamBitRate	 - imposta il bitrate dello stream
	 * @param sys_id					 - identificativo del Sistema in cui è presente l'elemento Tvcc desiderato
	 * @param tvccunit_id				 - identificativo della TvccUnit
	 * @param ch						 - canale in cui risiede lo stream di cui si vuol modificare il parametro
	 * @param st						 - stream di cui si vuole modificare il parametro
	 * @param br						 - bitrate che si vuole impostare
	 * @return   0 se la richiesta è andata a buon fine
	 *          -4 se il Sistema indicato non esiste
	 *          -6 se la TvccUnit che si vuole eliminare non esiste nel sistema indicato
	 */
	int sysTvccSetStreamBitRate(int sys_id, int tvccunit_id, int ch, int st, TvccBitRate br);
	/**
	 * @brief sysTvccSetStreamFrameRate	 - imposta il framerate dello stream
	 * @param sys_id					 - identificativo del Sistema in cui è presente l'elemento Tvcc desiderato
	 * @param tvccunit_id				 - identificativo della TvccUnit
	 * @param ch						 - canale in cui risiede lo stream di cui si vuol modificare il parametro
	 * @param st						 - stream di cui si vuole modificare il parametro
	 * @param fr						 - framerate che si vuole impostare
	 * @return   0 se la richiesta è andata a buon fine
	 *          -4 se il Sistema indicato non esiste
	 *          -6 se la TvccUnit che si vuole eliminare non esiste nel sistema indicato
	 */
	int sysTvccSetStreamFrameRate(int sys_id, int tvccunit_id, int ch, int st, TvccFrameRate fr);
	/**
	 * @brief sysTvccSetStreamQuality	 - imposta la qualità dello stream
	 * @param sys_id					 - identificativo del Sistema in cui è presente l'elemento Tvcc desiderato
	 * @param tvccunit_id				 - identificativo della TvccUnit
	 * @param ch						 - canale in cui risiede lo stream di cui si vuol modificare il parametro
	 * @param st						 - stream di cui si vuole modificare il parametro
	 * @param q							 - qualità che si vuole impostare
	 * @return   0 se la richiesta è andata a buon fine
	 *          -4 se il Sistema indicato non esiste
	 *          -6 se la TvccUnit che si vuole eliminare non esiste nel sistema indicato
	 */
	int sysTvccSetStreamQuality(int sys_id, int tvccunit_id, int ch, int st, TvccQuality q);
	/**
	 * @brief sysTvccSetStreamEncoder	 - imposta il codec dello stream (H264, MJPEG ecc..)
	 * @param sys_id					 - identificativo del Sistema in cui è presente l'elemento Tvcc desiderato
	 * @param tvccunit_id				 - identificativo della TvccUnit
	 * @param ch						 - canale in cui risiede lo stream di cui si vuol modificare il parametro
	 * @param st						 - stream di cui si vuole modificare il parametro
	 * @param enc						 - codec che si vuole impostare
	 * @return   0 se la richiesta è andata a buon fine
	 *          -4 se il Sistema indicato non esiste
	 *          -6 se la TvccUnit che si vuole eliminare non esiste nel sistema indicato
	 */
	int sysTvccSetStreamEncoder(int sys_id, int tvccunit_id, int ch, int st, TvccCodecType enc);
	/**
	 * @brief sysTvccSetStreamProfile	 - imposta il profilo del codec di uno stream
	 * @param sys_id					 - identificativo del Sistema in cui è presente l'elemento Tvcc desiderato
	 * @param tvccunit_id				 - identificativo della TvccUnit
	 * @param ch						 - canale in cui risiede lo stream di cui si vuol modificare il parametro
	 * @param st						 - stream di cui si vuole modificare il parametro
	 * @param pr						 - profilo da impostare
	 * @return   0 se la richiesta è andata a buon fine
	 *          -4 se il Sistema indicato non esiste
	 *          -6 se la TvccUnit che si vuole eliminare non esiste nel sistema indicato
	 */
	int sysTvccSetStreamProfile(int sys_id, int tvccunit_id, int ch, int st, TvccProfileStream pr);
	/**
	 * @brief sysTvccSetStreamIFrame	 - imposta l'intervallo tra un keyframe e l'altro
	 * @param sys_id					 - identificativo del Sistema in cui è presente l'elemento Tvcc desiderato
	 * @param tvccunit_id				 - identificativo della TvccUnit
	 * @param ch						 - canale in cui risiede lo stream di cui si vuol modificare il parametro
	 * @param st						 - stream di cui si vuole modificare il parametro
	 * @param ifr						 - iframe da impostare
	 * @return   0 se la richiesta è andata a buon fine
	 *          -4 se il Sistema indicato non esiste
	 *          -6 se la TvccUnit che si vuole eliminare non esiste nel sistema indicato
	 */
	int sysTvccSetStreamIFrame(int sys_id, int tvccunit_id, int ch, int st, TvccIFrame ifr);
	/**
	 * @brief sysTvccSetChannelBrightness	 - aumenta/riduce la luminosità di un canale
	 * @param sys_id						 - identificativo del Sistema in cui è presente l'elemento Tvcc desiderato
	 * @param tvccunit_id					 - identificativo della TvccUnit
	 * @param ch							 - canale di cui si vuol modificare il parametro
	 * @param b								 - 0 - riduci luminosità, 1 - aumenta luminosità
	 * @return   0 se la richiesta è andata a buon fine
	 *          -4 se il Sistema indicato non esiste
	 *          -6 se la TvccUnit che si vuole eliminare non esiste nel sistema indicato
	 */
	int sysTvccSetChannelBrightness(int sys_id, int tvccunit_id, int ch, int b);
	/**
	 * @brief sysTvccSetChannelContrast		 - aumenta/riduce il contrasto di un canale
	 * @param sys_id						 - identificativo del Sistema in cui è presente l'elemento Tvcc desiderato
	 * @param tvccunit_id					 - identificativo della TvccUnit
	 * @param ch							 - canale di cui si vuol modificare il parametro
	 * @param c								 - 0 - riduci contrasto, 1 - aumenta contrasto
	 * @return   0 se la richiesta è andata a buon fine
	 *          -4 se il Sistema indicato non esiste
	 *          -6 se la TvccUnit che si vuole eliminare non esiste nel sistema indicato
	 */
	int sysTvccSetChannelContrast(int sys_id, int tvccunit_id, int ch, int c);
	/**
	 * @brief sysTvccSetChannelSaturation	 - aumenta/riduce il parametro di saturazione di un canale
	 * @param sys_id						 - identificativo del Sistema in cui è presente l'elemento Tvcc desiderato
	 * @param tvccunit_id					 - identificativo della TvccUnit
	 * @param ch							 - canale di cui si vuol modificare il parametro
	 * @param s								 - 0 - riduci saturazione, 1 - aumenta saturazione
	 * @return   0 se la richiesta è andata a buon fine
	 *          -4 se il Sistema indicato non esiste
	 *          -6 se la TvccUnit che si vuole eliminare non esiste nel sistema indicato
	 */
	int sysTvccSetChannelSaturation(int sys_id, int tvccunit_id, int ch, int s);
	/**
	 * @brief sysTvccSetChannelHue			 - aumenta/riduce il parametro hue di un canale
	 * @param sys_id						 - identificativo del Sistema in cui è presente l'elemento Tvcc desiderato
	 * @param tvccunit_id					 - identificativo della TvccUnit
	 * @param ch							 - canale di cui si vuol modificare il parametro
	 * @param h								 - 0 - riduci hue, 1 - aumenta hue
	 * @return   0 se la richiesta è andata a buon fine
	 *          -4 se il Sistema indicato non esiste
	 *          -6 se la TvccUnit che si vuole eliminare non esiste nel sistema indicato
	 */
	int sysTvccSetChannelHue(int sys_id, int tvccunit_id, int ch, int h);
	/**
	 * @brief sysTvccChangePTZSpeed			 - cambia la velocità di movimentazione
	 * @param sys_id						 - identificativo del Sistema in cui è presente l'elemento Tvcc desiderato
	 * @param tvccunit_id					 - identificativo della TvccUnit
	 * @param ch							 - canale da movimentare
	 * @param cmd							 - 0 - riduci velocità, 1 - aumenta velocità
	 * @return   0 se la richiesta è andata a buon fine
	 *          -4 se il Sistema indicato non esiste
	 *          -6 se la TvccUnit che si vuole eliminare non esiste nel sistema indicato
	 */
	int sysTvccChangePTZSpeed(int sys_id, int tvccunit_id, int ch, int cmd);
	/**
	 * @brief sysTvccMovePTZStart			 - invia comandi di movimentazione al dispositivo ip
	 * @param sys_id						 - identificativo del Sistema in cui è presente l'elemento Tvcc desiderato
	 * @param tvccunit_id					 - identificativo della TvccUnit
	 * @param ch							 - canale da movimentare
	 * @param cmd							 - comando da inviare (es. zoom in, zoom out, left, right ecc..)
	 * @return   0 se la richiesta è andata a buon fine
	 *          -4 se il Sistema indicato non esiste
	 *          -6 se la TvccUnit che si vuole eliminare non esiste nel sistema indicato
	 */
	int sysTvccMovePTZStart(int sys_id, int tvccunit_id, int ch, TvccPtzCommand cmd);
	/**
	 * @brief sysTvccMovePTZStop			 - invia comando di stop di movimentazione
	 * @param sys_id						 - identificativo del Sistema in cui è presente l'elemento Tvcc desiderato
	 * @param tvccunit_id					 - identificativo della TvccUnit
	 * @param ch							 - canale da fermare
	 * @return   0 se la richiesta è andata a buon fine
	 *          -4 se il Sistema indicato non esiste
	 *          -6 se la TvccUnit che si vuole eliminare non esiste nel sistema indicato
	 */
	int sysTvccMovePTZStop(int sys_id, int tvccunit_id, int ch);
	/**
	 * @brief sysTvccAddPresetPointPTZ		 - aggiunge un preset point (registra la posizione corrente del ptz)
	 * @param sys_id						 - identificativo del Sistema in cui è presente l'elemento Tvcc desiderato
	 * @param tvccunit_id					 - identificativo della TvccUnit
	 * @param ch							 - canale dove aggiungere il preset point
	 * @param pp							 - slot preset point dove registrare la posizione ptz
	 * @return   0 se la richiesta è andata a buon fine
	 *          -4 se il Sistema indicato non esiste
	 *          -6 se la TvccUnit che si vuole eliminare non esiste nel sistema indicato
	 */
	int sysTvccAddPresetPointPTZ(int sys_id, int tvccunit_id, int ch, int pp);
	/**
	 * @brief sysTvccGotoPresetPointPTZ		 - richiama una posizione ptz registrata nel preset point passato
	 * @param sys_id						 - identificativo del Sistema in cui è presente l'elemento Tvcc desiderato
	 * @param tvccunit_id					 - identificativo della TvccUnit
	 * @param ch							 - canale dove risiede il preset point da richiamare
	 * @param pp							 - preset point da caricare
	 * @return   0 se la richiesta è andata a buon fine
	 *          -4 se il Sistema indicato non esiste
	 *          -6 se la TvccUnit che si vuole eliminare non esiste nel sistema indicato
	 */
	int sysTvccGotoPresetPointPTZ(int sys_id, int tvccunit_id, int ch, int pp);
	/**
	 * @brief sysTvccGetCruisePointInfoPTZ	 - ottiene le informazioni su un cruise point
	 * @param sys_id						 - identificativo del Sistema in cui è presente l'elemento Tvcc desiderato
	 * @param tvccunit_id					 - identificativo della TvccUnit
	 * @param ch							 - canale dove risiede il cruise point da richiamare
	 * @param cp							 - cruise point da caricare
	 * @return   0 se la richiesta è andata a buon fine
	 *          -4 se il Sistema indicato non esiste
	 *          -6 se la TvccUnit che si vuole eliminare non esiste nel sistema indicato
	 */
	int sysTvccGetCruisePointInfoPTZ(int sys_id, int tvccunit_id, int ch, int cp);
	/**
	 * @brief sysTvccSearchRecordByDay	 - cerca registrazioni video per data e tipo di attivazione (Manuale, allarme, motion ecc..)
	 * @param sys_id					 - identificativo del Sistema in cui è presente l'elemento Tvcc desiderato
	 * @param tvccunit_id				 - identificativo della TvccUnit
	 * @param channels					 - bitmask per identificare i canali dove effettuare la ricerca
	 * @param yyyy						 - data in cui si vuole effettuare la ricerca (anno)
	 * @param mm						 - data in cui si vuole effettuare la ricerca (mese)
	 * @param dd						 - data in cui si vuole effettuare la ricerca (giorno)
	 * @param start_H					 - tempo iniziale di ricerca (ore)
	 * @param start_M					 - tempo iniziale di ricerca (minuti)
	 * @param start_S					 - tempo iniziale di ricerca (secondi)
	 * @param stop_H					 - tempo finale di ricerca (ore)
	 * @param stop_M					 - tempo finale di ricerca (minuti)
	 * @param stop_S					 - tempo finale di ricerca (secondi)
	 * @param types						 - bitmask per filtro tipo di attivazione registrazione
	 * @return   0 se la richiesta è andata a buon fine
	 *          -4 se il Sistema indicato non esiste
	 *          -6 se la TvccUnit che si vuole eliminare non esiste nel sistema indicato
	 */
	int sysTvccSearchRecordByDay(int sys_id, int tvccunit_id, int channels, int yyyy, int mm, int dd,
															  int start_H, int start_M, int start_S,
															  int stop_H, int stop_M, int stop_S,
															  int types);
	/**
	 * @brief sysTvccStartLiveStream	 - start live stream like a vip call
	 * @param sys_id					 - system id of the tvcc system call
	 * @param tvccunit_id				 - tvcc system id
	 * @param channel					 - device channel
	 * @param stream					 - device stream
	 * @return   0 if requested is ok
	 *			-4 system does not exists
	 *			-6 tvcc system does not exists in the system specified
	 */
	int sysTvccStartLiveStream(int sys_id, int tvccunit_id, int channel, int stream);
	/**
	 * @brief sysTvccStopLiveStream		 - stop live stream started with sysTvccStartLiveStream method
	 * @param sys_id					 - system id where the live stream is started
	 * @param tvccunit_id				 - tvcc system id where the live stream started
	 * @param channel					 - device channel
	 * @param stream					 - device stream
	 * @param call_id					 - id of the call to stop
	 * @return   0 if requested is ok
	 *			-4 system does not exists
	 *			-6 tvcc system does not exists in the system specified
	 */
	int sysTvccStopLiveStream(int sys_id, int tvccunit_id, int channel, int stream, int call_id);
	/**
	 * @brief sysTvccStartPlayback		 - start a play back stream like a vip call
	 * @param sys_id					 - system id where the record lie
	 * @param tvccunit_id				 - tvcc system id where the record lie
	 * @param channel					 - device channel
	 * @param type						 - type of the record (recorded manually, sensor, time etc..)
	 * @param s_yyyy					 - start year
	 * @param s_mm						 - start month (1 - 12)
	 * @param s_dd						 - start day (1 -31)
	 * @param s_H						 - start hours (0 - 24)
	 * @param s_M						 - start minutes (0 - 60)
	 * @param s_S						 - start seconds (0 - 60)
	 * @param e_yyyy					 - stop year
	 * @param e_mm						 - stop month (1 - 12)
	 * @param e_dd						 - stop day (1 -31)
	 * @param e_H						 - stop hours (0 - 24
	 * @param e_M						 - stop minutes (0 - 60)
	 * @param e_S						 - stop seconds (0 - 60)
	 * @return   0 if requested is ok
	 *			-4 system does not exists
	 *			-6 tvcc system does not exists in the system specified
	 */
	int sysTvccStartPlayback(int sys_id, int tvccunit_id, int channel, int type, int s_yyyy, int s_mm, int s_dd, int s_H, int s_M, int s_S,
																				 int e_yyyy, int e_mm, int e_dd, int e_H, int e_M, int e_S);
	/**
	 * @brief sysTvccStopPlayback		 - stop a play back record started with sysTvccStartPlayback method
	 * @param sys_id					 - system id where the record to stop lie
	 * @param tvccunit_id				 - tvcc system id the record started lie
	 * @param channel					 - device channel
	 * @param type						 - type of the record (recorded manually, sensor, time etc..)
	 * @param call_id					 - id of the call to stop
	 * @return   0 if requested is ok
	 *			-4 system does not exists
	 *			-6 tvcc system does not exists in the system specified
	 */
	int sysTvccStopPlayback(int sys_id, int tvccunit_id, int channel, int type, int call_id);

	/**
	 * Creazione di un elemento di un elemento Rtsp dato un sistema
	 *
	 *
	 * @param sys_id	- identificativo del Sistema a cui associare l'elemento Tvcc
     * @param flags     - @see UnitFlags
	 *
	 *
	 * @return  id univoco della RtspUnit creata
	 *          -1 non è stato possibile creare una nuova RtspUnit
	 *          -4 se il Sistema a cui si vuole associare l'elemento non esiste
	 */
    int sysCreateRtspUnit(int sys_id, unsigned int flags);

	/**
	 * Rimozione di un elemento Rtsp all'interno di un dato Sistema.
	 *
	 *
	 * @param sys_id		- identificativo del Sistema a cui è associata la TvccUnit da Rimuovere
	 * @param rtspunit_id	- identificativo della RtspUnit da rimuovere
	 *
	 * @return   0 se la rimozione è andata a buon fine
	 *          -4 se il Sistema indicato non esiste
	 *          -6 se la RtspUnit che si vuole eliminare non esiste nel sistema indicato
	 */
	int sysRemoveRtspUnit(int sys_id, int rtspunit_id);

	/**
     * Set rtsp unit call times
     *
     * @param sys_id             - System's Id
     * @param rtspunit_id        - RtspUnit's Id
     * @param param              - Call time to set @ref CallTimeParam
     * @param value              - time to set [seconds]
     *
     * @return  -4 System doesn't exist
     *          -6 RtspUnit doesn't exist
     *          -1 unable to set call time
     *           0 success
     *        -101 function not supported with current compilation flags (RTSP_SYSTEM flag not set)
     */
    int sysRtspSetCallTimeParam(int sys_id, int rtspunit_id, CallTimeParam param, int value);

	/**
	 * @brief sysRtspStartCall	- start an rtsp call
	 * @param sys_id			- system id
	 * @param rtspunit_id		- rtsp unit id
	 * @param rtsp_url			- rtsp url to start the rtps communication
	 * @param user				- rtsp username
	 * @param pw				- rtsp password
	 * @return  >0 id della rtsp fsm appena creata
	 *          -4 se il Sistema indicato non esiste
	 *          -6 se la RtspUnit che si vuole far partire non esiste nel sistema indicato
	 */
	int sysRtspStartCall(int sys_id, int rtspunit_id, char* rtsp_url, char* user, char* pw);

	/**
	 * @brief sysRtspStopCall	- stop a specific rtsp call
	 * @param sys_id			- system id
	 * @param rtspunit_id		- rtsp unit id
	 * @param rtspcall_id		- rtsp call id
	 * @return   0 se la stop è andata a buon fine
	 *          -4 se il Sistema indicato non esiste
	 *          -6 se la RtspUnit o la RtspCall che si vuole fermare non esiste nel sistema indicato
	 */
	int sysRtspStopCall(int sys_id, int rtspunit_id, int rtspcall_id);

    /**
     * @brief sysRtspStopCall	- release a specific rtsp call after a close time setted
     * @param sys_id			- system id
     * @param rtspunit_id		- rtsp unit id
     * @param rtspcall_id		- rtsp call id
     * @return   0 if ok
     *          -4 if the system not exist
     *          -6 if the RtspUnit or the RtspCall not exist in the specific sysetm
     */
    int sysRtspReleaseCall(int sys_id, int rtspunit_id, int rtspcall_id);

	/**
	 * @brief sysRtspStopUnit	- stop a specific rtsp unit and all active rtsp calls
	 * @param sys_id			- system id
	 * @param rtspunit_id		- rtsp unit id
	 * @return   0 se la stop è andata a buon fine
	 *          -4 se il Sistema indicato non esiste
	 *          -6 se la RtspUnit che si vuole eliminare non esiste nel sistema indicato
	 */
    int sysRtspStopUnit(int sys_id, int rtspunit_id);


    /**
     * @brief sysLogStartSession - start a new log session
     * @param sys_id             - system id
     * @param tag                - log tag
     * @return  0 on success
     */
    int sysLogStartSession(int sys_id, char* tag);

    /**
     * @brief sysLogAddEvent     - add event to the running log session
     * @param sys_id             - system id
     * @param eventName          - event name
     * @return 0 on success
     */
    int sysLogAddEvent(int sys_id, char* eventName);

    /**
     * @brief sysLogAddElement   - add a generic element to the running log session
     * @param sys_id             - system id
     * @param key                - element name
     * @param value              - element value
     * @return 0 on success
     */
    int sysLogAddElement(int sys_id, char* key, char* value);

    /**
     * @brief sysLogStopSession  - stop a running log session
     * @param sys_id             - system id
     * @param[in,out] buf        - optional buffer to store log result allocated by user
     * @param[in] len            - buffer len (if any)
     * @return 0 on success
     */
    int sysLogStopSession(int sys_id, char* buf = nullptr, int bufLen = 0);

    /**
     * Method to create a playback for open and read an avi file
     * @note call this method before call @see playbackGetVideoFrame
     * @param fileName          -   name of the avi file to be open
     * @param fileInfo          -   structure that contain some parameters of the video in the avi file (@see AviInfo)
     * @param err               -   store a code representing if the method has been executed successfully(0) or not(-1)
     * @param audioOnly         -   if true open avi file to read audio stream only
     * @return        id of the object that handle the recording
     *                -1 if there was an error while opening the file
     *              -100 if the functionality is not supported with the current compilation flags
     */
    int createPlayer(const char *fileName, AviInfo *fileInfo, int* err, bool audioOnly = false);

    /**
     * Method to destroy a playback used for read an avi file
     *
     * @brief playbackStopRead @see destroyPlayback
     * @param readId - playback read id
     * @param err    - store a code representing if the method has been executed successfully
     * @return          0 if correctly destroyed
     *                 -1 if the id not exist
     *                 -100 if the functionality is not supported with the current compilation flags
     */
     int destroyPlayer(int readId, int* err);

    /**
     * Metodo per richiedere di estrarre un campione Audio dal file avi
     *
     * @brief playbackGetAudioPck
     * @param readId    - Identificativo della macchina a stati di chiamata a cui fare la richiesta
     * @param len       - puntatore ad una cella di memoria di una variabile di tipo int in cui verrà scritta la
     *                    dimensione in numero di byte del campione restituito
     *                  -1 nel caso di errore
     * @param err       - puntatore alla variabile intera in cui la libreria riporta il valore di un evenutale errore
     *                  -3 se id è inesistente
     *                   0 altrimenti
     *                -101 se la funzionalità non è supportata dall'attuale compilazione della libreria
     * @return          puntatore del campione audio
     *                  NULL in caso di errore o buffer vuoto
     */
    short* playbackGetAudioPck(int readId, int* len, int* err);

    /**
     * @brief playbackGetVideoFrame  - retrieve a video frame from an avi file
     * @param readId                 - Identificativo della macchina a stati di chiamata a cui fare la richiesta
     * @param len                    - puntatore ad una cella di memoria di una variabile di tipo int in cui verrà scritta la
     *                                 dimensione in numero di byte del campione restituito
     *                                 -1 nel caso di errore
     * @param err                    - contains an error code in case of get frame fail
     *                                 -3 if id passed is not present
     *                                 -200 if player is not alive
     *                                    0 no error
     *                                 -101 if the functionality is not supported with the current compilation flags
     * @return                       - pointer to video frame or NULL if error occur or buffer is empty
     */
    unsigned char* playbackGetVideoFrame(int readId, int* len, int* err, frameInfo* fr_info);

    /**
     * Request thumbnail of selected avi file
     *
     * @brief playbackGetThumbnail
     * @param filename          avi file name
     * @param thumbnailWidth    larghezza thumbnail
     * @param thumbnailHeight   altezza thumbnail
     * @param isNewFile         TRUE if the file is not readed yet, FALSE otherwhise
     * @param err               error pointer
     *                          -3 if id is wrong
     *                          0 otherwhise
     *                         -101 function is not supported from compiled library
     * @param bitsDepth         image bitsDepth in case of RAW image
     * @param imageFormat       idenitfy type of image raw or compressed, view ERM_IMAGE_ from libembrecman
     * @param thumbnailSize     image size in case of compressed image
     * @return Thumbnail image ptr, NULL in case of errors - "image memmory must be free by the caller"
     */
    unsigned char* playbackGetThumbnail(const char* filename,
                                        int *thumbnailWidth,
                                        int *thumbnailHeight,
                                        bool *isNewFile,
                                        int *err,
                                        int *bitsDepth = nullptr,
                                        int *imageFormat = nullptr,
                                        int *thumbnailSize = nullptr);

    /**
     * Request thumbnail parameters of selected avi file
     *
     * @brief playbackGetThumbnailParameters
     * @param filename          avi file name
     * @param thumbnailWidth    larghezza thumbnail
     * @param thumbnailHeight   altezza thumbnail
     * @param isNewFile         TRUE if the file is not readed yet, FALSE otherwhise
     * @param err               error pointer
     *                          -3 if id is wrong
     *                          0 otherwhise
     *                         -101 function is not supported from compiled library
     * @param bitsDepth         image bitsDepth in case of RAW image
     * @param imageFormat       idenitfy type of image raw or compressed, view ERM_IMAGE_ from libembrecman
     * @param thumbnailSize     image size in case of compressed image
     */
    void playbackGetThumbnailParameters(const char *filename,
                                        int *thumbnailWidth,
                                        int *thumbnailHeight,
                                        bool *isNewFile,
                                        int *err,
                                        int *bitsDepth = nullptr,
                                        int *imageFormat = nullptr,
                                        int *thumbnailSize = nullptr);

    /**
     * Free data previous got out from a circular buffer
     * (using functions playbackGetAudioPck or playbackGetVideoFrame).
     * Using this function allow to check for memory leaks
     *
     * @param readId                 - Identificativo della macchina a stati di chiamata a cui fare la richiesta
     * @param err                    - contains an error code in case of get frame fail
     *                                 < 0 error
     *                                   0 no error
     *                                 -101 if the functionality is not supported with the current compilation flags
     * @param dataPtr   - pointer to free
     */
    void playbackFreeDataPtr(int readId, int* err, void* dataPtr);

    /**
     * Function to get info library status. Return on json socket (or callback) the status of current engine
     * systems, units, calls, etc.
     */
    void getLibraryStatus();

    // wrapper Keybord Emulation Handler =========================================================
          
    /**
     * Function that requests to enter the emulated menu
     *
     * @param sys_id        - System identifier to which the unit is associated
     * @param safeunit_id   - Identification of the SafeUnit belonging to the system
     * @param menu_type     - Type of menu required (EMUL_TAST_MENU_FULL_TYPE etc...)
     *
     * @return 0: success, otherwise negative error code
     */
    int sysSafeKeybEmulationEnterMenu(int sys_id, int safeunit_id, u8 menu_type);    

    /**
     * Function that requests to exit from keyboard emulation manager
     *
     * @param sys_id        - System identifier to which the unit is associated
     * @param safeunit_id   - Identification of the SafeUnit belonging to the system
     *
     * @return 0: success, otherwise negative error code
     */
    int sysSafeKeybEmulationExitMenu(int sys_id, int safeunit_id);    

    /**
     * Function that requests the remote device emulated keyboard handler status
     *
     * @param sys_id        - System identifier to which the unit is associated
     * @param safeunit_id   - Identification of the SafeUnit belonging to the system
     *
     * @return 0: success, otherwise negative error code
     */
	int sysSafeKeybEmulationReqStatus(int sys_id, int safeunit_id);

private:

    /*
     * @brief srandSeed - seed used to initialize the rand() PRNG used by libcomelit and its dependency.
     */
    static uint32_t srandSeed;

    /**
     * @brief runThread - Thread delle funzioni di run
     */
    void* runThread;

    /**
     * @brief timerThread - Thread relativo ai timer
     */
    void* timerThread;

    /**
     * @brief instance - Singola istanza della classe ComelitEngine
     */
    static ComelitEngine* instance;

    /**
     * @brief engineAlive - Variabile che identifica se l'engine è attiva
     */
    volatile bool engineAlive;

    /**
     * @brief pData - Puntatore alla struttura dei dati di ComelitEngine
     */
    void* pData;

    /**
     * @brief seq_sys_number - Numero sequenziale da assegnare come id ad un sistema
     */
    unsigned int seq_sys_number;

    /**
     * @brief ticks100ms clock ticks counter with 100 milliseconds resolution
     */
    std::atomic_uint ticks100ms;

    /**
     * Metodo per cercare il prossimo numero sequenziale da assegnare com id ad un nuovo sistema
     *
     * @return numero da assegnare come id al sistema
     */
    unsigned int getSysSequenceNumber();

    /**
     * Metodo per ricercare il puntatore all'itanza di Sistema tramite l'id
     *
     * @param sys_id - identificativo del sistema da ricercare
     *
     * @return puntatore all'istanza del Sistema con id uguale a sys_id
     */
    void* getSystemById(unsigned int sys_id);

    /**
     * Function to check if system exist, that return also error
     * @param sysId          - system id to search
     * @param error          - return error value
     * @param callerFunction - optional argument for debug print
     * @return pointer to system with id sysId
     */
    void* checkAndGetSystemById(unsigned int sysId, int* error, const char *callerFunction = NULL);

    /**
     * Function to check if unit exist and return it. Internally
     * @param sysId          - system id to search
     * @param unitId         - unit id to search
     * @param unitType       - type of unit to search
     * @param error          - return error value
     * @param callerFunction - optional argument for debug print
     * @return  pointer to unit with system with id sysId and unidId, or null if not exist
     */
    void* checkAndGetUnitById(int sysId, int unitId, gUnitType unitType, int* error, const char *callerFunction = NULL);

    /**
     * Metodo per ricercare il puntatore all'itanza di Embedder rec tramite l'id
     *
     * @brief getEmbeddedPlaybackById
     * @param id - embedded recorder id
     *
     * @return pointer to the embedded recorder with the give id or NULL if not found
     */
    void* getEmbeddedPlaybackById(unsigned int id);

    /**
     * @brief cfg - Struttura contenente alcuni dati legati alla vita dell'istanza di ComelitEngine
     */
    cfg_t cfg;

    /**
     * @brief execRun - Object with timer callback implementation
     */
    Comelit::Utils::PeriodicExecution execRun;

    /**
     * Costruttore della classe ComelitEngine
     */
    ComelitEngine();

    /**
      * Distruttore della Classe ComelitEngine
      */
    ~ComelitEngine();

    /**
     * Metodo che raggruppa un insieme di run da eseguire ogni tempo di cfg time [ms]
     */
    void run();

    /**
     *  Metodo che raggruppa un insieme di timer da eseguire ogni 100ms
     */
    void timer100ms();

    /**
     * Metodo per mettere in stop tutti i thread e i sistemi presenti nella Engine
     */
    void stopThreadsAndSystems();
    
    /**
     * Metodo che rimappa il sottotipo azione in formato SAFE, pronto per essere inviato al gestore azioni
     *
     * @param action_type tipo azione
     * @param action_sub_type sottotipo azione, dipende direttamente dal tipo azione
     *
     * @return sottotipo azione in formato SAFE
     *         -102 se la funzionalità non è supportata dall'attuale compilazione della libreria (manca flag SAFE_SYSTEM)
     *
     */
    int sysSafeGetSafeActionSubtype(SafeActionType action_type, unsigned int action_sub_type);

    /**
     * Create a new system with Gateway (uniquely identified in the library through an id).
     * A System is simply a container of one or more Comelit worlds belonging to a planr (Safe, ViP, ...)
     *
     * @attention To use this system mode the library must be initialized with useViperTunnel=true or setup systems connection using viper
     *
     * @attention To use this mode must be created a base system through ::createBaseSystem()
     *
     * @param sys_id        - Unique ID of the System created with ::createBaseSystem()
     * @param params        - parameters for gateway
     * @param autoStart     - start tunnel immediately
     * @param useRun        - true if viper run needed
     * @param fd            - return parameter fd (used only for mode without run)
     *
     * @return    0 Request successful
     *           -1 if the library is initialized with useViperTunnel=false
     *           -2 gateway not reachable
     *         -100 function not supported (missing VIPER_TUNNEL flag)
     */
    int pOpenViperTunnel(int sys_id, ViperConnParameters params, bool autoStart = true, bool useRun = true, int *fd = nullptr);

    /**
     * Create a new system with P2P connection (uniquely identified in the library with an id).
     * The status of the connection to the system will be notified asynchronously by an event, @see eViperConnection
     *
     * @attention To use this mode must be created a base system through ::createBaseSystem()
     *
     * @attention To use this system mode the library must be initialized with useViperTunnel=true or setup systems connection using viper
     *
     * @param sys_id        - Unique ID of the System created with ::createBaseSystem()
     * @param p2pInfo       - A structure contains all the various parameters for the p2p @see p2pParameters
     * @param autoStart     - start tunnel immediately
     *
     * @return  The File Descriptor of the P2P socket
     *           -1 if the library is initialized with useViperTunnel=false
     *           -2 gateway not reachable
     *          -100 if the library is not compiled with VIPER_TUNNEL and VIPER_P2P flags
     */
    int pOpenViperTunnelP2P(int sys_id, p2pParameters p2pInfo, bool autoStart = true);
    
    /**
     * Verify if the Safe device is supported by library
     *
     * @param  deviceType Device type that must be instancated (used during communication RemoteCom)
     *
     * @return true if the library supports it, otherwise false
     */
    bool isSafeDeviceSupported(SafeDeviceType deviceType);

    /**
     * Clean remaining systems and players.
     * The operation is performed on all objects if forced is set to true.
     * The operation is performed only on not alive objects if forced is set to false.
     * Additionally, the method invoke cspstack_run() to process any pending events.
     *
     * @param forced perform the clean up operations on all Engine's objects or on those not alive
     *        true delete all systems, all players
     *        false delete systems not alive, delete players not alive
     */
    void cleanUpEngine(bool forced = false);

#ifdef ENABLE_VM_SERVER
    /**
     * Check if vm server is active
     * @return true if the server is active
     * @return false if the server is not active
     */
    bool isVmServerActive();
#endif

};

#endif // COMELITENGINE_H
