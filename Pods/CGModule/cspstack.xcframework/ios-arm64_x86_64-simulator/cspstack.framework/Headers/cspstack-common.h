/**
 *
 * @file cspstack-common.h
 *
 * CSP Stack - common definitions and utilities.
 *
 * Copyright (C) 2015 Comelit Group SpA
 *
 ***************************************************************************/

#ifndef CSPSTACK_COMMON_H
#define CSPSTACK_COMMON_H

#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

// Some libraries (e.g. Blackfin VisualDSP) declare errno in errno.h, but
// don't define its possible values.

#ifndef ENOMEM
#define ENOMEM      12 /* Out of memory */
#endif

#ifndef EEXIST
#define EEXIST      17 /* File exists */
#endif

#ifndef EINVAL
#define EINVAL      22 /* Invalid argument */
#endif

#ifndef ENOSYS
#define ENOSYS      38 /* Function not implemented */
#endif

#ifndef EPROTO
#define EPROTO      71 /* Protocol error */
#endif

#ifndef ENOTSOCK
#define ENOTSOCK    88 /* Socket operation on non-socket */
#endif

#ifndef EISCONN
#define EISCONN     106 /* Transport endpoint is already connected */
#endif

#ifndef ENOTCONN
#define ENOTCONN    107 /* Transport endpoint is not connected */
#endif

#ifndef EINPROGRESS
#define EINPROGRESS 115 /* Operation now in progress */
#endif

#ifdef __cplusplus
}
#endif

#endif // CSPSTACK_COMMON_H

