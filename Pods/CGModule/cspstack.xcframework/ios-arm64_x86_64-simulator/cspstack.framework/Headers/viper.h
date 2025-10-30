/**
 *
 * @file viper.h
 *
 * ViPER (the ViP ExtendeR protocol).
 *
 * Copyright (C) 2015 Comelit Group SpA
 *
 ***************************************************************************/

#ifndef VIPER_H
#define VIPER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <unistd.h>
#include <stdbool.h>
#ifdef __MINGW32__
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#include <cspstack/logaddr.h>

// TODO esportare ALL'APPLICATIVO le informazioni per calcolare RTP_MAX_MTU
// TODO permettere di aggiungere campi ad una MEDIAREQ in un csp_msgbuf (per aggiungere max_mtu dove manca)


// GENERAL DEFINITIONS =====================================================

typedef uint32_t fourcc_do_not_use;

// For internal use only
#define VIPER_FOURCC_BUILD(a,b,c,d) (    \
	(((uint32_t)(a))      ) | \
	(((uint32_t)(b)) <<  8) | \
	(((uint32_t)(c)) << 16) | \
	(((uint32_t)(d)) << 24) )

/**
 * Generate a 32-bit variable containing a FOURCC.
 * Example: VIPER_FOURCC('Abc7') generates in memory
 * the bytes 0x41 0x62 0x63 0x37.
 */
#define VIPER_FOURCC(x) VIPER_FOURCC_BUILD((x)[0], (x)[1], (x)[2], (x)[3])

// CHANNEL =================================================================

typedef uint16_t viper_channel_id; // TODO union con bitfield? O macro per accedere ai campi?
typedef uint8_t  viper_chunk_flags;

typedef enum
{
	VIPER_CHANNEL_OVER_TCP,
	VIPER_CHANNEL_OVER_UDP,
	VIPER_CHANNEL_OVER_SECURE_TCP,
	VIPER_CHANNEL_N_TRANSPORTS
} viper_channel_transport;

typedef enum
{
	VIPER_OK                            = 0x00,
	VIPER_ERROR_PACKET_TOO_SHORT        = 0x01,
	VIPER_ERROR_CHANNEL_EXISTS          = 0x02,
	VIPER_ERROR_MALFORMED_PACKET        = 0x03,
	VIPER_ERROR_INTERNAL                = 0x04, // e.g. out of memory
	VIPER_ERROR_NOT_IMPLEMENTED         = 0x05,
	VIPER_ERROR_CHANNEL_ALREADY_OPEN    = 0x06,
	VIPER_ERROR_NO_SUCH_CHANNEL         = 0x07,
	VIPER_ERROR_FORBIDDEN               = 0x08,
	VIPER_ERROR_UNKNOWN                 = 0xfe
} viper_error;

/// A ViPER Channel object.
typedef struct viper_channel_str viper_channel;

/**
 * Callback called by a channel when a chunk is received.
 *
 * @param ch         Channel object on which the chunk was received.
 * @param user_data  User argument for callbacks. When a callback is
 *                   called, this parameter will be passed back unmodified.
 * @param chunk_data Pointer to the received data.
 *                   The buffer is owned by the caller.
 *                   It MUST be copied by the callee.
 * @param chunk_len  Length of the data received.
 *
 * @return The called must return 0 if the packet reception raised no critical
 *         errors.
 *         If an error that is fatal for the tunnel happened, it must return -1
 *         and set errno accordingly. Note: if -1 is returned, the tunnel will
 *         be closed immediately: use only when really needed!
 */
typedef int (*viper_channel_chunk_received)(viper_channel *ch,
                                            void *user_data,
                                            void *chunk_data, int chunk_len);
                                            
/// Channel ops: callbacks fired from a channel.
typedef struct
{
	viper_channel_chunk_received           chunk_received;
} viper_channel_ops;

#define VIPER_CHANNEL_OPS_INITIALIZER {NULL}

/**
 * Return the Channel ID.
 * @param[in]  ch  The channel. Not NULL.
 */
