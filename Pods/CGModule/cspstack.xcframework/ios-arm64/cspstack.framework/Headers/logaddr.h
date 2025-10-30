/**
 *
 * @file logaddr.h
 *
 * CSP Stack - Logical Address.
 *
 * Copyright (C) 2009 Comelit Group SpA
 *
 ***************************************************************************/

#ifndef LOGADDR_H
#define LOGADDR_H

#include <stdint.h>
#include <string.h>
#include "port.h"


#ifdef __cplusplus
extern "C" {
#endif


#define LOGADDR_NDIGITS 8  ///< Comelit IP System has 8 digits!

/**
 * Logical Address - Main part.
 * @note In most cases the address is sent on the wire together with the
 *       subaddress. In this case you should use ::logaddr_t.
 */
typedef uint8_t  logaddr_main_t[LOGADDR_NDIGITS];

/**
 * Logical Sub-Address.
 */
typedef uint16_t logaddr_sub_t;

/**
 * Complete Logical Address (network encoding).
 * @note Depending on the architecture used, variables of this type may need
 *       to be aligned to a 4-byte boundary, using a directive such as:
 *       \code
 *       #pragma align 4
 *       \endcode
 */
CSPS_PACK_STRUCT_BEGIN
typedef struct
{
	logaddr_main_t digit;  ///< "Main" Logical address
	logaddr_sub_t  sub;    ///< Sub-address
} CSPS_PACK_STRUCT_STRUCT logaddr_t;
CSPS_PACK_STRUCT_END

/**
 * Logical Address Range (Main part only).
 */
CSPS_PACK_STRUCT_BEGIN
typedef struct
{
	logaddr_main_t min;
	logaddr_main_t max;
} CSPS_PACK_STRUCT_STRUCT logaddr_range_t;
CSPS_PACK_STRUCT_END

#define SUBADDR_NONE   0x00 ///< The sub-address of an Addressable Unit
#define SUBADDR_BCAST  0xff ///< Sub-address value for Broadcast
#define SUBADDR_RTC    0x63 ///< Sub-address value for rtc unit

/// True if s is a valid sub-address for a device (not an Addressable Unit)
#define subaddr_is_sub(s)   ( (((s) & 0xf0)==0x30) || ( s == SUBADDR_RTC) )

/// True if s is a valid sub-address (device, Addressable Unit or broadcast)
#define subaddr_is_valid(s) ( subaddr_is_sub(s) ||                    \
                              (s)==SUBADDR_NONE || (s)==SUBADDR_BCAST )

/// Special value, not to be used in normal cases: marks a logical address
/// as not assigned. It is fully composed of 0x00 bytes.
extern const logaddr_t logaddr_unassigned;

/// Print complete logical address (e.g. "00000123#7")
#define LOGADDR_PRINT_ALL    0x00
/// Do not print leading zeros in the main logaddr (e.g. "123#7").
/// Exception: address 00000000, becomes string "0", not "".
#define LOGADDR_STRIP_ZERO   0x01
/// Do not print the subaddress (e.g. "00000123")
#define LOGADDR_STRIP_SUB    0x02
/// Do not print leading zeros nor the subaddress (e.g. "123")
#define LOGADDR_STRIP_ALL (LOGADDR_STRIP_ZERO | LOGADDR_STRIP_SUB)

/**
 * Set a Main logical address from a string.
 * Example: string "123" -> digits "00000123".
 *
 * @param  lm             Pointer to the logical address to set.
 * @param  digits_string  String containing  8 digit. It must
 *                        contain only valid digits.
 *                        If it contains less than 8 characters,
 *                        the string is filled up with zeroes.
 */
void CSPS_API logaddr_main_set_s(logaddr_main_t lm, const char *digits_string);

/**
 * Set a logical address from a <string, integer> pair.
 * Example: string "123" -> digits "00000123".
 *
 * @param  logaddr        Pointer to the logical address to set.
 * @param  digits_string  String containing  8 digit. It must
 *                        contain only valid digits.
 *                        If it contains less than 8 characters,
 *                        the string is filled up with zeroes.
 * @param  sub            Sub-address. It must already be in the
 *                        right coding (see. subaddr_is_valid()).
 */
void CSPS_API logaddr_set_si(logaddr_t *logaddr, const char *digits_string, uint16_t sub);

