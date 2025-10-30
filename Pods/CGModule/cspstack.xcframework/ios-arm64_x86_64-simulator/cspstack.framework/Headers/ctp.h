/**
 *
 * @file ctp.h
 *
 * Comelit Transport Protocol.
 *
 * Allows to create reliable datagram transport connections.
 *
 * Relies on the CRP module to resolve addresses.
 *
 * The connection status is maintained internally. In all user interactions
 * the connection is identified by the opaque CTP Connection ID (CID).
 *
 * Initialization and runtime:
 * - Before any usage, CTP must have been initialized by cspstack_init().
 * - During usage call cspstack_run() and cspstack_timer() to let CTP work.
 *
 * Identities:
 * - Create a CTP identity with ctp_identity_new()
 * - Configure it with ctp_identity_setopt(), ctp_identity_bind() and companion
 *   functions
 * - Activate it with ctp_identity_activate()
 * - Destroy the identity with ctp_remove().
 *
 * Connections:
 * - Call ctp_new_connection_out() to create a new outgoing connection
 * - Send messages calling ctp_write() and ctp_write_broadcast()
 * - Call ctp_read() to read incoming messages and be notified about
 *   connection shutdown
 * - Call ctp_close() to close a connection and deallocate the associated
 *   resources.
 *
 * Copyright (C) 2009-2013 Comelit Group SpA
 *
 ***************************************************************************/

#ifndef CTP_H
#define CTP_H

#include <stdint.h>
#ifndef _MSC_VER
#include <stdbool.h>
#endif
#include "logaddr.h"
#include "port.h"