viper_channel_id viper_channel_get_id(const viper_channel *ch);

/**
 * Return the Channel Transport.
 * @param[in]  ch  The channel. Not NULL.
 */
viper_channel_transport viper_channel_get_transport(const viper_channel *ch);

/**
 * Set channel ops (callbacks).
 * The passed info is copied in the viper_channel object.
 *
 * @param[in]  ch       The channel whose ops are to be set.
 * @param[in]  ops      Pointer to the structure describing the ops.
 *                      All unimplemented ops MUST be set to NULL.
 *                      @a ops must not be NULL.
 *                      @note MUST be initialized to
 *                      VIPER_CHANNEL_OPS_INITIALIZER before being set!
 * @param[in]  ops_size Size of @a ops in bytes.
 *                      MUST be a multiple of sizeof(void*).
 * @return On success, 0. On error, -1 and errno is set appropriately.
 */
int viper_channel_set_ops(viper_channel *ch,
                          const viper_channel_ops *ops, size_t ops_size);
                          
/**
 * Set user data for channel callbacks.
 * The passed info is copied in the viper_channel object.
 *
 * @param[in]  ch         The channel whose user data is to be set.
 * @param[in]  user_data  User argument for callbacks. When a callback is
 *                        called, this parameter will be passed back unmodified.
 * @return On success, 0. On error, -1 and errno is set appropriately.
 */
int viper_channel_set_user_data(viper_channel *ch, void *user_data);

/**
 * Send a chunk over a channel.
 *
 * @param ch           Channel where to send the chunk.
 * @param payload_buf  Payload to send.
 *                     The caller is in charge of (de)allocating this memory.
 * @param payload_len  Payload length. Allowed values are 1 to 65535.
 * @param chunk_flags  Flags to be set in the chunk.
 *                     Currently none implemented.
 * @param send_flags   Flags that define how to send the chunk. Possible values:
 *                     VIPER_SEND_FLAG_FORCED_DATAGRAM.
 * @return On success, 0. On error, -1 and errno is set appropriately.
 */
int viper_channel_send(viper_channel *ch,
                       const void *payload_buf, size_t payload_len,
                       viper_chunk_flags chunk_flags, int send_flags);
                       
// TODO docs
int viper_channel_close(viper_channel *ch);

// TODO docs
/// @note MUST call viper_channel_close() before!
int viper_channel_free(viper_channel *ch);

// UDP LANE - FORWARD DECLARATIONS =========================================

typedef enum
{
	UDPLANE_OFF,           ///< UDP Lane not open, no socket, no control channel
	UDPLANE_WAIT_PROBE1,   ///< Waiting for 1st incoming probe (server only, for binding socket)
	UDPLANE_WAIT_PEER_ACK, ///< Socket bound, waiting for peer to confirm rx from us
	UDPLANE_ACTIVE         ///< Active: peer confirms reception from us
} udplane_status;


// TUNNEL  =================================================================

/// A unique integer that identifies a tunnel.
typedef uint16_t viper_tunnel_uid;

/// Special Tunnel UID meaning "unknown UID".
static const viper_tunnel_uid viper_tunnel_uid_unknown = 0xFFFF;

/// A ViPER Tunnel object.
typedef struct viper_tunnel_str  viper_tunnel;

/// Identifier of a ViPER Service type.
typedef fourcc_do_not_use viper_service_id;

/// Called by a ViPER Tunnel to notify the peer requested to open a channel.
// TODO rename con prefisso viper_tunnel
// TODO documentare che chi riceve la callback deve settare ops (e user_data opzionalmente)
/// @return 0 to accept the channel, or a ViPER error code (@see viper_error)
///         to reject it. Note: returning -1 is deprecated, and currently
///         remapped to VIPER_ERROR_UNKNOWN.
typedef int (*viper_new_incoming_channel)(viper_tunnel *tun,
                                          void *user_data,
                                          viper_channel *new_ch,
                                          viper_service_id svc,
                                          const void *svc_data,
                                          size_t svc_data_size);
                                          
