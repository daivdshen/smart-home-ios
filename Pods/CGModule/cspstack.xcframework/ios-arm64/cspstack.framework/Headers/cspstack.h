/**
 *
 * @file cspstack.h
 *
 * CSP Stack - Main Interface.
 *
 * Copyright (C) 2009-2015 Comelit Group SpA
 *
 ***************************************************************************/

#ifndef CSPSTACK_H
#define CSPSTACK_H

#include <stdint.h>
#include "port.h"


#ifdef __cplusplus
extern "C" {
#endif


#define CSPS_TIMER_PERIOD 100 ///< Periodo invocazione ctp_timer() [ms]

/// Structure to return the static library parameters.
typedef struct
{
	int     crp_cache_n_entries;
	int     crp_identities_n_entries;
	uint8_t crp_enabled;
	uint8_t crp_detached;
	uint8_t ctp_tap_enabled;
	uint8_t viper_client_enabled;
	uint8_t viper_server_enabled;
	uint8_t viper_gnutls_enabled;
} cspstack_params_t;

/**
 * Get the library configuration parameters.
 *
 * @note Currently implemented on autotools-based platforms.
 *
 * @param params  A structure to be filled the library parameters (not NULL).
 * @param len     Length of @a param.
 * @return The amount of bytes written into @a params (equal to the minimum
 *         between @a len and sizeof(cspstack_params_t), or 0 if this
 *         feature is not supported).
 */
size_t CSPS_API cspstack_get_params(cspstack_params_t *params, size_t len);

/**
 * Return the version of the cspstack library you're using.
 * @note This is implemented only on autotools-based build systems.
 * @return The library version string, e.g. "3.6".
 *         This is a pointer to an internal static const string.
 *         On non-sutotools-based build systems the return value is always
 *         "unknown".
 */
const char *cspstack_version(void);

/**
 * Initialize the stack.
 *
 * @note Before any usage, call srand(). CTP uses the C library random number
 *       generator (rand()).
 *
 * @param crp_flags  Flags that affect CRP operation:
 *                   - CRP_DO_BACKEND: in detached mode, activates backend
 *                     operations (replying to incoming requests and sending
 *                     requests for pending cache entries).
 *                     In non-detached mode, this flag does nothing.
 *                   - CRP_START_DISABLED: initialize CRP without enabling CRP
 *                     functionality. CRP will only reply to crp_resolve() for
 *                     local identities.
 *                     CRP can be enabled and disabled at any time calling
 *                     crp_enable() and crp_disable().
 */
void CSPS_API cspstack_init(uint8_t crp_flags);

/**
 * Executes routinary activities.
 * @note To be called in a low-priority context.
 *       For single-threaded applications it should be normally called from
 *       within the main ("idle") application loop.
 */
void CSPS_API cspstack_run(void);

/**
 * Top-level timer, to be called periodically.
 * Manages all internal timers.
 * It should be called at least every second (possibly every
 * ::CSPS_TIMER_PERIOD), to guarantee correct behaviour in all cases.
 * Can be safely invoked from interrupt context.
 *
 * @param nticks  Time elapsed since the last call, in units of
 *                ::CSPS_TIMER_PERIOD. Must be >0.
 */
void CSPS_API cspstack_timer(int16_t nticks);


#ifdef __cplusplus
}
#endif

#endif // CSPSTACK_H