#ifdef __cplusplus
extern "C" {
#endif


/// Typical port for broadcast packets
#define CTP_DEFAULT_BROADCAST_PORT    24101

typedef uint16_t ctp_cid; ///< CTP Connection ID

#define CTP_CID_CONNLESS  0x0000  ///< Connectionless pseudo-CID
#define CTP_CID_NONE      0xffff  ///< Not a valid CID

// Error values
#ifndef ERR_CONN
#define ERR_CONN       -8  ///< Not connected
#endif

/**
 * CTP Identity handler.
 * The transport-layer counterpart of an Identity: maintains all the
 * internal data that are needed to handle CTP procedures
 * (e.g. creating connections).
 */
typedef struct ctp_identity_t ctp_identity_t;

/// A user argument for callbacks
typedef void *ctp_cb_arg_t;

/// Callback to notify a new connection related to an identity.
typedef void (CSPS_API *ctp_new_conn_t)(ctp_cid cid, ctp_cb_arg_t);

/// Callback to notify a new connection not related to any identity.
/// It is called if a new connection arrives from the tap AND the Extra Info
/// destination address is not equal to the address of any identity.
/// @note Useful only if CTP TAP support is enabled (CSPS_CTP_TAP_ENABLE is
/// defined).
typedef void (CSPS_API *ctp_tap_new_conn_cb_t)(ctp_cid cid, ctp_cb_arg_t arg);

/**
 * Callback for tap connections to notify of packets to be sent.
 *
 * @note Useful only if CTP TAP support is enabled (CSPS_CTP_TAP_ENABLE is
 * defined).
 *
 * @param cid       ID of the connection that needs to send the packet
 * @param arg       User argument. @see ctp_tap_set_new_conn_cb()
 * @param data      The raw packet to be sent (includes CTP headers).
 *                  Owned and freed by the caller.
 * @param data_size Number of bytes to be sent (size of @data)
 * @param src       Logical address of the original sender
 * @param dst       Logical address of the intended recipient
 */
typedef void (CSPS_API *ctp_tap_send_cb_t)(ctp_cid cid, ctp_cb_arg_t arg,
                                           const void *data, int data_size,
                                           const logaddr_t *src,
                                           const logaddr_t *dst);
                                           
/**
 * Inizializza le strutture interne.
 */
void ctp_init(void);

/**
 * Set the new incoming connection callback for the tap.
 * Only available if CSPS_CTP_TAP_ENABLE is defined.
 *
 * @param cb   Pointer to the function to be called when a new CTP connection
 *             has arrived and it could not be related to any identity.
 * @param arg  User argument for the callback. When a callback is
 *             called, this parameter will be passed back unmodified.
 * @return 0 if OK, <0 otherwise.
 */
int ctp_tap_set_new_conn_cb(ctp_tap_new_conn_cb_t cb, ctp_cb_arg_t arg);

/**
 * Set the send callback for the tap.
 * Only available if CSPS_CTP_TAP_ENABLE is defined.
 *
 * @param cb   Pointer to the function to be called when CTP needs to send
 *             a packet over the tap.
 * @param arg  User argument for the callback. When a callback is
 *             called, this parameter will be passed back unmodified.
 * @return 0 if OK, <0 otherwise.
 */
int ctp_tap_set_send_cb(ctp_tap_send_cb_t cb, ctp_cb_arg_t arg);

/**
 * Create a new CTP identity object.
 *
 * Allocates a new CTP identity and initializes all fields to "empty" values.
 * The new object is not active until proper initialization is performed with
 * suitable methods and then started calling ctp_identity_activate().
 *
 * Call ctp_remove() to deactivate and remove it, even if it has not been
 * activated yet.
 *
 * @return On success, the newly created object.
 *         On error, NULL and errno is set appropriately.
 */
ctp_identity_t *CSPS_API ctp_identity_new(void);

/**
 * Create and bind the main UDP socket.
 *
 * The IP address, netmask and CTP_ID_OPT_SOCKET_BIND_ON_ANY value must have
 * been already set!
 *
 * If the socket can be created but an error occurs in a later step, the
 * socket is destroyed before returning. In other words, the object state is
 * always kept consistent.
 *
 * @param id          The identity to work on.
 * @param local_port  Local UDP port (host byte order)
 *
 * @return On success, 0. On error, -1 and errno is set appropriately.
 */
int CSPS_API ctp_identity_bind(ctp_identity_t *id, int local_port);

/**
 * Create and bind the UDP socket for receiving brodcast messages.
 *
 * You should not call this function if you do not need to receive broadcast
 * messages.
 *
 * The remarks to ctp_identity_bind() apply here as well.
 *
 * @param id  The identity to work on.
 *
 * @return On success, 0. On error, -1 and errno is set appropriately.
 */
int CSPS_API ctp_identity_bind_bcast(ctp_identity_t *id);

/**
 * Activate a previously configured identity.
 *
 * The identity must have been allocated using ctp_identity_new() and
 * initialized using ctp_identity_setopt(), ctp_identity_bind() and companion
 * functions.
 */
int CSPS_API ctp_identity_activate(ctp_identity_t *id);

/**
 * True if an identity is active.
 * An identity is active between successful ctp_identity_activate()
 * [or ctp_listen()] and ctp_remove().
 */
bool CSPS_API ctp_identity_is_active(const ctp_identity_t *id);

/**
 * Creates a new CTP identity and enables CTP (with or without registering a
 * CRP identity).
 *
 * @note Do not use this function for new code. Instead use ctp_identity_new()
 *       and related functions as described in the file documentation.
 *       New features will not be implemented in ctp_listen(), and it might
 *       be deprecated and removed in the future.
 *
 * @param  logaddr      The local (own) Logical Address.
 *                      In normal usage, it must not be NULL.
 *                      If NULL, a CRP identity will NOT be registered,
 *                      the returned CTP identity will NOT be visible via
 *                      regular CRP requests and will NOT be able to start
 *                      connections to a Logical Address: use with care!
 * @param  local_ip     Local IP address (network byte order)
 * @param  local_mask   Local IP netmask (network byte order)
 * @param  local_port   Local UDP port (host byte order)
 * @param  cb           Callback to be called upon incoming connections.
 *                      Must NOT be NULL.
 * @param  cb_arg       User argument for the callback. When the callback is
 *                      called, this parameter will be passed back unmodified.
 *
 * @return  CTP identity handler, or NULL on error.
 */
ctp_identity_t *CSPS_API ctp_listen(const logaddr_t *logaddr,
                                    const struct in_addr *local_ip, const struct in_addr *local_mask,
                                    int local_port, ctp_new_conn_t cb, ctp_cb_arg_t cb_arg);
                                    
/**
 * Removes a CTP identity.
 * This also closes all associated connections.
 *
 * @note The identity resources are not released immediately, but they are kept
 *       until all associated connections have been closed.
 *       Most notably, the sockets associated to the connection are kept open
 *       for some time for the connections to properly close. So it is
 *       impossible to ctp_listen() on the same port immediately.
 *
 * @param  ctp_identity  The CTP identity to remove. It must have been
 *                       returned by ctp_listen() and not already removed.
 *
 * @return 0 if ok, <0 on error
 */
int CSPS_API ctp_remove(ctp_identity_t *ctp_identity);

/**
 * Removes a CTP identity.
 * This also force closes all associated connections.
 * @note Differently by ctp_remove resources are released immediately
 *       and it is possible to ctp_listen() on the same port immediately.
 *       Connections (if any) are closed without send a FIN.
 * @ref #ctp_remove
 * @param  ctp_identity  The CTP identity to remove. It must have been
 *                       returned by ctp_listen() and not already removed.
 * @return 0 if ok, <0 on error
 */
int CSPS_API ctp_remove_forced(ctp_identity_t *ctp_identity);

/**
 * Return the file descriptor of the socket associated with the CTP identity.
 *
 * @param[in]  ctp_identity  The identity whose socket fd must be obtained.
 * @return The type of the return value depends on the underlying IP stack:
 *         - On implementations based on the BSD socket APIs, returns the
 *           socket file descriptor (int). -1 if the socket an error occurred.
 *         - On lwIP-based implementations, returns a struct udp_pcb *.
 *           NULL if the socket an error occurred.
 */
csps_usocket ctp_identity_get_socket(const ctp_identity_t *ctp_identity);

/**
 * List of options that can be manipulated by @a ctp_identity_setopt().
 */
typedef enum
{
	CTP_ID_OPT_CRP_LIMITED_BROADCAST = 1,
	CTP_ID_OPT_CB_ARG,
	CTP_ID_OPT_NEW_CTP_CONN_CB,
	CTP_ID_OPT_LOGADDR,
	CTP_ID_OPT_LOCAL_IPV4_ADDRESS,
	CTP_ID_OPT_LOCAL_IPV4_NETMASK,
	CTP_ID_OPT_SOCKET_BIND_ON_ANY,
	CTP_ID_OPT_IFACE_NAME,
	CTP_ID_OPT_LOCAL_IPV6_ADDRESS,
	CTP_ID_OPT_LOCAL_IPV6_PREFIX_LEN,
	CTP_ID_OPT_NUM
} ctp_identity_opt;

/**
 * Manipulate options for an identity.
 *
 * @param id      The identity to manipulate
 * @param optname The option to change
 * @param optval  The value to be set for the option (the type depends on @a opt)
 * @param optlen  The size of @a optval [bytes]
 *
 * @note Options can be modified only when the identity is not yet active,
 *       except CTP_ID_OPT_CRP_LIMITED_BROADCAST.
 *
 * * @a CTP_ID_OPT_CRP_LIMITED_BROADCAST: configure CRP to send broadcast
 *   messages to the limited broadcast address (255.255.255.255), not to the
 *   directed broadcast address.
 *   @optval must be an int with value 1 to enable limited broadcast mode.
 *   @note Once limited broadcast mode has been enabled, it cannot be disabled.
 *   @note Only available if CRP is enabled (i.e. CSPS_CRP_ENABLE is defined).
 *
 * * @a CTP_ID_OPT_CB_ARG: set the user argument for callbacks.
 *   @optval shall be the user argument, @optlen is ignored.
 *
 * * @a CTP_ID_OPT_NEW_CTP_CONN_CB: set the callback to be called upon incoming
 *   connections. @optval shall be the function pointer, @optlen is ignored.
 *
 * * @a CTP_ID_OPT_LOGADDR: set the logical address.
 *   @optval shall be a pointer to a logaddr_t object.
 *
 * * @a CTP_ID_OPT_LOCAL_IPV4_ADDRESS: set the local IP address.
 *   @optval shall point to a struct in_addr, whose s_addr field contains the
 *   IPv4 address in network byte order.
 *
 * * @a CTP_ID_OPT_LOCAL_IPV4_NETMASK: set the local IP network mask.
 *   @optval shall point to a struct in_addr, whose s_addr field contains the
 *   IPv4 netmask in network byte order.
 *
 * * @a CTP_ID_OPT_SOCKET_BIND_ON_ANY: configure the identity to bind the
 *   socket on the ANY IP address (in6addr_any or " :: ") instead of the local IP address
 *   set with ::CTP_ID_OPT_LOCAL_IPV4_ADDRESS or ::CTP_ID_OPT_LOCAL_IPV6_ADDRESS.
 *   By default it is disabled (i.e. use the "real" IP address), except when
 *   using ctp_listen(), which uses a setting depending on CSPS_CTP_DUAL_SOCKET.
 *   Listening on in6addr_any let incoming call be accepted on both IPv4 and IPv6
 *   network layers.
 *   @optval must point to an int with value 1 to request binding on ANY, 0 to
 *   bind on the address that was passed.
 *   @note Setting this option does still require an IP address to be set, or
 *   sending any packet would be impossible.
 *
 * * @a CTP_ID_OPT_IFACE_NAME: bind sockets to a specific interface
 *
 * * @a CTP_ID_OPT_LOCAL_IPV6_ADDRESS: set the local IP address.
 *   @optval shall point to a struct in_addr6, carrying the
 *   IPv6 address in network byte order.
 *
 * * @a CTP_ID_OPT_LOCAL_IPV6_PREFIX_LEN: set the local IPv6 prefix length.
 *   @optval shall point to an integer, whose value is the IPv6 prefix length
 *   ranging from 1 to 128.
 *
 * @return On success, 0. On error, -1 and errno is set appropriately.
 */
int CSPS_API ctp_identity_setopt(ctp_identity_t *id, ctp_identity_opt optname,
                                 void *optval, size_t optlen);
                                 
/**
 * Create a connection to a specified logaddr.
 *
 * @note Only available if CRP is enabled (CSPS_CRP_ENABLE is defined).
 * @note Cannot be used if the CTP identity passed in @a ctp_id has been
 *       created without a CRP identity registration (@see ctp_listen()).
 *
 * @param  ctp_id   CTP identity handler (returned by ctp_listen()).
 * @param  my_logaddr    Own Logaddr for Extra Info.
 * @param  peer_logaddr  Peer Logaddr to which connection shall be done, also
 *                       used in the Extra Info.
 *
 * @return ID of the newly created connection, or CTP_CID_NONE on error.
 */
ctp_cid CSPS_API ctp_new_connection_out(ctp_identity_t *ctp_id,
                                        const logaddr_t *my_logaddr, const logaddr_t *peer_logaddr);
                                        
/**
 * Create a connection to a specified IP address and UDP port.
 * @note This for special usage scenarios and is normally not needed.
 *
 * @param  ctp_id   CTP identity handler (returned by ctp_listen()).
 * @param  my_logaddr    Own Logaddr for Extra Info.
 * @param  peer_logaddr  Peer Logaddr for Extra Info.
 * @param  ip       The target IP address.
 * @param  port     The target UDP port.
 *
 * @return ID of the newly created connection, or CTP_CID_NONE on error.
 */
ctp_cid CSPS_API ctp_new_connection_toip(ctp_identity_t *ctp_id,
                                         const logaddr_t *my_logaddr, const logaddr_t *peer_logaddr,
                                         const struct in_addr *ip, uint16_t port);
                                         
/**
 * Create a connection to a specified IPv6 address and UDP port.
 * @note This for special usage scenarios and is normally not needed.
 *
 * @param  ctp_id   CTP identity handler (returned by ctp_listen()).
 * @param  my_logaddr    Own Logaddr for Extra Info.
 * @param  peer_logaddr  Peer Logaddr for Extra Info.
 * @param  ip       The target IPv6 address.
 * @param  port     The target UDP port.
 *
 * @return ID of the newly created connection, or CTP_CID_NONE on error.
 */
ctp_cid CSPS_API ctp_new_connection_toip6(ctp_identity_t *ctp_id,
                                          const logaddr_t *my_logaddr, const logaddr_t *peer_logaddr,
                                          const struct in6_addr *ip, uint16_t port);

/**
 * Create a connection using the tap for transmissions.
 *
 * @note Only available if CTP TAP support is enabled (CSPS_CTP_TAP_ENABLE is
 *       defined).
 *
 * @param  my_logaddr    Own Logaddr for Extra Info.
 * @param  peer_logaddr  Peer Logaddr to which connection shall be done, also
 *                       used in the Extra Info.
 *
 * @return ID of the newly created connection, or CTP_CID_NONE on error.
 */
ctp_cid CSPS_API ctp_new_connection_totap(
        const logaddr_t *my_logaddr, const logaddr_t *peer_logaddr);
        
/**
 * Tell if a connection is on the tap or on regular UDP.
 * @param cid  The connection cid
 * @return 1 if the connection is on the tap, 0 if it is on regular UDP,
 *         -1 if an error occurred (e.g. no such connection).
 */
int CSPS_API ctp_conn_is_on_tap(ctp_cid cid);

/**
 * Close an open connection.
 * After the call, the @a cid is invalid and cannot be used anymore.
 *
 * @param  cid  CID of the connection to be closed
 */
void CSPS_API ctp_close(ctp_cid cid);

/**
 * Close an open connection.
 * After the call, the @a cid is invalid and cannot be used anymore.
 * Resources are freed immediately.
 *
 * @param  cid  CID of the connection to be closed
 */
void CSPS_API ctp_close_forced(ctp_cid cid);

/**
 * Handle queued messages and events on all connections.
 * @note To be called in a medium priority context (e.g. in the main
 *       application loop).
 */
void ctp_run(void);

#define CTP_TIMER_PERIOD 100 ///< Invocation period for ctp_timer() [ms]
/**
 * Timer to be called periodically.
 * Performs retransmissions, deallocates closed connections etc.
 *
 * @param nticks  Time elapsed since the last call, in units of
 *                ::CTP_TIMER_PERIOD. Must be >0.
 */
void ctp_timer(int16_t nticks);

/**
 * Sends a CTP message over a connection.
 *
 * @param  cid    CTP connection ID.
 * @param  ptr    Pointer to the message content (not NULL).
 *                It will be deallocated using csps_free() once sent.
 * @param  len    Message length (>0) [bytes].
 */
void ctp_write(ctp_cid cid, void *ptr, uint16_t len);

/**
 * Send a broadcast (connectionless) message.
 * This message has no acknowledgement: it is sent and forgotten.
 * The Destination Logaddr in the Extra Info for broadcast messages is set to
 * logaddr_unassigned.
 *
 * @param  ctp_id         CTP identity handler (returned by ctp_listen()).
 * @param  my_logaddr     Sender Logaddr (for CTP Extra Info).
 * @param  port           Destination UDP port.
 * @param  payload_ptr    Pointer to the message content (not NULL).
 *                        It will be deallocated using csps_free() once sent.
 * @param  payload_len    Message content length (>0) [bytes].
 */
void ctp_write_broadcast(ctp_identity_t *ctp_id, const logaddr_t *my_logaddr,
                         uint16_t port, void *payload_ptr, uint16_t payload_len);
                         
/**
 * Get the next message received on a CTP connection.
 *
 * @param  cid  ID of the connection from which to read the message.
 *              Must correspond to a valid connection.
 * @param  data Pointer to the data buffer to contain the message.
 *              Must be not NULL.
 *
 * @retval  >0        A pointer to the message has been returned in data.
 *                    It must be deallocated by the caller.
 *                    The returned value is the message length in bytes.
 * @retval  =0        No messages in queue.
 * @retval  ERR_CONN  The connection identified by CID is not valid or has
 *                    been aborted.
 */
int ctp_read(ctp_cid cid, void **data);

/**
 * Get the next broadcast (connectionless) message received.
 *
 * @param[out] data           Pointer to the data buffer to contain the message.
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
 * @retval  >0        A pointer to the message has been returned in data.
 *                    It must be deallocated by the caller.
 *                    The returned value is the message length in bytes.
 * @retval  =0        No messages in queue.
 */
int ctp_read_broadcast(void **data, ctp_identity_t **ctp_id,
                       logaddr_t *local_addr, logaddr_t *peer_addr,
                       int *have_addresses);
                       
/**
 * Obtain CTP transport addresses for the connection (from Extra Info).
 * Retrieves the transport addresses for the connection if available.
 * - For outgoing connections they are the same addresses set by calling
 *   ctp_new_connection_out() or ctp_new_connection_toip().
 * - For incoming connections they are obtained from the first received packet;
 *   depending on the peer implementation they may not available.
 *
 * @param  local_addr  The address at our side. Ignored if NULL.
 * @param  peer_addr   The address of the peer. Ignored if NULL.
 *
 * @return true if the addresses are available, false otherwise.
 */
int ctp_get_addresses(ctp_cid cid, logaddr_t *local_addr, logaddr_t *peer_addr);

/**
 * Obtain IP address and/or UDP port of the peer of a CTP connection.
 *
 * @note The values returned ha no meaning for a tap connection.
 *
 * @param[in]  cid   ID of the TCP connection to query.
 * @param[out] ip    The peer IPv4 address (in network byte order). Ignored if NULL.
 * @param[out] port  The peer UDP port     (in host    byte order). Ignored if NULL.
 *
 * @return 0 if OK, <0 on error (e.g. no such connection).
 */
int CSPS_API ctp_get_peer_udp_socket(ctp_cid cid, struct in_addr *ip, uint16_t *port);

/**
 * Inject a packet into CTP via the tap.
 * The packet will be handled as an incoming packet.
 *
 * @param msg_data  Pointer to the message data.
 *                  It will be freed internally using free().
 * @param msg_size  Size of the message in bytes.
 * @return 0 if OK, <0 on error.
 */
int ctp_tap_inject(void *msg_data, uint16_t msg_size);


#ifdef __cplusplus
}
#endif

#endif // CTP_H