/// Callback called by a tunnel after one if its channels has been closed.
/// The channel is not operational anymore and must be freed using
/// viper_channel_free().
/// It is safe to invoke viper_channel_free() from within this callback.
typedef int (*viper_tunnel_channel_closed)(viper_tunnel *tun, void *tun_user_data,
                                           viper_channel *closed_ch, void *ch_user_data);
                                           
/// Callback called by a tunnel after it has been closed.
/// The tunnel is not operational anymore and must be freed using
/// viper_channel_free().
typedef int (*viper_tunnel_closed)(viper_tunnel *tun,
                                   void *user_data);
                                   
// TODO docs
typedef int (*viper_tunnel_udplane_status_changed)(viper_tunnel *tun,
                                                   void *user_data,
                                                   udplane_status old_status,
                                                   udplane_status new_status);
                                                   
/**
 * Called by a ViPER tunnel when a response to a channel open request has been
 * received from the other end.
 * @param tun           The tunnel handling the channel
 * @param tun_user_data User data (@see viper_tunnel_set_user_data())
 * @param ch            The channel that is being opened
 * @param error_code    VIPER_OK confirms successful channel opening; other
 *                      values denote an error.
 * @return You must return zero.
 */
typedef int (*viper_tunnel_channel_open_res)(viper_tunnel  *tun,
                                             void          *tun_user_data,
                                             viper_channel *ch,
                                             viper_error    error_code);
                                             
/// Flag data to be sent preferably as datagram (UDP or UDP-like).
/// If the UDP lane is inactive, the packet will be sent to stream.
#define VIPER_SEND_FLAG_DATAGRAM           0x01
/// Flag data to be sent as stream (TCP or TCP-like)
#define VIPER_SEND_FLAG_STREAM             0x02
/// Flag data to be sent unconditionally as datagram (UDP or UDP-like).
/// If the UDP lane is inactive, the packet will be dropped.
#define VIPER_SEND_FLAG_FORCED_DATAGRAM    0x04

/**
 * Request to send data on behalf of a ViPER tunnel.
 * Called by a ViPER tunnel configured for tap transmission in order to send
 * data.
 *
 * @param tun           The tunnel object
 * @param tun_user_data User data (@see viper_tunnel_set_user_data())
 * @param buf           Data to send
 * @param len           Size of data to send
 * @param send_flags    Flags affecting how data should be sent.
 *                      Possible values:
 *                      - VIPER_SEND_FLAG_DATAGRAM
 *                      - VIPER_SEND_FLAG_STREAM
 *
 * @return 0 if packet transmission raised no critical problems.
 *         If an error that is fatal for the tunnel happened, it must return -1
 *         and set errno accordingly. Note: if -1 is returned, the tunnel might
 *         be closed immediately: use only when really needed!
 */
typedef int (*viper_tunnel_send_on_tap)(viper_tunnel  *tun,
                                        void          *tun_user_data,
                                        const void    *buf,
                                        int            len,
                                        uint32_t       send_flags);
                                        
/**
 * User callback called when viper tunnel run loop is idle
 * (no processing of UDP or TCP packets and events).
 * The callback is not called during normal operations, it should be triggered
 * using the viper_tunnel_trigger_out_of_run_op() function.
 * When triggered the callback is called once before re-entering normal mode.
 *
 * @param tun The tunnel object
 * @param tun_user_data User data (@see viper_tunnel_set_user_data())
 */
typedef void (*viper_tunnel_out_of_run)(viper_tunnel  *tun,
                                        void          *tun_user_data);
                                        
typedef enum
{
	TUNNEL_CB_NEW_INCOMING_CHANNEL = 1,
	TUNNEL_CB_CHANNEL_CLOSED,
	TUNNEL_CB_CLOSED,
	TUNNEL_CB_UDPLANE_STATUS_CHANGED,
	TUNNEL_CB_CHANNEL_OPEN_RES,
	TUNNEL_CB_SEND_ON_TAP,
	TUNNEL_CB_OUT_OF_RUN,
} viper_tunnel_op_index;

