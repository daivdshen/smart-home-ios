/**
 *
 * @file csp.h
 *
 * Comelit Signalling Protocol
 * - Encoding and Decoding
 * - Transmission
 * - Reception
 *
 * Copyright (C) 2009 Comelit Group SpA
 *
 ***************************************************************************/

#ifndef CSP_H
#define CSP_H

#include <stdint.h>
#include <string.h>
#include "ctp.h"
#include "logaddr.h"
#include "port.h"


#ifdef __cplusplus
extern "C" {
#endif


/// Default port for CSP signalling.
/// Any other port may be used if CRP is in place to make it reachable.
/// However CSP broadcast messages are sent to CTP_DEFAULT_BROADCAST_PORT
/// (24101), so that must be used in order to be able to receive broadcast
/// CSP messages.
#define CSP_DEFAULT_PORT    CTP_DEFAULT_BROADCAST_PORT


// CSP message format =====================================================

// Packed structs for network coding
CSPS_PACK_STRUCT_BEGIN

/// Data type to represent the CSP message length
typedef uint16_t csp_len_t;

/// CSP packet header
typedef struct
{
	uint8_t  rsvd0;   ///< Reserved: 0 on send, ignore on receive
	uint8_t  msg;     ///< Message type
} CSPS_PACK_STRUCT_STRUCT csp_header_t;

#define CAPAB_AUDIO_DST   0x00000001  ///< Can receive an audio flow
#define CAPAB_AUDIO_SRC   0x00000002  ///< Can send an audio flow
#define CAPAB_VIDEO_DST   0x00000004  ///< Can receive a video flow
#define CAPAB_VIDEO_SRC   0x00000008  ///< Can send a video flow
#define CAPAB_OPENDOOR    0x00000010  ///< Can open a door
#define CAPAB_MSTREAM     0x00000020  ///< Can send or receive multiple audio/video flows
#define CAPAB_MAX_NUM     6           ///< Max number of capabilities
#define CAPAB_NONE        0

/// Capability field
typedef uint32_t csp_capab_t;


/// @name CSP Messages
/// @{
#define MSG_START          0x01
#define MSG_CAPAB_REPORT   0x03
#define MSG_PROCEEDING     0x07
#define MSG_ALERTING       0x0a
#define MSG_CONNECT        0x0c
#define MSG_RELEASE        0x0e
#define MSG_MEDIAREQ       0x11
#define MSG_MEDIAOFFER     0x12
#define MSG_SETOUTPUT      0x20
#define MSG_SETOUTPUT2     0x21
#define MSG_OPENDOOR       0x2d
#define MSG_DOORSTATUS     0x35
#define MSG_REGISTER_REQ   0x40
#define MSG_REGISTER_RES   0x41
#define MSG_REGISTER_RENEW 0x44
#define MSG_CAPTURE_ADD    0x50
#define MSG_CAPTURE_UPDATE 0x51
#define MSG_CAPTURE_RES    0x52
#define MSG_SERVICE_ACTION 0x60
#define MSG_SERVICE_STATUS 0x61
#define MSG_VOICESTATUS    0x70
#define MSG_LIFTLAND_REQ   0x80
#define MSG_LIFTLAND_RES   0x81
#define MSG_LIFTCALL_REQ   0x82
#define MSG_LIFTCALL_RES   0x83
#define MSG_LIFTSTATUS_REQ 0x84
#define MSG_LIFTSTATUS_RES 0x85
#define MSG_FIRE_ALERT     0x86
#define MSG_VINPUT_SET     0x87
/// @}

typedef char memo_id_t[40];

typedef enum
{
	CSP_START_EXT_NONE = 0, //No extension
	CSP_START_EXT_MEMO, //start message for memo play call
	CSP_START_EXT_USER, //start message for calling a specific user
	CSP_START_EXT_SLAVE, //start message for calling a specific slave
	CSP_START_EXT_TECH_MESSAGE //start message for sending a technical alarm
} csp_start_extension_t;

/// @name START message
/// @{
/// ::MSG_START message format
typedef struct csp_msg_start_t
{
	csp_header_t hdr;     ///< Header
	logaddr_t from;       ///< Logical address of caller entity
	logaddr_t to;         ///< Logical address of called entity
	uint8_t   call_type;  ///< Type of call: "normal", TVCC, test etc
	uint8_t   call_flags;
	uint32_t  call_id;    ///< Call ID
	// Optional fields follow
	logaddr_t forwarder;
	uint8_t   from_role;
	uint8_t   forwarder_role;
	uint8_t   call_flags2;
	uint8_t   extension_type; ///< type of extension message
} CSPS_PACK_STRUCT_STRUCT csp_msg_start;

#define CALLTYPE_CFP        0xd0 ///< CFP (out-of-door call)
#define CALLTYPE_NORMAL     0xc0 ///< Normal door entry call
#define CALLTYPE_INTERCOM   0x80 ///< Intercom call
#define CALLTYPE_TVCC       0x40 ///< Video surveillance call
#define CALLTYPE_TEST       0x10 ///< Connectivity/bandwidth test call
#define CALLTYPE_VIP        0x01 ///< ViP calls

#define CALLFLAG_PRIOCALL1  0x01
#define CALLFLAG_PRIOCALL2  0x02
#define CALLFLAG_PRIOCALL3  0x04
#define CALLFLAG_PRIOCALL4  0x08
#define CALLFLAG_PRIOCALL_ALL \
	(CALLFLAG_PRIOCALL1 | CALLFLAG_PRIOCALL2 | \
	 CALLFLAG_PRIOCALL3 | CALLFLAG_PRIOCALL4)

#define CALLFLAG_CAPTURED   0x80 ///< The call is captured
#define CALLFLAG_DIVERTED   0x40 ///< The call has been diverted
#define CALLFLAG_EXTERNAL   0x20 ///< The call is being sent to a subunit registered as external
#define CALLFLAG_GROUP      0x10 ///< The call has been forked by a group

#define CALLFLAG2_PAGING    0x01 ///< This is a paging (public announcement) call

#define ROLE_PORTER         'P'
#define ROLE_INTUNIT        'I'
#define ROLE_CPS            'S'
#define ROLE_CFP            'F'
#define ROLE_CAMERA         'C'
#define ROLE_VIDOUT         'V'
#define ROLE_IO             'G'
#define ROLE_FIRE           'R'
#define ROLE_UNKNOWN        '?'

/// @}

/// @name CAPABILITY REPORT message
/// @{
/// ::MSG_CAPAB_REPORT message format
typedef struct csp_msg_capab_report_t
{
	csp_header_t hdr;        ///< Header
	uint8_t      role;       ///< Sender role
	uint8_t      keylock_map;///< Bit map of opendoor keylocks available
	csp_capab_t  capab;      ///< Exported capabilities
} CSPS_PACK_STRUCT_STRUCT csp_msg_capab_report;
/// @}

/// @name PROCEEDING message
/// @{
/// ::MSG_PROCEEDING message format
typedef struct csp_msg_proceeding_t
{
	csp_header_t hdr;     ///< Header
} CSPS_PACK_STRUCT_STRUCT csp_msg_proceeding;
/// @}

/// @name ALERTING message
/// @{
/// ::MSG_ALERTING message format
typedef struct csp_msg_alerting_t
{
	csp_header_t hdr;     ///< Header
	uint8_t      type;    ///< Alerting type
} CSPS_PACK_STRUCT_STRUCT csp_msg_alerting;

#define ALR_NORMAL     0x00  ///< normal ring
#define ALR_WAIT       0x01  ///< waiting ring
/// @}

/// @name RELEASE message
/// @{
/// ::MSG_RELEASE message format
typedef struct csp_msg_release_t
{
	csp_header_t hdr;     ///< Header
	uint8_t      cause;   ///< ending call cause
	// Optional fields follow
	uint8_t      rsvd0;
	logaddr_t    divert_target; ///< Divert target (if cause==DIVERTED)
} CSPS_PACK_STRUCT_STRUCT csp_msg_release;

#define CAUSE_NORMAL           0x00 ///< Normal call clearing
#define CAUSE_USER_BUSY        0x08 ///< Called user busy
#define CAUSE_NONSEL           0x0a ///< Non-selected user clearing
#define CAUSE_REJECTED         0x0d ///< Call rejected
#define CAUSE_DIVERTED         0x17 ///< Call diverted
#define CAUSE_NO_USER          0x19 ///< User not found  
#define CAUSE_NO_DEVICE        0x1a ///< Physical device does not exist
#define CAUSE_SYSTEM_BUSY      0x1b ///< A required resource is not available
#define CAUSE_CALL_UNSUPPORTED 0x41 ///< Call type not supported
#define CAUSE_MALFORMED_MSG    0x5e ///< Malformed message received
///< (e.g. too short)
#define CAUSE_INTERNAL_ERROR   0x71 ///< Internal error
#define CAUSE_FAILURE          0x7f ///< Generic failure
/// @}

/// @name CONNECT message
/// @{
/// ::MSG_CONNECT message format
typedef struct csp_msg_connect_t
{
	csp_header_t hdr;     ///< Header
} CSPS_PACK_STRUCT_STRUCT csp_msg_connect;
/// @}

/// @name MEDIAREQ message
/// @{

/// ::MSG_MEDIAREQ message format
typedef struct csp_msg_mediareq_t
{
	csp_header_t   hdr;            ///< Header
	/// ::MEDIAREQ_START/::MEDIAREQ_STOP | ::MEDIAREQ_AUDIO/::MEDIAREQ_VIDEO
	uint8_t        action_channel;
	uint8_t        flags;
	struct in_addr target_ip_addr;   ///< Requester IP Address
	uint16_t       target_transport; ///< Requester UDP Port or ViPER channel ID
	// Optional fields follow
	uint16_t       rtp_mtu;        ///< Maximum RTP packet size required
	///< (RTP header + payload) [bytes]
	uint32_t       pref_bitrate;   ///< Preferred bitrate (media, no RTP headers)
	// MultiStream fields
	uint16_t       max_xres;       ///< Maximum supported video width in pixels
	uint16_t       max_yres;       ///< Maximum supported video height in pixels
	uint16_t       pref_xres;      ///< Preferred video width in pixels
	uint16_t       pref_yres;      ///< Preferred video height in pixels
	uint8_t        max_fps;        ///< Maximum supported frames per second
	uint8_t        flow_id;        ///< Flow ID of this media request
} CSPS_PACK_STRUCT_STRUCT csp_msg_mediareq;

#define MEDIAREQ_CHANNEL_MASK 0x0f ///< Channel field bitmask
#define MEDIAREQ_AUDIO        0x08 ///< Audio flow request
#define MEDIAREQ_VIDEO        0x04 ///< Video flow request

#define MEDIAREQ_ACTION_MASK  0xf0 ///< Action field bitmask
#define MEDIAREQ_START        0x10 ///< Start flow request
#define MEDIAREQ_STOP         0x90 ///< Stop  flow request

/// Notify this MEDIAREQ sender can scale video up to max_xres X max_yres pixels; the sender MUST
/// accept streams with lower resolutions and MUST display them at the specified resolution.
#define MEDIAREQ_FLAG_SCALEUP   0x10

/// Notify this MEDIAREQ sender can scale video down to max_xres X max_yres pixels; the sender MUST
/// accept streams with higher resolutions and MUST display them at the specified resolution.
#define MEDIAREQ_FLAG_SCALEDOWN 0x20

/// Request to steal from current recipient
#define MEDIAREQ_FLAG_STEAL     0x08

/// Use UDP hole punching to establish requester IP and port.
/// @a ip_addr and @a udp_port fields are reserved (send 0, ignore on receive).
/// Not used for ViP Systems.
#define MEDIAREQ_FLAG_HOLEPUNCH 0x04

/// Tells if this MEDIAREQ message is over a ViPER tunnel.
/// If set:
///  - @a target_ip_addr    is reserved (send 0, ignore or receive).
///  - @a target_transport  is the ViPER channel ID
/// If not set:
///  - @a target_ip_addr    is the target IP address
///  - @a target_transport  is the target UDP port
#define MEDIAREQ_FLAG_VIPER     0x02

/// Default flow ID, to be used in case Multistream is not supported or not available but a flow ID
/// is requested by the implementation.
/// A flow ID is a tag identifying a set of MEDIAREQ senders; within this set the MEDIAREQ parameters
/// can be collapsed to a common resolution, framerate, bitrate and MTU.
/// A flow ID can be seen as a sort of multicast channel.
#define MEDIAREQ_FLOWID_DEFAULT 0
/// flow ID 255 (0xFF) identifies all active/supported flow IDs (sort of broadcast address)
#define MEDIAREQ_FLOWID_ALL     0xff

/// @}

/// @name MEDIAOFFER message
/// @{

/// ::MSG_MEDIAOFFER message format
typedef struct csp_msg_mediaoffer_t
{
	csp_header_t   hdr;            ///< Header
	/// ::MEDIAOFFER_START/::MEDIAOFFER_STOP | ::MEDIAOFFER_AUDIO/::MEDIAOFFER_VIDEO
	uint8_t        action_channel;
	uint8_t        flags;
	struct in_addr group_ip_addr;   ///< Multicast group IP Address
	uint16_t       group_transport; ///< Multicast group UDP Port or ViPER channel ID
	uint8_t        rtp_pt;          ///< RTP payload type
	uint8_t        codec;           ///< Encoder used
} CSPS_PACK_STRUCT_STRUCT csp_msg_mediaoffer;

#define MEDIAOFFER_CHANNEL_MASK 0x0f ///< Channel field bitmask
#define MEDIAOFFER_AUDIO        0x08 ///< Audio flow request
#define MEDIAOFFER_VIDEO        0x04 ///< Video flow request

#define MEDIAOFFER_ACTION_MASK  0xf0 ///< Action field bitmask
#define MEDIAOFFER_START        0x10 ///< Start flow request
#define MEDIAOFFER_STOP         0x90 ///< Stop  flow request

/// Tells if this MEDIAOFFER message is over a ViPER tunnel.
/// If set:
///  - @a target_ip_addr    is reserved (send 0, ignore or receive).
///  - @a target_transport  is the ViPER channel ID
/// If not set:
///  - @a target_ip_addr    is the target IP address
///  - @a target_transport  is the target UDP port
#define MEDIAOFFER_FLAG_VIPER     0x02

#define MEDIAOFFER_CODEC_MPEG4C 0x01    ///< Comelit-reduced MPEG4
#define MEDIAOFFER_CODEC_H264BP 0x02    ///< H.264 Baseline Profile (BP)
#define MEDIAOFFER_CODEC_MJPEG  0x03    ///< MJPEG

#define MEDIAOFFER_CODEC_ALAW   0x01    ///< A-law
#define MEDIAOFFER_CODEC_ULAW   0x02    ///< μ-law

/// @}

/// @name DOORSTATUS message
/// @{
/// ::MSG_DOORSTATUS message format
typedef struct csp_msg_doorstatus_t
{
	csp_header_t   hdr;            ///< Header
	logaddr_t      addr;           ///< Device handling the door
	uint8_t        door_status;    ///< ::DOOR_OPENED/::DOOR_CLOSED
} CSPS_PACK_STRUCT_STRUCT csp_msg_doorstatus;

#define DOOR_OPENED      0x01  ///< Door is open
#define DOOR_CLOSED      0x00  ///< Door is closed
/// @}

/// @name SETOUTPUT message
/// @{
/// ::MSG_SETOUTPUT message format
typedef struct csp_msg_setoutput_t
{
	csp_header_t   hdr;        ///< Header
	uint8_t        module;     ///< Module that contains the relay
	uint8_t        output_n;   ///< Relay index (within the module)
} CSPS_PACK_STRUCT_STRUCT csp_msg_setoutput;

#define OUTPUT_MODULE_BUILTIN   0xff ///< Command a built-in relay
/// @}

/// @name SETOUTPUT2 message
/// @{
/// ::MSG_SETOUTPUT2 message format
typedef struct csp_msg_setoutput2_t
{
	csp_header_t   hdr;        ///< Header
	uint8_t        module;     ///< Module that contains the relay
	uint8_t        output_n;   ///< Relay index (within the module)
	uint8_t        action;     ///< Requested action
} CSPS_PACK_STRUCT_STRUCT csp_msg_setoutput2;

#define OUTPUT_MODULE_BUILTIN     0xff ///< Command a built-in relay

#define OUTPUT_ACTION_AUTO        0x00 ///< Execute default action
#define OUTPUT_ACTION_SET_OFF     0x01 ///< MONO: disattiva e stop timer | BIST: metti OFF (se già OFF non fa nulla)
#define OUTPUT_ACTION_SET_ON      0x02 ///< MONO: attiva e start timer   | BIST: metti ON  (se già ON  non fa nulla)
/// @}

/// @name OPENDOOR message
/// @{
/// ::MSG_OPENDOOR message format
typedef struct csp_msg_opendoor_t
{
	csp_header_t   hdr;            ///< Header
	logaddr_t      addr;           ///< Device handling door/relay
	uint8_t        id_relay;       ///< Identifier of the relay to be triggered (1 or 2)
} CSPS_PACK_STRUCT_STRUCT csp_msg_opendoor;
/// @}

/// @name REGISTER_REQ message
/// @{
/// ::MSG_REGISTER_REQ message format
typedef struct csp_msg_register_req_t
{
	csp_header_t   hdr;            ///< Header
	uint16_t       reg_id;         ///< Registration identifier
	logaddr_t      user_addr;      ///< User to be registered
	uint16_t       expires;        ///< Registration TTL [s]
	// Optional fields follow
	uint8_t        flags;
} CSPS_PACK_STRUCT_STRUCT csp_msg_register_req;

/// This client is registering ad additional Logaddr. The main logaddr of the
/// client may be different.
#define REGISTER_REQ_FLAG_EXTERNAL     0x01
/// @}

/// @name REGISTER_RES message
/// @{
/// ::MSG_REGISTER_RES message format

/// Messaggio ::MSG_REGISTER_RES
typedef struct csp_msg_register_res_t
{
	csp_header_t   hdr;            ///< Header
	uint16_t       reg_id;         ///< Registration identifier
	logaddr_t      user_addr;      ///< User to be registered
	uint8_t        response;       ///< Registration response
	uint8_t        rsvd0;
} CSPS_PACK_STRUCT_STRUCT csp_msg_register_res;

#define REG_RES_OK                         0x00 ///< OK, Registration accepted
#define REG_RES_ERR_INTERNAL_REGISTERED    0xe1 ///< Reject "external" subunit because another, internal, subunit is already registered
#define REG_RES_ERR                        0xee ///< Generic error, registration rejected
/// @}

/// @name REGISTER_RENEW message
/// @{
/// ::MSG_REGISTER_RENEW message format
typedef struct csp_msg_register_renew_t
{
	csp_header_t   hdr;            ///< Header
	uint16_t       renew_id;       ///< Registration renew request identifier
	logaddr_t      subproxy_addr;  ///< Address to which to renew registr
} CSPS_PACK_STRUCT_STRUCT csp_msg_register_renew;
/// @}

/// @name CAPTURE_ADD message
/// @{
/// ::MSG_CAPTURE_ADD message format
typedef struct csp_msg_capture_add_t
{
	csp_header_t   hdr;            ///< Header
	uint16_t       req_id;         ///< Request identifier
	logaddr_t      capturer;
	logaddr_t      captured;
	logaddr_range_t dest_range;
	uint16_t       expires;        ///< Capture TTL [s]
} CSPS_PACK_STRUCT_STRUCT csp_msg_capture_add;

/// @}

/// @name CAPTURE_UPDATE message
/// @{
/// ::MSG_CAPTURE_UPDATE message format
typedef struct csp_msg_capture_update_t
{
	csp_header_t   hdr;            ///< Header
	uint16_t       req_id;         ///< Request identifier
	uint16_t       handle;         ///< Opaque capture handle
	uint16_t       expires;        ///< Capture TTL [s]
} CSPS_PACK_STRUCT_STRUCT csp_msg_capture_update;

/// @}

/// @name CAPTURE_RES message
/// @{
/// ::MSG_CAPTURE_RES message format
typedef struct csp_msg_capture_res_t
{
	csp_header_t   hdr;            ///< Header
	uint16_t       req_id;         ///< Request identifier
	uint8_t        response;
	uint8_t        rsvd0;
	uint16_t       handle;         ///< Opaque capture handle
} CSPS_PACK_STRUCT_STRUCT csp_msg_capture_res;

#define CAPTURE_OK                0x00 ///< OK, request accepted
#define CAPTURE_ERR_PARAM         0xe0 ///< Error: Illegal packet parameters
#define CAPTURE_ERR_BAD_CAPTURED  0xe1 ///< Error: Bad captured (and won't relay)
#define CAPTURE_ERR_COLLISION     0xe2 ///< Error: dest_range collision
#define CAPTURE_ERR_NOENT         0xe3 ///< Error: no such entry
#define CAPTURE_ERR_INTERNAL      0xee ///< Internal Error
/// @}


// Action types for SERVICE_ACTION and SERVICE_STATUS
#define SVC_FEATURE_ADD_ADDR         1 ///< Additional Logaddr
#define SVC_FEATURE_MEMORY           2 ///< Audio[/video] memory
#define SVC_FEATURE_ABSENT_MSG       3 ///< Message on absent
#define SVC_FEATURE_CALL_FWD         4 ///< Call forwarding

/// @name SERVICE_ACTION message
/// @{

#define SVC_ACTION_ACTIVATE          1
#define SVC_ACTION_DEACTIVATE        2
#define SVC_ACTION_GET_STATUS        3

typedef struct
{
	uint8_t action;
} CSPS_PACK_STRUCT_STRUCT csp_svc_action_simple;

typedef struct
{
	uint8_t      action;
	uint8_t      index;
} CSPS_PACK_STRUCT_STRUCT csp_svc_action_additional_addr;

typedef union
{
	csp_svc_action_simple            simple; ///< "Base class" for simple features
	
	csp_svc_action_additional_addr   additional_addr;
	csp_svc_action_simple            memory;
	csp_svc_action_simple            absent_msg;
	csp_svc_action_simple            call_fwd;
} CSPS_PACK_STRUCT_STRUCT csp_svc_action_item;

/// ::MSG_SERVICE_ACTION message format
typedef struct
{
	csp_header_t         hdr;            ///< Header
	uint8_t              feature_id;
	uint8_t              rsvd0;
	csp_svc_action_item  a;             ///< NOTE: only the size of the used union field  is sent
} CSPS_PACK_STRUCT_STRUCT csp_msg_svc_action;
/// @}

/// @name SERVICE_STATUS message
/// @{

// Status values for SERVICE_ACTION and SERVICE_STATUS
// - The most  significant nibble (mask 0xf0) indicates the status "family".
// - The least significant nibble (mask 0x0f) indicates the specific status
//   within the family.
// 0x00..0x0f: Inactive
#define SVC_STATUS_NOT_ENABLED       0x01 ///< Feature cannot be activated (e.g. not enabled in configuration)
#define SVC_STATUS_INACTIVE          0x02 ///< Feature can be activated, but is now not enabled
// 0x10..0x1f: Transient states
#define SVC_STATUS_ACTIVATING        0x11 ///< Activation in progress
#define SVC_STATUS_DEACTIVATING      0x12 ///< Deactivation in progress
// 0x80..0x8f: Active
#define SVC_STATUS_ACTIVE            0x80 ///< Feature active
// 0xe0..0xef: Error statuses: the feature activation has been requested but
//             failed; activation will be tried again later and may succeed.
//             Further status requests will return the same status (until it changes).
#define SVC_STATUS_REJECTED          0xe2 ///< Feature activation requested, but rejected (retrying)
#define SVC_STATUS_NO_REPLY          0xe3 ///< Feature activation requested, but timed out (retrying)
#define SVC_STATUS_ERROR_STATUS      0xef ///< Other/unknown/generic error status

typedef struct
{
	uint8_t status;
} CSPS_PACK_STRUCT_STRUCT csp_svc_status_simple;

typedef struct
{
	uint8_t      status;
	uint8_t      index;
	logaddr_t    addr;
} CSPS_PACK_STRUCT_STRUCT csp_svc_status_additional_addr;

typedef union
{
	csp_svc_status_simple            simple; ///< "Base class" for simple features
	
	csp_svc_status_additional_addr   additional_addr;
	csp_svc_status_simple            memory;
	csp_svc_status_simple            absent_msg;
	csp_svc_status_simple            call_fwd;
} CSPS_PACK_STRUCT_STRUCT csp_svc_status_item;

/// ::MSG_SERVICE_STATUS message format
typedef struct
{
	csp_header_t         hdr;            ///< Header
	uint8_t              feature_id;
	uint8_t              err_code;
	csp_svc_status_item  s;             ///< NOTE: only the size of the used union field is sent
} CSPS_PACK_STRUCT_STRUCT csp_msg_svc_status;

#define SVC_STATUS_OK                0x00 ///< OK, request accepted
#define SVC_STATUS_ERR_FEATURE_UNKN  0xe1 ///< Error: feature unknown
#define SVC_STATUS_ERR_NO_ENTRY      0xe2 ///< The addressed entry does not exist
#define SVC_STATUS_ERR_INTERNAL      0xee ///< Internal Error
#define SVC_STATUS_ERR_UNKNOWN       0xef ///< Unknown/generic error
/// @}

/// @name VOICESTATUS message
/// @{
/// ::MSG_VOICESTATUS message format
typedef struct csp_msg_voicestatus_t
{
	csp_header_t   hdr;            ///< Header
	logaddr_t      interlocutor;   ///< Device that open/close voice channel
	uint8_t        voice_status;  ///< ::VOICE_OPENED/::VOICE_CLOSED
	// Optional fields follow
	uint8_t        flags;
} CSPS_PACK_STRUCT_STRUCT csp_msg_voicestatus;

#define VOICE_OPENED      0x01  ///< Voice channel open
#define VOICE_CLOSED      0x00  ///< Voice channel closed
/// @}

#define LIFT_MANUFACTURER_MASK  0x0f ///< Bit reserved in flags field for manufacturer id
#define LIFT_KONE               0x01 ///< Kone Message protocol

/// @name lift_status this values are used in LIFTSTATUS_RES messages to
/// describe the status of the request.
/// These values map the status of the request in the manufacturer elevator server
/// @{
#define LIFT_STATUS_PENDING     0x01 ///< request accepted, ready to be sent to manufacturer elevator server
#define LIFT_STATUS_SENT        0x02 ///< request sent to manufacturer elevator server, still no response though
#define LIFT_STATUS_QUEUED      0x03 ///< request sent and accepted by the manufacturer elevator server, to be processed
#define LIFT_STATUS_PROCESSING  0x04 ///< request is currently proccessed by the manufacturer elevator server
#define LIFT_STATUS_PROCESSED   0x05 ///< request processed successfully and terminated by the manufacturer elevator server
#define LIFT_STATUS_CANCELED    0x06 ///< request deleted by the manufacturer elevator server for some unknown errors
#define LIFT_STATUS_TIMEOUT     0x07 ///< request sent to elevator manufacturer server but no response after some time
/// @}

/// @name lift_response these values are used in every elevator response message
/// to notify if the request has produced an error or not (eventually specifying the error cause)
/// @{
#define LIFT_RES_OK                  0x00 ///< valid request
#define LIFT_RES_ADDR_NOT_CONFIGURED 0x01 ///< one or more ViP address do not have necessary parameters
#define LIFT_RES_ADDR_UNKNOWN        0x02 ///< one or more ViP address not mapped in the elevator manufacturer protocol
#define LIFT_RES_ADDR_NOT_SUPPORTED  0x03 ///< one or more ViP address has not the permission to made the request
#define LIFT_RES_REQ_DUPLICATED      0x04 ///< manufacturer elevator server is already processing an identical request
#define LIFT_RES_REQ_NOT_CAPABLE     0x05 ///< request handler not implemented
#define LIFT_RES_REQ_NOT_FOUND       0x06 ///< request not found
#define LIFT_RES_REQ_REJECTED        0x07 ///< one or more fields necessary to connect to manufacturer elevator server are empty (or corrupted)
#define LIFT_RES_NETWORK_ERR         0x08 ///< request could not be sent due to network error
/// @}

/// @name LIFTLAND messages that describe landing call operation
/// @{
///
/// ::MSG_LIFTLAND_REQ message format
typedef struct csp_msg_liftland_req_t
{
	csp_header_t   hdr;            ///< Header
	uint16_t
	req_id;         ///< Id that represent the request. This is used in csp lift status messages
	logaddr_t      land_addr;      ///< Landing ViP Address
	uint8_t        direction;      ///< elevator direction after landing
	uint8_t        rsvd0;
	uint32_t       flags;
} CSPS_PACK_STRUCT_STRUCT csp_msg_liftland_req;

#define LIFTLAND_DIR_UP    0x01
#define LIFTLAND_DIR_DOWN  0x02
#define LIFTLAND_DIR_LEFT  0x03
#define LIFTLAND_DIR_RIGHT 0x04

/// ::MSG_LIFTLAND_RES message format
typedef struct csp_msg_liftland_res_t
{
	csp_header_t hdr;            ///< Header
	uint16_t
	req_id;         ///< Id that represent the request. This is used in csp lift status messages
	logaddr_t    land_addr;      ///< Landing ViP Address
	uint8_t      direction;      ///< elevator direction after landing
	uint8_t      response;       ///< Request response @see lift_response
	uint32_t     flags;
} CSPS_PACK_STRUCT_STRUCT csp_msg_liftland_res;
/// @}

/// @name LIFTCALL messages that describe landing call operation
/// @{
///
/// ::MSG_LIFTCALL_REQ message format
typedef struct csp_msg_liftcall_req_t
{
	csp_header_t hdr;            ///< Header
	uint16_t
	req_id;         ///< Id that represent the request. This is used in csp lift status messages
	logaddr_t    src_addr;       ///< Source ViP Address
	logaddr_t    dst_addr;       ///< Destination ViP Address
	uint32_t     flags;
} CSPS_PACK_STRUCT_STRUCT csp_msg_liftcall_req;

/// ::MSG_LIFTCALL_RES message format
typedef struct csp_msg_liftcall_res_t
{
	csp_header_t hdr;            ///< Header
	uint16_t
	req_id;         ///< Id that represent the request. This is used in csp lift status messages
	logaddr_t    src_addr;       ///< Source ViP Address
	logaddr_t    dst_addr;       ///< Destination ViP Address
	uint8_t      response;       ///< Request response @see lift_response
	uint8_t      rsvd0[3];
	uint32_t     flags;
} CSPS_PACK_STRUCT_STRUCT csp_msg_liftcall_res;
/// @}

/// @name LIFTSTATUS messages for requesting the status of an elevator operation
/// @{
/// ::MSG_LIFTSTATUS_REQ message format
typedef struct csp_msg_liftstatus_req_t
{
	csp_header_t hdr;            ///< Header
	uint16_t
	req_id;         ///< Id that represent the request. This is used in csp lift status messages
	uint32_t     flags;
} CSPS_PACK_STRUCT_STRUCT csp_msg_liftstatus_req;

/// ::MSG_LIFTSTATUS_RES message format
typedef struct csp_msg_liftstatus_res_t
{
	csp_header_t hdr;            ///< Header
	uint16_t     req_id;         ///< Id that represent the request
	uint8_t      status;         ///< Request response @see lift_status
	uint8_t      response;       ///< Request response @see lift_response
	uint16_t     rsvd0;
	uint32_t     flags;
} CSPS_PACK_STRUCT_STRUCT csp_msg_liftstatus_res;
/// @}

/// @name FIRE_ALERT message
/// @{
/// ::MSG_FIRE_ALERT message format
typedef struct csp_msg_fire_alert_t
{
    csp_header_t hdr;     ///< Header
    uint16_t     req_id;         ///< Id that represent the request
    uint8_t      alert_type;  ///< Type of alert: fault, evaquate, fire, prealarm
    uint8_t      rsvd0[3];
} CSPS_PACK_STRUCT_STRUCT csp_msg_fire_alert;
/// @}

/// @name alert_type
/// type of alert from fire controller to propagate
/// @{
#define FIRE_ALERT_STOP         0x00
#define FIRE_ALERT_EVACUATE     0x10
#define FIRE_ALERT_ALARM        0x20
#define FIRE_ALERT_PREALARM     0x40
#define FIRE_ALERT_FAULT        0x80
#define FIRE_ALERT_TECH_ALRM    0x01
#define FIRE_ALERT_COM_ERR      0x02
/// @}

/// @name SET_VIRINP message
/// @{
/// ::MSG_SET_VIRINP message format
typedef struct csp_msg_set_virtinp_t
{
    csp_header_t hdr;     ///< Header
    uint16_t     req_id;         ///< Id that represent the request
    uint32_t     virt_inp_mask;  ///< Mask for which of the virt inp should be set: 32 totals, starting from 0
    uint32_t     virt_inp_vals;  ///< Mask for virt inp values to set: 0->reset, 1->set
} CSPS_PACK_STRUCT_STRUCT csp_msg_set_virtinp;
/// @}
///

/// Union of all CSP message types
typedef union
{
    csp_header_t hdr;
	// ---
	csp_msg_start           start;
	csp_msg_capab_report    capab_report;
	csp_msg_proceeding      proceeding;
	csp_msg_alerting        alerting;
	csp_msg_release         release;
	csp_msg_connect         connect;
	csp_msg_mediareq        mediareq;
	csp_msg_mediaoffer      mediaoffer;
	csp_msg_doorstatus      doorstatus;
	csp_msg_opendoor        opendoor;
	csp_msg_register_req    register_req;
	csp_msg_register_res    register_res;
	csp_msg_register_renew  register_renew;
	csp_msg_capture_add     capture_add;
	csp_msg_capture_update  capture_update;
	csp_msg_capture_res     capture_res;
	csp_msg_svc_action      svc_action;
	csp_msg_svc_status      svc_status;
	csp_msg_voicestatus     voicestatus;
	csp_msg_liftland_req    liftland_req;
	csp_msg_liftland_res    liftland_res;
	csp_msg_liftcall_req    liftcall_req;
	csp_msg_liftcall_res    liftcall_res;
	csp_msg_liftstatus_req  liftstatus_req;
	csp_msg_liftstatus_res  liftstatus_res;
	csp_msg_fire_alert      fire_alerting;
	csp_msg_set_virtinp     set_virtinp;
} CSPS_PACK_STRUCT_STRUCT csp_message_t;

CSPS_PACK_STRUCT_END

/**
 * Return the minimum lenght of each message type.
 * @param  msgtype  Message type.
 * @return Minimum message length, or 0 if @a msgtype is unimplemented (and
 *         unexpected).
 */
csp_len_t msg_minlen(uint8_t msgtype);


// CSP message buffer ======================================================

/// @name CSP Message Buffer
/// Defines a structure that holds a CSP message.
/// @{

/**
 * Buffer holding a CSP message.
 */
typedef struct
{
	void      *dat;   ///< Pointer to data
	csp_len_t  len;   ///< Data length [Bytes]
} csp_msgbuf;

/**
 * Deallocate a csp_msgbuf (including the data pointer).
 *
 * @param  buf  The buffer to deallocate (must be not NULL and with data).
 */
void CSPS_API csp_msgbuf_free(csp_msgbuf *buf);

/**
 * True if the buffer is correct.
 * This checks correct allocation, not the message content.
 */
CSPS_INLINE int csp_msgbuf_isvalid(const csp_msgbuf *buf)
{
	return ((buf != NULL) && (buf->dat != NULL));
}

/// @}


// CSP message generation and transmission =================================

/// @name Create CSP messages
/// These functions create and fill a new CSP message. In detail, they:
/// - Create a csp_msgbuf with data pointer and fill them.
/// - Return a pointer to the message, or NULL on error.
/// @{

csp_msgbuf *CSPS_API csp_create_start(
        const logaddr_t      *src_logaddr,
        const logaddr_t      *dst_logaddr,
        uint8_t               call_type,
        uint32_t              call_id);
        
csp_msgbuf *CSPS_API csp_create_start40(
        const logaddr_t      *src_logaddr,
        const logaddr_t      *dst_logaddr,
        uint8_t               call_type,
        uint8_t               call_flags,
        uint32_t              call_id,
        const logaddr_t      *fwdr_logaddr,
        uint8_t               from_role,
        uint8_t               fwdr_role);
        
csp_msgbuf *CSPS_API csp_create_start41(
        const logaddr_t      *src_logaddr,
        const logaddr_t      *dst_logaddr,
        uint8_t               call_type,
        uint8_t               call_flags,
        uint32_t              call_id,
        const logaddr_t      *fwdr_logaddr,
        uint8_t               from_role,
        uint8_t               fwdr_role,
        uint8_t               call_flags2);
        
csp_msgbuf *CSPS_API csp_create_release(
        uint8_t               cause);
        
csp_msgbuf *CSPS_API csp_create_release14(
        uint8_t               cause,
        const logaddr_t      *divert_target);
        
/// @}


/// @name Send CSP messages
/// These functions send CSP messages from a csp_msgbuf.
/// @{

/**
 * Send a CSP message over a CTP connection (and consume it).
 *
 * @param  cid  ID of the CTP connection over which the message is to be sent.
 * @param  buf  CSP message to send. It is consumed even in case of failure,
 *              so the pointer will become invalid after this function
 *              has returned. Must not be NULL.
 */
void CSPS_API csp_send(ctp_cid cid, csp_msgbuf *buf);

/**
 * Send a copy of a CSP message over a CTP connection.
 *
 * @param  cid  ID of the CTP connection over which the message is to be sent.
 * @param  buf  CSP message to send. It is not modified. Must not be NULL.
 *
 * @retval 0  if the packet has been queued for sending.
 * @retval <0 on error
 */
int CSPS_API csp_send_copy(ctp_cid cid, const csp_msgbuf *buf);

/**
 * Broadcast a CSP message (and consume it).
 *
 * @param  ctp_identity  Sender CTP Identity.
 * @param  my_logaddr    Sender Logaddr (for CTP Extra Info).
 * @param  buf           Payload to send. Must not be NULL.
 */
void CSPS_API csp_send_broadcast(
        ctp_identity_t       *ctp_identity,
        const logaddr_t      *my_logaddr,
        csp_msgbuf           *buf);
        
/**
 * Broadcast a copy of a CSP message.
 *
 * @param  ctp_identity  Sender CTP Identity.
 * @param  my_logaddr    Sender Logaddr (for CTP Extra Info).
 * @param  buf           Payload to send. Must not be NULL.
 *
 * @retval 0  if the packet has been queued for sending.
 * @retval <0 on error
 */
int CSPS_API csp_send_broadcast_copy(
        ctp_identity_t       *ctp_identity,
        const logaddr_t      *my_logaddr,
        const csp_msgbuf     *buf);
        
/// @}


/// @name Create and send CSP messages
/// These functions create, fill and send a new CSP message. In detail, they:
/// - Allocate a buffer to hold a CSP message and fill it.
/// - Send the message via the appropriate CTP function.
/// @{

/// Send START message with all fields up to Call ID and with call_flags
/// equal to zero (28 bytes total).
void CSPS_API csp_send_start(
        ctp_cid               cid,
        const logaddr_t      *src_logaddr,
        const logaddr_t      *dst_logaddr,
        uint8_t               call_type,
        uint32_t              call_id);
        
/// Send START message with all fields up to Forwarder Role (40 bytes total).
void CSPS_API csp_send_start40(
        ctp_cid               cid,
        const logaddr_t      *src_logaddr,
        const logaddr_t      *dst_logaddr,
        uint8_t               call_type,
        uint8_t               call_flags,
        uint32_t              call_id,
        const logaddr_t      *fwdr_logaddr,
        uint8_t               from_role,
        uint8_t               fwdr_role);
        
/// Send START message with all fields up to Flags 2 (41 bytes total).
void CSPS_API csp_send_start41(
        ctp_cid               cid,
        const logaddr_t      *src_logaddr,
        const logaddr_t      *dst_logaddr,
        uint8_t               call_type,
        uint8_t               call_flags,
        uint32_t              call_id,
        const logaddr_t      *fwdr_logaddr,
        uint8_t               from_role,
        uint8_t               fwdr_role,
        uint8_t               flags2);

/// Send START message with message extension for memo play (42 + memoid_size bytes total)
void CSPS_API csp_send_start_memo(ctp_cid          cid,
                                  const logaddr_t *src_logaddr,
                                  const logaddr_t *dst_logaddr,
                                  uint8_t          call_type,
                                  uint8_t          call_flags,
                                  uint32_t         call_id,
                                  const logaddr_t *fwdr_logaddr,
                                  uint8_t          from_role,
                                  uint8_t          fwdr_role,
                                  uint8_t          call_flags2,
                                  uint8_t          memoid_size,
                                  const char*      memoid);

/// Send START message with message extension (42 + memoid_size bytes total)
void CSPS_API csp_send_start_tech_message(ctp_cid          cid,
                                  const logaddr_t *src_logaddr,
                                  const logaddr_t *dst_logaddr,
                                  uint8_t          call_type,
                                  uint8_t          call_flags,
                                  uint32_t         call_id,
                                  const logaddr_t *fwdr_logaddr,
                                  uint8_t          from_role,
                                  uint8_t          fwdr_role,
                                  uint8_t          call_flags2,
                                  uint8_t          messageid_size,
                                  const char*      messageid);

/// Send START message with message extension to call one specific user (42 + userid_size bytes total)
void CSPS_API csp_send_start_user(ctp_cid          cid,
                                  const logaddr_t *src_logaddr,
                                  const logaddr_t *dst_logaddr,
                                  uint8_t          call_type,
                                  uint8_t          call_flags,
                                  uint32_t         call_id,
                                  const logaddr_t *fwdr_logaddr,
                                  uint8_t          from_role,
                                  uint8_t          fwdr_role,
                                  uint8_t          call_flags2,
                                  uint8_t          userid_size,
                                  const char*      userid);

/// Send START message with message extension to call one specific slave (42 + 2 bytes total)
void CSPS_API csp_send_start_slave(ctp_cid          cid,
                                   const logaddr_t *src_logaddr,
                                   const logaddr_t *dst_logaddr,
                                   uint8_t          call_type,
                                   uint8_t          call_flags,
                                   uint32_t         call_id,
                                   const logaddr_t *fwdr_logaddr,
                                   uint8_t          from_role,
                                   uint8_t          fwdr_role,
                                   uint8_t          call_flags2,
                                   logaddr_sub_t    slaveid);

void CSPS_API csp_send_capab_report(
        ctp_cid               cid,
        uint8_t               role,
        uint8_t               keylock_map,
        csp_capab_t           capab);
        
void CSPS_API csp_send_proceeding(
        ctp_cid               cid);
        
void CSPS_API csp_send_alerting(
        ctp_cid               cid,
        uint8_t               type);
        
void CSPS_API csp_send_connect(
        ctp_cid               cid);
        
void CSPS_API csp_send_release(
        ctp_cid               cid,
        uint8_t               cause);
        
void CSPS_API csp_send_release14(
        ctp_cid               cid,
        uint8_t               cause,
        const logaddr_t      *divert_target);
        
/**
 * Send a 10-bytes MEDIAREQ message.
 * @note ip_addr is ignored is MEDIAREQ_FLAG_VIPER is set, required otherwise.
 */
void CSPS_API csp_send_mediareq(
        ctp_cid               cid,
        uint8_t               action_channel,
        uint8_t               flags,
        const struct in_addr *ip_addr,
        uint16_t              target_transport);
        
/**
 * Send a 16-bytes MEDIAREQ message.
 * @note ip_addr is ignored is MEDIAREQ_FLAG_VIPER is set, required otherwise.
 */
void CSPS_API csp_send_mediareq16(
        ctp_cid               cid,
        uint8_t               action_channel,
        uint8_t               flags,
        const struct in_addr *ip_addr,
        uint16_t              target_transport,
        uint16_t              rtp_mtu,
        uint32_t              pref_bitrate);
        
/**
 * Send a 26-bytes MEDIAREQ message.
 * @note ip_addr is ignored if MEDIAREQ_FLAG_VIPER is set, required otherwise.
 */
void CSPS_API csp_send_mediareq26(
        ctp_cid               cid,
        uint8_t               action_channel,
        uint8_t               flags,
        const struct in_addr *ip_addr,
        uint16_t              udp_transport,
        uint16_t              rtp_mtu,
        uint32_t              pref_bitrate,
        uint16_t              max_xres,
        uint16_t              max_yres,
        uint16_t              pref_xres,
        uint16_t              pref_yres,
        uint8_t               max_fps,
        uint8_t               flow_id);
        
/**
 * Send a MEDIAOFFER message.
 * @note ip_addr is ignored if MEDIAREQ_FLAG_VIPER is set, required otherwise.
 */
void CSPS_API csp_send_mediaoffer(
        ctp_cid               cid,
        uint8_t               action_channel,
        uint8_t               flags,
        const struct in_addr *ip_addr,
        uint16_t              transport,
        uint8_t               rtp_pt,
        uint8_t               codec);
        
void CSPS_API csp_send_doorstatus(
        ctp_identity_t       *ctp_identity,
        const logaddr_t      *my_logaddr,
        const logaddr_t      *addr,
        uint8_t               open);
        
void CSPS_API csp_send_setoutput(
        ctp_cid               cid,
        uint8_t               module_idx,
        uint8_t               output_idx);
        
void CSPS_API csp_send_setoutput2(
        ctp_cid               cid,
        uint8_t               module_idx,
        uint8_t               output_idx,
        uint8_t               action);
        
void CSPS_API csp_send_opendoor_bcast(
        ctp_identity_t       *ctp_identity,
        const logaddr_t      *my_logaddr,
        const logaddr_t      *dst_logaddr,
        uint8_t               relay_num);
        
void CSPS_API csp_send_opendoor_onconn(
        ctp_cid               cid,
        const logaddr_t      *dst_logaddr,
        uint8_t               relay_num);
        
void CSPS_API csp_send_register_req(
        ctp_cid               cid,
        uint16_t              reg_id,
        const logaddr_t      *user_addr,
        uint16_t              expires,
        uint8_t               flags);
        
void CSPS_API csp_send_register_res(
        ctp_cid               cid,
        uint16_t              reg_id,
        const logaddr_t      *user_addr,
        uint8_t               response);
        
void CSPS_API csp_send_register_renew(
        ctp_identity_t       *ctp_identity,
        const logaddr_t      *my_logaddr,
        uint16_t              renew_id,
        const logaddr_t      *subproxy_addr);
        
void CSPS_API csp_send_capture_add(
        ctp_cid                 cid,
        uint16_t                req_id,
        const logaddr_t        *capturer,
        const logaddr_t        *captured,
        const logaddr_range_t  *dest_range,
        uint16_t                expires);
        
void CSPS_API csp_send_capture_update(
        ctp_cid                 cid,
        uint16_t                req_id,
        uint16_t                handle,
        uint16_t                expires);
        
void CSPS_API csp_send_capture_res(
        ctp_cid                 cid,
        uint16_t                req_id,
        uint8_t                 response,
        uint16_t                handle);
        
void CSPS_API csp_send_svc_action_simple(
        ctp_cid                 cid,
        uint8_t                 feature_id,
        uint8_t                 action);
        
void CSPS_API csp_send_svc_action_additional_addr(
        ctp_cid                 cid,
        uint8_t                 action,
        uint8_t                 index);
        
void CSPS_API csp_send_svc_status_simple(
        ctp_cid                 cid,
        uint8_t                 feature_id,
        uint8_t                 err_code,
        uint8_t                 status);
        
void CSPS_API csp_send_svc_status_additional_addr(
        ctp_cid                 cid,
        uint8_t                 err_code,
        uint8_t                 status,
        uint8_t                 index,
        const logaddr_t        *addr);
        
/**
 * Send status of the voice channel
 * @param cid           connection cid
 * @param interlocutor  logaddr's sender of voice channel status
 * @param voice_status  status of the voice channel
 * @param flags         optional parameter (for further informations)
 */
void CSPS_API csp_send_voicestatus(
        ctp_cid               cid,
        const logaddr_t      *interlocutor,
        uint8_t               voice_status,
        uint8_t               flags);
        
/**
 * Send an elevator landing request
 * @param cid           connection cid
 * @param land_addr     logaddr where elevator must landing
 * @param direction     direction of the elevator after landing
 * @param req_id        Id of the elevator request, generated by the client. Must be unique
 * @param flags         the first 4 bits contain the elevator manufacturer.
 *                      Other bits remain unused, for further informations
 */
void CSPS_API csp_send_liftland_req(
        ctp_cid               cid,
        const logaddr_t      *land_addr,
        uint8_t               direction,
        uint16_t              req_id,
        uint32_t              flags);
        
/**
 * Send response to an elevator landing request
 * @param cid           connection cid
 * @param land_addr     logaddr where elevator must landing
 * @param direction     direction of the elevator
 * @param req_id        Id of the elevator request
 * @param response      request response @see lift_response
 * @param flags         the first 4 bits contain the elevator manufacturer.
 *                      Other bits remain unused, for further informations
 */
void CSPS_API csp_send_liftland_res(
        ctp_cid               cid,
        const logaddr_t      *land_addr,
        uint8_t               direction,
        uint16_t              req_id,
        uint8_t               response,
        uint32_t              flags);
        
/**
 * Send an elevator call request (elevator from/to)
 * @param cid        connection cid
 * @param src_addr   source logaddr elevator
 * @param dst_addr   destination logaddr elevator
 * @param req_id     Id of the elevator request, generated by the client. Must be unique
 * @param flags      the first 4 bits contain the elevator manufacturer.
 *                   Other bits remain unused, for further informations
 */
void CSPS_API csp_send_liftcall_req(
        ctp_cid               cid,
        const logaddr_t      *src_addr,
        const logaddr_t      *dst_addr,
        uint16_t	      req_id,
        uint32_t              flags);
        
/**
 * Send response to an elevator call request
 * @param cid        connection cid
 * @param src_addr   source logaddr elevator
 * @param dst_addr   destination logaddr elevator
 * @param req_id     Id of the elevator request
 * @param response   request response @see lift_response
 * @param flags      the first 4 bits contain the elevator manufacturer.
 *                   Other bits remain unused, for further informations
 */
void CSPS_API csp_send_liftcall_res(
        ctp_cid               cid,
        const logaddr_t      *src_addr,
        const logaddr_t      *dst_addr,
        uint16_t              req_id,
        uint8_t               response,
        uint32_t              flags);
        
/**
 * Send an elevator status request
 * @param cid        connection cid
 * @param req_id     request Id
 * @param flags      the first 4 bits contain the elevator manufacturer.
 *                   Other bits remain unused, for further informations
 */
void CSPS_API csp_send_liftstatus_req(
        ctp_cid               cid,
        uint16_t              req_id,
        uint32_t              flags);
        
/**
 * Send response to an elevator status request
 * @param cid        connection cid
 * @param req_id     request Id
 * @param status     status of the call @see lift_status
 * @param response   request response @see lift_response
 * @param flags      the first 4 bits contain the elevator manufacturer.
 *                   Other bits remain unused, for further informations
 */
void CSPS_API csp_send_liftstatus_res(
        ctp_cid               cid,
        uint16_t              req_id,
        uint8_t               status,
        uint8_t               response,
        uint32_t              flags);

/**
 * Send fire alert
 * @param cid        connection cid
 * @param req_id     request Id
 * @param alert_type type of alert to communicate @see alert_type
 */
void CSPS_API csp_send_fire_alert(ctp_cid cid, uint16_t req_id, uint8_t alert_type);

/**
 * Send set/reset virtual input
 * @param cid        connection cid
 * @param req_id     request Id
 * @param virt_inp_mask     which input
 * @param virt_inp_vals     value to send
 * @param alert_type type of alert to communicate @see alert_type
 */
void CSPS_API csp_send_set_virtinp(ctp_cid cid, uint16_t req_id, uint32_t virt_inp_mask, uint32_t virt_inp_vals);
/// @}



// CSP message reception ===================================================

/// @name CSP message reception
/// @{

/**
 * Get the next valid CSP message received on a CTP connection.
 * Invalid messages are discarded.
 *
 * @param  cid  ID of the CTP connection from which to read the message.
 *              Must correspond to a valid connection.
 * @param  buf  Pointer to the csp_msgbuf to contain the message.
 *              Must be not NULL.
 *
 * @retval  >0        Length of the message received.
 *                    It must be deallocated by the caller.
 * @retval  =0        No messages in queue, or the next message is invalid.
 * @retval  ERR_CONN  The connection identified by CID is not valid or has
 *                    been aborted.
 */
int CSPS_API csp_recv(ctp_cid cid, csp_msgbuf **buf);

/**
 * Get the next valid broadcast (connectionless) CSP message received.
 *
 * @param[out] buf    Pointer to the csp_msgbuf to contain the message.
 *                    Must be not NULL.
 *
 * @retval  >0        Length of the message received.
 *                    It must be deallocated by the caller.
 * @retval  =0        No messages in queue, or the next message is invalid.
 */
int CSPS_API csp_recv_broadcast(csp_msgbuf **buf);

/**
 * Get the next valid broadcast (connectionless) CSP message received with
 * indication of the CTP identity that received it.
 *
 * @param[out] buf            Pointer to the csp_msgbuf to contain the message.
 *                            Must be not NULL.
 * @param[out] ctp_id         Pointer to the CTP identity that received the message.
 *                            Ignored if NULL.
 * @param[out] local_addr     Target logaddr of the received the message,
 *                            if received. Not touched otherwise.
 *                            If not received, *have_einfo is set to false.
 *                            Ignored if NULL.
 * @param[out] peer_addr      Sender logaddr of the received the message,
 *                            if received. See @a local_addr.
 * @param[out] have_addresses Sender and Target logaddr available in Extra Info.
 *                            Ignored if NULL.
 *
 * @retval  >0        Length of the message received.
 *                    It must be deallocated by the caller.
 * @retval  =0        No messages in queue, or the next message is invalid.
 */
int CSPS_API csp_recv_broadcast5(csp_msgbuf **buf, ctp_identity_t **ctp_id,
                                 logaddr_t *local_addr, logaddr_t *peer_addr,
                                 int *have_addresses);
                                 
/**
 * Copies the next valid CSP message received on a CTP connection into a
 * user-supplied buffer.
 * Invalid messages are discarded.
 *
 * This is a variant of csp_recv() that does not use csp_msgbuf.
 * It should be used only for languages that do not support pointers.
 *
 * @param  cid  ID of the CTP connection from which to read the message.
 *              Must correspond to a valid connection.
 * @param  buf  Pointer to a buffer where tha received packet will be copied.
 *              Must be not NULL.
 * @param  maxlen  Size of buf [B]. Bigger messages are truncated.
 *
 * @retval  >0        Length of the message received.
 * @retval  =0        No messages in queue, or the next message is invalid.
 * @retval  ERR_CONN  The connection identified by CID is not valid or has
 *                    been aborted.
 */
int CSPS_API csp_recv_buf(ctp_cid cid, csp_message_t *message, int maxlen);

/**
 * Copies the next valid CSP message received on a CTP connection into a
 * user-supplied buffer.
 * Invalid messages are discarded.
 *
 * This is a variant of csp_recv() that does not use csp_msgbuf.
 * It should be used only for languages that do not support pointers.
 *
 * @param  cid  ID of the CTP connection from which to read the message.
 *              Must correspond to a valid connection.
 * @param  buf  Pointer to a buffer where tha received packet will be copied.
 *              Must be not NULL.
 * @param  maxlen  Size of buf [B]. Bigger messages are truncated.
 *
 * @retval  >0        Length of the message received.
 * @retval  =0        No messages in queue, or the next message is invalid.
 */
int CSPS_API csp_recv_broadcast_buf(csp_message_t *message, int maxlen);

/// @}


// CSP message decoding ====================================================

/// @name CSP message decoding
/// @{

/**
 * Return the message type field of a CSP message.
 */
CSPS_INLINE uint8_t csp_get_msgtype(const csp_msgbuf *buf)
{
	const csp_header_t *hdr;
	CSPS_ASSERT(csp_msgbuf_isvalid(buf));
	hdr = (const csp_header_t *) buf->dat;
	return (hdr->msg);
}

CSPS_INLINE uint8_t csp_mediareq_action(const csp_msgbuf *buf)
{
	const csp_msg_mediareq *msg;
	CSPS_ASSERT(csp_msgbuf_isvalid(buf));
	msg = (csp_msg_mediareq *) buf->dat;
	return (msg->action_channel & MEDIAREQ_ACTION_MASK);
}

CSPS_INLINE uint8_t csp_mediareq_channel(const csp_msgbuf *buf)
{
	const csp_msg_mediareq *msg;
	CSPS_ASSERT(csp_msgbuf_isvalid(buf));
	msg = (csp_msg_mediareq *) buf->dat;
	return (msg->action_channel & MEDIAREQ_CHANNEL_MASK);
}

CSPS_INLINE uint8_t csp_mediaoffer_action(const csp_msgbuf *buf)
{
	const csp_msg_mediaoffer *msg;
	CSPS_ASSERT(csp_msgbuf_isvalid(buf));
	msg = (csp_msg_mediaoffer *) buf->dat;
	return (msg->action_channel & MEDIAOFFER_ACTION_MASK);
}

CSPS_INLINE uint8_t csp_mediaoffer_channel(const csp_msgbuf *buf)
{
	const csp_msg_mediaoffer *msg;
	CSPS_ASSERT(csp_msgbuf_isvalid(buf));
	msg = (csp_msg_mediaoffer *) buf->dat;
	return (msg->action_channel & MEDIAOFFER_CHANNEL_MASK);
}

/// True if the packet has a Call ID field
CSPS_INLINE int csp_has_callid(const csp_msgbuf *buf)
{
	CSPS_ASSERT(csp_msgbuf_isvalid(buf));
	return (csp_get_msgtype(buf) == MSG_START && buf->len >= 28);
}

/// True if the packet has a Forwarder Role field
CSPS_INLINE int csp_has_forwarder_role(const csp_msgbuf *buf)
{
	CSPS_ASSERT(csp_msgbuf_isvalid(buf));
	return (csp_get_msgtype(buf) == MSG_START && buf->len >= 40);
}

/// True if the packet has a Divert Target field
CSPS_INLINE int csp_has_divert_target(const csp_msgbuf *buf)
{
	CSPS_ASSERT(csp_msgbuf_isvalid(buf));
	return (csp_get_msgtype(buf) == MSG_RELEASE && buf->len >= 14);
}

/// True if the packet has a Maximum RTP Packet Size field
CSPS_INLINE int csp_has_rtp_mtu(const csp_msgbuf *buf)
{
	CSPS_ASSERT(csp_msgbuf_isvalid(buf));
	return (csp_get_msgtype(buf) == MSG_MEDIAREQ && buf->len >= 12);
}

/// True if the packet has a Preferred Bitrate field
CSPS_INLINE int csp_has_pref_bitrate(const csp_msgbuf *buf)
{
	CSPS_ASSERT(csp_msgbuf_isvalid(buf));
	return (csp_get_msgtype(buf) == MSG_MEDIAREQ && buf->len >= 16);
}

/// True if the packet has a flags field
CSPS_INLINE int csp_has_flags(const csp_msgbuf *buf)
{
	CSPS_ASSERT(csp_msgbuf_isvalid(buf));
	return (csp_get_msgtype(buf) == MSG_REGISTER_REQ && buf->len >= 17);
}

/// True if the packet has a Flags 2 field
CSPS_INLINE int csp_has_flags2(const csp_msgbuf *buf)
{
	CSPS_ASSERT(csp_msgbuf_isvalid(buf));
	return (csp_get_msgtype(buf) == MSG_START && buf->len >= 41);
}

/// True if the packet has the Multistream fields
CSPS_INLINE int csp_has_multistream(const csp_msgbuf *buf)
{
	CSPS_ASSERT(csp_msgbuf_isvalid(buf));
	return ((csp_get_msgtype(buf) == MSG_MEDIAREQ) && (buf->len >= 26));
}

CSPS_INLINE uint8_t csp_mediareq_flowid(const csp_msgbuf *buf)
{
	const csp_msg_mediareq *msg;
	CSPS_ASSERT(csp_msgbuf_isvalid(buf));
	msg = (csp_msg_mediareq *) buf->dat;
	return (csp_has_multistream(buf) ? msg->flow_id : MEDIAREQ_FLOWID_DEFAULT);
}

/// @}



#ifdef __cplusplus
}
#endif

#endif // CSP_H