/**
 * Set a logical address obtaining it from an <integer, integer> pair.
 * Usable only if the logical address is completely composed by decimal
 * digits and if it is less than 2^32.
 * Example: integer 123 -> digits "00000123".
 *
 * @param  logaddr     Pointer to the logical address to be set.
 * @param  digits_int  Integer to be transformed into 8 decimal digits.
 * @param  sub         Sub-address. It must already be in the
 *                     right coding (cfr. subaddr_is_valid()).
 */
void CSPS_API logaddr_set_ii(logaddr_t *logaddr, uint32_t digits_int, uint16_t sub);

/**
 * Returns an integer number corresponding to the "main" logical address.
 * Usable only if the logical address is completely composed by
 * decimal digits and it is less than 2^32.
 * Example: digits "00000123" -> integer 123.
 *
 * @param  logaddr   Pointer to the logical address to be converted.
 * @return The main logical address expressed as an integer.
 */
uint32_t CSPS_API logaddr_get_ii(const logaddr_t *logaddr);

/// Copies a logical address (including subaddress)
void CSPS_API logaddr_cpy(logaddr_t *dest, const logaddr_t *src);

/// True if the logical addresses are equal (subaddress including)
int CSPS_API logaddr_eq(const logaddr_t *log_ptr1, const logaddr_t *log_ptr2);

/// Copies a main logical address
void CSPS_API logaddr_main_cpy(logaddr_main_t dest,
                               const logaddr_main_t src);
                               
/// Compare main logical addresses (memcmp-like)
int CSPS_API logaddr_main_cmp(const logaddr_t *log_ptr1,
                              const logaddr_t *log_ptr2);
                              
/// True if the main logical addresses are equal (subaddress not included)
int CSPS_API logaddr_main_eq(const logaddr_t *log_ptr1,
                             const logaddr_t *log_ptr2);
                             
/// Copies a logical address range
void CSPS_API logaddr_range_cpy(logaddr_range_t *dest,
                                const logaddr_range_t *src);
                                
/// True if the range is valid (min <= max)
int CSPS_API logaddr_range_isvalid(const logaddr_range_t *rng_ptr);

/// True if the logaddr in in the range (inclusive).
/// @note The range must be valid (see logaddr_range_isvalid()).
int CSPS_API logaddr_is_in_range(const logaddr_t       *addr,
                                 const logaddr_range_t *rng);
                                 
/// True if there is at least one logical address that is in both ranges.
/// @note Ranges must be valid (see logaddr_range_isvalid()).
/// Condition: (r1.max < r2.min) || (r1.min > r2.max)
int CSPS_API logaddr_ranges_overlap(const logaddr_range_t *r1,
                                    const logaddr_range_t *r2);
                                    
/// True if the Main Logaddr is unassigned.
CSPS_INLINE int logaddr_main_is_unassigned(const logaddr_main_t lm)
{
	return (logaddr_main_cmp((logaddr_t *)lm, &logaddr_unassigned) == 0);
}

/// True if the logaddr contains only valid characters ([0-9A-Z])
int CSPS_API logaddr_main_is_valid(const logaddr_main_t lm);

CSPS_INLINE int logaddr_is_valid(const logaddr_t *addr)
{
	return (subaddr_is_valid(addr->sub) && logaddr_main_is_valid(addr->digit));
}


/**
 * Write a logical address in a printable string.
 *
 * @param  logaddr  Logical address to be translated into a string.
 * @param  flags    flag che definiscono la formattazione della stringa
 *
 * @note Not reentrant (the string is a static variable).
 *
 * @return String representation of the logical address.
 */
char *CSPS_API logaddrtoa(const logaddr_t *logaddr, uint16_t flags);

/**
 * Write a "main" logical address in a printable string.
 */
char *CSPS_API logaddr_main_toa(const logaddr_main_t logaddr_main, uint16_t flags);

/**
 * Write a logical address range to a printable string.
 *
 * @param  range    Logical address range to be translated into a string.
 * @param  flags    flag che definiscono la formattazione della stringa
 *
 * @note Not reentrant (the string is a static variable).
 *
 * @return String representation of the logical address.
 */
char *CSPS_API logaddr_range_toa(const logaddr_range_t *range, uint16_t flags);

#ifdef __cplusplus
}
#endif

#endif // LOGADDR_H