/// Tunnel ops: callbacks fired from a channel.
typedef struct
{
	viper_new_incoming_channel            new_incoming_channel;
	viper_tunnel_channel_closed           channel_closed;
	viper_tunnel_closed                   closed;
	viper_tunnel_udplane_status_changed   udplane_status_changed;
	viper_tunnel_channel_open_res         channel_open_res;
	viper_tunnel_send_on_tap              send_on_tap;
	viper_tunnel_out_of_run               out_of_run;
} viper_tunnel_ops;

#define VIPER_TUNNEL_OPS_INITIALIZER {NULL, NULL, NULL, NULL, NULL, NULL, NULL}

/**
 * Create a new ViPER Tunnel.
 * @return On success, the newly created Tunnel object.
 *         On error, NULL and errno is set appropriately.
 */
viper_tunnel *viper_tunnel_new();

/**
 * Set ops (callbacks) for a tunnel.
 * The passed info is copied in the viper_tunnel object.
 * @note new_incoming_channel MUST be implemented.
 *
 * @param[in]  tun      The tunnel whose ops are to be set.
 * @param[in]  ops      Pointer to the structure describing the ops.
 *                      All unimplemented ops MUST be set to NULL.
 *                      @a ops must not be NULL.
 *                      @note MUST be initialized to
 *                      VIPER_TUNNEL_OPS_INITIALIZER before being set!
 * @param[in]  ops_size Size of @a ops in bytes.
 *                      MUST be a multiple of sizeof(void*).
 * @return On success, 0. On error, -1 and errno is set appropriately.
 */
int viper_tunnel_set_ops(viper_tunnel *tun,
                         const viper_tunnel_ops *ops, size_t ops_size);
                         
int viper_tunnel_set_op(viper_tunnel *tun, viper_tunnel_op_index index,
                        const void *op);
                        
/**
 * Set the user data for the tap callbacks.
 * @param[in]  tun        The tunnel whose user data are to be set.
 * @param[in]  user_data  User argument for the callback. When a callback is
 *                        called, this parameter will be passed back unmodified.
 * @return On success, 0. On error, -1 and errno is set appropriately.
 */
int viper_tunnel_set_user_data(viper_tunnel *tun, void *user_data);

/**
 * Return the tunnel identifier.
 * This ID is unique unless an uint16_t overflow occurred.
 * @note Can be called only on the server!
 *
 * @param tun  The tunnel object
 * @return The Tunnel UID.
 */
viper_tunnel_uid viper_tunnel_get_uid(viper_tunnel *tun);

// TODO doc
int viper_tunnel_get_tcp_socket(const viper_tunnel *tun);

/**
 * Return the number of channels that currently exist in the tunnel object.
 * For informational purposes only.
 *
 * @param tun  The tunnel object
 * @return The number of channels currently handled by the tunnel, including
 *         closed channels. The number decreases after freeing the channel.
 */
int viper_tunnel_get_channel_count(const viper_tunnel *tun);

/**
 * Tell if the TCP socket is currently connected. If the socket is on tap
 * the connection is supposed to be secure (tcp or tcp-like)
 *
 * @param[in]  tun     The tunnel object
 * @param[out] errval  Pointer to the current socket error status.
 *                     If the return value is false, the current socket
 *                     error is written in *errval. If NULL, ignored.
 * @return true if the TCP socket is connected or the connection is on tap,
 *         false if the socket has not been opened or has been closed.
 */
bool viper_tunnel_is_connected(const viper_tunnel *tun, int *errval);

/**
 * Connect a (client) ViPER Tunnel to a ViPER Server.
 * @param tun         The client tunnel.
 * @param tcp_addr    The ViPER Server TCP address.
 *                    @see https://www.gnu.org/software/libc/manual/html_node/Address-Formats.html#Address-Formats
 *                    @see https://www.gnu.org/software/libc/manual/html_node/Internet-Address-Formats.html
 * @param tcp_addrlen The length of @a tcp_addr.
 * @param udp_addr    The ViPER Server UDP address.
 * @param udp_addrlen The length of @a udp_addr.
 * @param timeout     Time to wait before giving up to connect to TCP [s].
 *                    If 0, the default operating system timeout is used.
 * @return On success, 0. On error, -1 and errno is set appropriately.
 */
