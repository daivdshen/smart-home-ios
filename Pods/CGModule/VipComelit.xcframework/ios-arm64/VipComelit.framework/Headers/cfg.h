/**
 *
 * @file cfg.h
 *
 * Definizione della struttura condivisa che gestisce i dati associati ad
 * una elemento ViP.
 *
 * @author Eros Magri
 *
 ***************************************************************************/

#ifndef CFG_H
#define CFG_H

#include <arpa/inet.h>

#include <cspstack/logaddr.h>

#define MAX_SUBUNITS            16          ///< Max subunit registered to master.
#define MAX_REPLICAST_TARGETS   6           ///< Max target video stream replicast
#define CSP_SUBUNIT_PORT        24102       ///< Porta su cui resta in ascolto la subunit "staffa"
#define PI_UDP_AUDIO_PORT       24108       ///< Porta per la ricezione del flusso RTP Audio in ingresso
#define PI_UDP_VIDEO_PORT       24104       ///< Porta per la ricezione del flusso RTP Video in ingresso
#define RTP_MAX_MTU_SIZE        (2060)      ///< Max recv len on rtp video sockets

#if defined(ANDROID) || defined(IOS) || defined(MACOS)
typedef unsigned long ulong;
#endif

/**
 * Identificazione dei canali. Usati per comodità per le chiamate a una
 * stessa procedura che può applicarsi sia all'audio che al video.
 */
typedef enum {
    AUDIO,
    VIDEO,
    VIDEO2,
    NCHANNELS
} channel_t;

typedef enum {
    DIR_TX,
    DIR_RX
} direction_t;

///< contains main parameters for a frame captured (live or layback)
typedef struct {
    unsigned int width;              ///< frame's width[pixels]
    unsigned int height;             ///< frame's height [pixels]
    uint32_t     ts;                 ///< frame's timestamp
    uint32_t     flags;              ///< @see VideoPacketRx flags field
} frameInfo;

/// Call log parameters
typedef struct {
    bool        enabled;             ///< true if calllog enabled
    char*       ipAddr;              ///< calllog server ip address
    int         port;                ///< calllog port
} callLog;

/// Vm server parameters
typedef struct {
    bool        enabled;                        ///< true if vm server is enabled
    char*       ipAddr;                         ///< vm server ip address
    char        vipAddr[LOGADDR_NDIGITS + 1];   ///< vip address of sender
    int         port;                           ///< vm server port
} vmServer_t;

typedef struct {

    struct in_addr  ip_addr;                ///< Ultimo indirizzo IP valido (align 4!)
    struct in_addr  ip_netmask;             ///< Netmask (ignorata se si usa AUTOIP) (align 4!)
    int             broadcastLimited;       ///< Se il dispositivo deve inviare trasmissioni dirette o limitate
    ulong           rtpAduMaxBytesSize;     ///< Allow setup max rtp adu size (bytes)
    callLog         calllog;                ///< calllog parameters
    int             runTimer;               ///< sleep time on run loop
    vmServer_t      vmserver;               ///< vm server parameters

} cfg_t;



#endif // CFG_H
