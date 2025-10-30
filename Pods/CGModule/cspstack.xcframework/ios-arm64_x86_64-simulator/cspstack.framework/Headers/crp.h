/**
 *
 * @file crp.h
 *
 * Comelit Resolution Protocol Implementation.
 *
 * @note Only available if CRP is enabled (CSPS_CRP_ENABLE is defined).
 *
 * Resolves logical addresses (8 digit + sub) to <IP address, UDP port>
 * pairs.
 * Responds to the incoming CRP resolution requests.
 *
 * It has a simple, polling-based API.
 *
 * Usage:
 * - crp_init() once at the start of application
 * - crp_resolve() to resolve an address
 *
 * Copyright (C) 2009-2015 Comelit Group SpA
 *
 ***************************************************************************/

#ifndef CRP_H
#define CRP_H

#include <stdint.h>
#include "logaddr.h"
#include "port.h"

#ifdef __cplusplus
extern "C" {
#endif


/// A device identity at the networking (CRP) level.
typedef struct crp_identity_str crp_identity_t;

/// Act as the CRP backend: update structures based on timers etc.
/// This applies only if DETACHED_CRP is defined.
/// If DETACHED_CRP is not defined, backend operations are always performed
/// regardless of this flag.
#define CRP_DO_BACKEND ((uint8_t)0x01)

/// Start with the CRP system disabled. This is equivalent to calling
/// crp_init() without this flag set and then calling crp_disable().
#define CRP_START_DISABLED ((uint8_t)0x02)

/**
 * Initialize internal structures.
 * If in detached mode and the CRP_DO_BACKEND flag is present, also opens
 * the UDP port for listening.
 * @see crp_identity_add() for UDP port opening in non-detached mode.
 */
void crp_init(uint8_t flags);

#define CRP_TIMER_PERIOD 1000 ///< crp_timer() invocation period [ms]
/**
 * Timer to be called periodically.
 * Performs retransmissions, flushes old cache entries etc.
 * It should be called every ::CRP_TIMER_PERIOD to guarantee retransmission of
 * CRP requests. Otherwise resolution failures may happen.
 *
 * @param nticks  Time elapsed since the last call, in units of
 *                ::CRP_TIMER_PERIOD. Must be >0.
 */
void crp_timer(int16_t nticks);

/**
 * Performs pending actions. Call in the main application loop.
 */
void crp_run(void);

/**
 * Enable CRP functionality.
 * CRP will reply to requests from the network and send requests on the
 * network for resolving other addresses.
 * @see crp_disable().
 */
int CSPS_API crp_enable();

/**
 * Disable CRP functionality.
 * CRP will only reply to crp_resolve() for local identities.
 * After calling crp_disable() it is recommended to flush all the CRP cache
 * by calling crp_invalidate_all().
 * @see crp_enable().
 */
int CSPS_API crp_disable();

/**
 * Add a new CRP identity.
 * If in non-detached mode, when the first identity is added also opens
 * the UDP port for listening.
 * @see crp_init() for UDP port opening in detached mode.
 *
 * @param  logaddr  Local logical address.
 * @param  ip       Local IP address.
 * @param  netmask  Local IP netmask.
 * @param  port     Local CSP port (must NOT be 0).
 * @return Handler of the newly allocated identity, NULL on error.
 *         The handler is valid until it is removed by identity_remove().
 */
crp_identity_t *crp_identity_add(
        const logaddr_t *logaddr, const struct in_addr *ip,
        const struct in_addr *netmask, uint16_t port);
        
        
/**
 * List of options that can be manipulated by @a crp_identity_setopt().
 */
typedef enum
{
	CRP_ID_OPT_LIMITED_BROADCAST = 1,
	CRP_ID_OPT_NUM
} crp_identity_opt;

/**
 * Manipulate options for an identity.
 *
 * @param id The identity to manipulate
 * @param optname The option to change
 * @param optval  The value to be set for the option (the type depends on @a opt)
 * @param optlen  The size of @a optval [bytes]
 *
 * * @a CRP_ID_OPT_LIMITED_BROADCAST: send broadcast messages to the limited
 *   broadcast address (255.255.255.255), not to the directed broadcast address.
 *   @optval must be an int with value 1 to enable limited broadcast mode.
 *   @note Once limited broadcast mode has been enabled, it cannot be disabled.
 *
 * @return 0 on success, -1 on error
 */
int crp_identity_setopt(crp_identity_t *id, crp_identity_opt optname,
                        void *optval, size_t optlen);
                        
/**
 * Remove an existing CRP identity.
 *
 * If in non-detached mode, when the last identity is removed also closes
 * the UDP listening port.
 * If no identity exists with such a handler, does nothing.
 *
 * @param  id       Handler of the identity to be removed
 *                  (created by crp_identity_add()).
 */
void crp_identity_remove(crp_identity_t *id);

/**
 * Resolve a logical address into an <ip_addr,udp_port> pair.
 *
 * - If the request is for one of our identities, our <ip,port> are returned;
 * - if the requested logaddr has recently been resolved, it may still be
 *   cached: the cached <ip,port> is returned;
 * - otherwise a request is sent and 0 is returned: upon receiving a reply
 *   the result is cached, so try again later.
 *
 * @note At least an identity is needed to send a request.
 *       See crp_identity_add().
 *
 * @param  log   Logical address to be resolved.
 * @param  ip    Location for resolved IP address.
 * @param  port  Location for resolved UDP port.
 *
 * @retval 1 if the address has been resolved.
 * @retval 0 if the address has not been resolved.
 */
uint16_t CSPS_API crp_resolve(const logaddr_t *log, struct in_addr *ip, uint16_t *port);

/// Available TTL values for crp_resolve4().
typedef enum
{
	TTL_5_SEC, TTL_30_SEC, TTL_2_MIN, TTL_5_MIN,
	TTL_15_MIN, TTL_1_HR, TTL_6_HR, TTL_18_HR,
	TTL_N_VALUES
} ttl_sel_t;

/**
 * Resolve a logical address into an <ip_addr,udp_port> pair and sets a custom
 * TTL for the result.
 *
 * Same as crp_resolve(), but also allow to pass a custom TTL (Time-to-Live)
 * to be used when the entry has been found. This makes the outcome of the
 * resolution remain in the cache for a longer or shorter time.
 *
 * @note The result of the resolution may become invalid at any time.
 *       Requesting a long TTL may lead to failed connections or connections to
 *       the wrong peer for as long as the TTL.
 *       Use with care!
 *
 * @note Requesting a short TTL may lead to an unnecessarily high broadcast
 *       traffic when repeated resolutions are requested towards the same
 *       target.
 *       Use with care!
 *
 * @param  log   Logical address to be resolved.
 * @param  ip    Location for resolved IP address.
 * @param  port  Location for resolved UDP port.
 * @param  ttl_sel TTL Selection (@see ttl_sel_t for the possible values).
 *
 * @retval 1 if the address has been resolved.
 * @retval 0 if the address has not been resolved.
 **/
uint16_t CSPS_API crp_resolve4(const logaddr_t *log, struct in_addr *ip, uint16_t *port,
                               ttl_sel_t ttl_sel);

/**
 * Evaluate crp cache to resolve a logical address into an <ip_addr,udp_port> pair.
 *
 * - if the requested logaddr has recently been resolved, it may still be
 *   cached: the cached <ip,port> is returned. If requested logaddr is still
 *   in pending 0 is returned while if the logaddr resolution is failed -1 is
 *   returned.
 *   Upon receiving a reply the result is cached, so try again later untill 0 is
 *   returned.
 *
 * @param  log   Logical address to be resolved.
 * @param  ip    Location for resolved IP address.
 * @param  port  Location for resolved UDP port.
 *
 * @retval 1 if the address has been resolved.
 * @retval 0 if the address has not been resolved.
 * @retval -1 if the address resolution is end and failed.
 */
int CSPS_API crp_is_resolved(const logaddr_t *log,
                             struct in_addr *ip,
                             uint16_t *port);

/**
 * Remove a CRP cache entry.
 *
 * Useful when the user has knowledge that a CRP entry has become invalid.
 * This is a rare event, that happens when a remote unit changed its IP
 * address or UDP port. In this case, the existing CRP cache entry maps the
 * same Logical Address to the "old" IP/UDP pair, making the remote unit
 * unreachable until the entry TTL expires.
 *
 * Note that if a remote unit changes its Logical Address this does not make
 * the entry invalid.
 *
 * @param  log  Logical Address whose entry must be removed.
 *
 * @retval 0 if no entry with the given Logical Address has been found.
 * @retval 1 if an entry with the given Logical Address has been found and
 *           removed.
 */
uint16_t CSPS_API crp_invalidate(const logaddr_t *log);

/**
 * Remove all CRP cache entries.
 *
 * Useful for example when the user has knowledge that the device has changed
 * network connectivity and thus it should start a fresh resolution for
 * successive calls.
 *
 * @return 0 if OK, <0 on error.
 */
uint16_t CSPS_API crp_invalidate_all();

/**
 * Return the descriptor of the socket used by the CRP subsystem.
 *
 * @return The type of the return value depends on the underlying IP stack:
 *         - On implementations based on the BSD socket APIs, returns the
 *           socket file descriptor (int). -1 if the socket is not open or an
 *           error occurred.
 *         - On lwIP-based implementations, returns a struct udp_pcb *.
 *           NULL if the socket is not open or an error occurred.
 */
csps_usocket crp_get_socket();

#ifdef __cplusplus
}
#endif

#endif // CRP_H