int viper_tunnel_connect2(viper_tunnel *tun,
                          const struct sockaddr *tcp_addr, socklen_t tcp_addrlen,
                          const struct sockaddr *udp_addr, socklen_t udp_addrlen,
                          int timeout);
                          
/**
 * Connect a (client) ViPER Tunnel to a ViPER Server (IPv4 only).
 *
 * This is equivalent to viper_tunnel_connect2(), but only accepts IPv4
 * addresses. Don't use this function.
 */
int viper_tunnel_connect(viper_tunnel *tun,
                         const struct sockaddr_in *tcp_addr, socklen_t tcp_addrlen,
                         const struct sockaddr_in *udp_addr, socklen_t udp_addrlen,
                         int timeout);
                         
/**
 * Mark a (client) tunnel as connected on tap.
 *
 * This does not perform a real connection. Connections over the tap is up to
 * the user.
 *
 * @param tun The client tunnel.
 */
void viper_tunnel_connect_tap(viper_tunnel *tun);

/**
 * Close a ViPER Tunnel.
 * @param tun  The tunnel object to close.
 *             This object will not be usable anymore, but still allocated.
 *             It must then be freed calling viper_tuinnel_free().
 * @return On success, 0. On error, -1 and errno is set appropriately.
 */
int viper_tunnel_close(viper_tunnel *tun);

// TODO docs
/// @note MUST call viper_tunnel_close() before!
int viper_tunnel_free(viper_tunnel *tun);

/**
 * Inject into the tunnel data coming from the TCP socket.
 * This is the data received from the other endpoint of the connection.
 *
 * All the useful data are copied in internal structures. The content of
 * @a data can be freed or reused by the caller after this function returns.
 *
 * @param tun      The tunnel object.
 * @param data     The data received on the socket, that the tunnel must handle.
 * @param data_len Data length.
 * @return On success, 0. On error, -1 and errno is set appropriately.
 */
int viper_tunnel_inject_tcp(viper_tunnel *tun, const void *data, size_t data_len);

// TODO docs
int viper_tunnel_inject_udp(viper_tunnel *tun, const void *data, size_t data_len);

// TODO docs
typedef enum
{
	TUNNEL_TCP_SHUTDOWN,      ///< The TCP peer has performed an orderly shutdown
	TUNNEL_TCP_ERROR,         ///< An error occurred on the TCP connection
	TUNNEL_VIPER_PROTO_ERROR, ///< Protocol error at the ViPER layer
	TUNNEL_N_EVENTS
} tunnel_event;

// TODO docs
// @note if event == TCP _SHUTDOWN or TCP_ERROR, tcp_errno MUST be != 0
int viper_tunnel_inject_tcp_event(viper_tunnel *tun, tunnel_event ev,
                                  int tcp_errno);
                                  
// TODO viper_tunnel_inject_udp_event()?

/**
 * Tunnel main loop.
 * After initializing and connecting the tunnel, this function must be called
 * for the tunnel to operate. This implements an infinite loop that exits only
 * in case of disconnection or unrecoverable errors or when explicitly
 * requested (@see viper_tunnel_suspend_run). Generally viper_tunnel_run()
 * should be called within a thread.
 *
 * @param tun  The Tunnel object.
 *
 * @retval  0  When the user requested to exit and suspend processing events
 *             (@see viper_tunnel_suspend_run()). In this case the tunnel state
 *             is left untouched, thus viper_tunnel_run() can be called
 *             immediately after to immediately resume processing.
 * @retval -1  On error, and errno is set appropriately.
 */
int viper_tunnel_run(viper_tunnel *tun);

/**
 * Force viper_tunnel_run() to exit, suspending event handling.
 *
 * Allows to call any method on the tunnel without waiting for
 * viper_tunnel_run() to exit.
 *
 * @see viper_tunnel_run() for more details.
 *
 * @return On success, 0. On error, -1 and errno is set appropriately.
 */
int viper_tunnel_suspend_run(viper_tunnel *tun);

/**
 * Force viper_tunnel_run() to call a user callback.
 *
 * @see viper_tunnel_run() and viper_tunnel_out_of_run for more details.
 *
 * @return On success, 0. On error, -1 and errno is set appropriately.
 */
int viper_tunnel_trigger_out_of_run_op(viper_tunnel *tun);

/**
 * Create a new Channel over an established Tunnel, the channel should be opened
 * before using it (@see viper_tunnel_channel_open()).
 *
 * @param tun        The established Tunnel on which the Channel is created.
 * @param service_id ID of the service to be requested to the remote end.
 * @param pref_tr    The preferred underlying transport medium.
 * @return On success, Channel object created.
 *         On error, NULL and errno is set appropriately.
 */
viper_channel *viper_tunnel_channel_create(viper_tunnel            *tun,
                                           viper_service_id         service_id,
                                           viper_channel_transport  pref_tr);
                                           
/**
 * Request opening of an already created Channel over an established Tunnel.
 * The channel is not actually open until an acknowledgement is received from
 * the other side.
 *
 * @param tun      The established Tunnel on which the Channel is to be opened.
 * @param ch       Channel object to open.
 * @param svc_info Pointer to custom service additional info. In general no additional
 *                 info should be provided only ctp channel foresees the list of
 *                 handled logaddrs (@see viper_tunnel_open_ctp_channel())
 * @param svc_info_size  size of @a svc_info struct
 * @return On success, true.
 *         On error, false and errno is set appropriately.
 */
bool viper_tunnel_channel_open(viper_tunnel  *tun,
                               viper_channel *new_ch,
                               const void    *svc_info,
                               size_t         svc_info_size);
                               
/**
 * Creates a new Channel and request the opening over an established Tunnel.
 * The channel is not actually open until an acknowledgement is received from
 * the other side.
 *
 * @param tun      The established Tunnel on which the Channel is to be opened.
 * @param svc      ID of the service to be requested to the remote end.
 * @param pref_tr  The preferred underlying transport medium.
 * @return On success, the newly created channel.
 *         On error, NULL and errno is set appropriately.
 */
// TODO notificare con callback se il canale viene chiuso o non viene aperto
viper_channel *viper_tunnel_open_channel(viper_tunnel            *tun,
                                         viper_service_id         service_id,
                                         viper_channel_transport  pref_tr);
                                         
// TODO aggiornare docs
viper_channel *viper_tunnel_open_ctp_channel(viper_tunnel            *tun,
                                             viper_channel_transport  pref_tr,
                                             const logaddr_t          addr[],
                                             int                      naddr);
                                             
/**
 * Return the channel with a given Channel ID.
 * @param tun  The tunnel that owns the desired channel.
 * @param id   The Channel ID to search.
 * @return The channel, or NULL if not found.
 */
viper_channel *viper_tunnel_get_channel_with_id(const viper_tunnel *tun,
                                                viper_channel_id    id);
                                                
                                                
// UDP LANE ================================================================

// TODO documentare viper_tunnel_udplane_*
int viper_tunnel_udplane_open(viper_tunnel *tun);
int viper_tunnel_udplane_close(viper_tunnel *tun);
int viper_tunnel_udplane_tick(viper_tunnel *tun, int nticks_100ms);
int viper_tunnel_udplane_get_socket(const viper_tunnel *tun);
udplane_status viper_tunnel_udplane_get_status(const viper_tunnel *tun);


// SERVER ==================================================================
// The server functions are available only if VIPER_SERVER is defined

typedef struct viper_server_str viper_server;

/**
 * Create a new ViPER Server.
 * @return On success, the newly created Server object.
 *         On error, NULL and errno is set appropriately.
 */
viper_server *viper_server_new();

/**
 * Create a new incoming ViPER tunnel on tap, set the callback for
 * sending data on tap and add it to tunnel list.
 *
 * @param srv        The ViPER Server object
 * @param cbk        ViPER tunnel send callback
 * @param user_data  User argument for callbacks. When a callback is
 *                   called, this parameter will be passed back unmodified.
 * @return the pointer to the ViPER tunnel created
 */
viper_tunnel *viper_server_create_incoming_tunnel_tap(viper_server *srv,
                                                      viper_tunnel_send_on_tap cbk,
                                                      void *user_data);
                                                      
/**
 * Prepares the Server to listen for new connections.
 * Creates a TCP socket, binds it to @a tcp_listen_addr and sets it up for
 * listening (i.e. socket(), bind(), listen()).
 * You then need to call viper_server_accept() for the server to actually
 * accept incoming connections.
 *
 * @param srv                  The ViPER Server object
 * @param tcp_listen_addr      The listen address for accepting TCP connections
 * @param tcp_listen_addr_len  The @a tcp_listen_addr lenght
 * @param udp_listen_addr      The listen address for accepting UDP connections
 * @param udp_listen_addr_len  The @a udp_listen_addr lenght
 * @return On success, 0. On error, -1 and errno is set appropriately.
 */
int viper_server_start(viper_server *srv,
                       const struct sockaddr *tcp_listen_addr,
                       socklen_t             tcp_listen_addr_len,
                       const struct sockaddr *udp_listen_addr,
                       socklen_t              udp_listen_addr_len);
                       
/**
 * Handle incoming server events and accept connections from clients.
 * After preparing the server with viper_server_start(), this function must be
 * called for the server to operate. This implements an infinite loop that
 * exits only if a new connection is correctly accepted or an unrecoverable
 * error occurred or when explicitly requested (@see viper_server_suspend_run).
 * Generally this should be called within a thread such that after it returns
 * a newly established tunnel it is immediately set to accept another one.
 *
 * @param      srv      The ViPER Server object
 * @param[out] new_tun  The newly created ViPER Tunnel object.
 * @retval 0  On new connection and @a *new_tun points to the newly established tunnel
 *            or when the user requested to exit (@see viper_server_suspend_run()) and *new_tun is NULL.
 * @retval -1 On error, and errno is set appropriately
 */
int viper_server_run(viper_server *srv, viper_tunnel **new_tun);

/**
 * Force viper_server_run() to exit, suspending event handling.
 *
 * Allows to call any method on the server without waiting for
 * viper_server_run() to exit.
 *
 * @see viper_server_run() for more details.
 *
 * @return On success, 0. On error, -1 and errno is set appropriately.
 */
int viper_server_suspend_run(viper_server *srv);

/**
 * Stop listening for new connections.
 * Close all listening sockets, but do not free the ViPER Server object.
 * No new connections can be accepted and viper_server_accept() must not be
 * called anymore.
 * After calling this function, call viper_server_delete() to free the ViPER
 * Server object.
 *
 * @param  srv  The ViPER Server object
 * @return On success, 0. On error, -1 and errno is set appropriately.
 */
int viper_server_stop(viper_server *srv);

/**
 * Free the VipER Server object (and stop listening).
 * Free the memory used by the ViPER Server object. Also stop listening for
 * new connections (calling viper_server_stop()) if not done yet.
 *
 * @param  srv  The ViPER Server object.
 *              This becomes invalid and must not be used anymore.
 * @return On success, 0. On error, -1 and errno is set appropriately.
 */
void viper_server_delete(viper_server *srv);

/// Return the number of tunnels owned by this server
int viper_server_get_tunnel_count(viper_server *srv);

/// Find a tunnel based on the Unique ID
viper_tunnel *viper_server_find_tunnel_by_uid(viper_server *srv,
                                              viper_tunnel_uid target_uid);
                                              
#ifdef __cplusplus
}
#endif

#endif // VIPER_H
