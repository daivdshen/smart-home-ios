/**
 *
 * @file port.h
 *
 * Copyright (C) 2009-2013 Comelit Group SpA
 *
 ***************************************************************************/


#ifndef PORT_H
#define PORT_H

#ifdef __cplusplus
extern "C" {
#endif


// Environment includes ====================================================

// BSD socket interface
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <net/if.h>

// Standard C includes
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>


// Configuration ===========================================================

// Error values ============================================================

#define ERR_OK     0


// Compiler specific =======================================================

#define CSPS_API
#define CSPS_INLINE static inline
#define CSPS_ASSERT(p) assert(p)
#define CSPS_PACK_STRUCT_BEGIN
#define CSPS_PACK_STRUCT_END
#define CSPS_PACK_STRUCT_STRUCT __attribute__((__packed__))
#define CSPS_UNUSED_PARAM(var) (void)(var)


// Protection of critical sections =========================================

#define CSPS_DECL_PROTECT(lev)
#define CSPS_PROTECT(lev)
#define CSPS_UNPROTECT(lev)


// Dynamic memory allocation ===============================================

CSPS_INLINE void *csps_malloc(size_t size)
{
	return malloc(size);
}

CSPS_INLINE void csps_free(void *ptr)
{
	free(ptr);
}


// Debug ===================================================================

#ifdef DEBUG_ON

#define CSPS_DEBUG_PRINTF(...) do { printf(__VA_ARGS__); } while(0)
#define CSPS_DEBUG_PUTCHAR(c)  do { putchar((c));        } while(0)

#else

#define CSPS_DEBUG_PRINTF(...) do {} while(0)
#define CSPS_DEBUG_PUTCHAR(c)  do {} while(0)

#endif


// Random numbers ==========================================================

#define csps_rand32() rand()

CSPS_INLINE uint16_t csps_rand16(void)
{
	uint32_t r = rand();
	r = r ^ (r>>16);
	return (r & 0xffff);
}

CSPS_INLINE uint8_t csps_rand8(void)
{
	uint32_t r = rand();
	r = r ^ (r>>8) ^ (r>>16) ^ (r>>24);
	return (r & 0xff);
}


// Entry point =============================================================

#define port_init()
#define port_run()
#define port_timer(nticks)


// TCP/IP stack ============================================================

typedef int           csps_socket;
typedef csps_socket   csps_usocket;

typedef void (*csps_usocket_recv_cb)(void *arg, void *buf, uint16_t buflen,
                                     struct sockaddr_storage *inaddr);

#define csps_usocket_isvalid(us)     ((us) != -1)
#define csps_usocket_none            (-1)

// Protocol family is either PF_INET or PF_INET6
csps_usocket csps_usocket_create(int proto);

CSPS_INLINE int csps_usocket_set_reuseaddr(csps_usocket fd, int value)
{
  return setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value));
}

CSPS_INLINE int csps_usocket_set_ifname(csps_usocket fd, const char * value)
{
	int idx = if_nametoindex(value);
	if (idx)
		return setsockopt(fd, IPPROTO_IP, IP_BOUND_IF, &idx, sizeof(idx));
	else
		return -1;
}

// Address family is either AF_INET or AF_INET6.
// If family is set to AF_INET, inaddr is expected to point to a struct in_addr structure
// If family is set to AF_INET6, inaddr is expected to point to a struct in6_addr structure
int csps_usocket_bind(csps_usocket pcb, int family, const void *inaddr, uint16_t port);

CSPS_INLINE int csps_usocket_recv(csps_usocket upcb, csps_usocket_recv_cb callback, void *arg)
{
  CSPS_UNUSED_PARAM(upcb);
  CSPS_UNUSED_PARAM(callback);
  CSPS_UNUSED_PARAM(arg);

  return ERR_OK;
}

int csps_usocket_poll(csps_usocket usock, csps_usocket_recv_cb cb, void *arg);
int16_t csps_udp_sendto(csps_usocket pcb, const void *buf, uint16_t buflen,
                        const struct in_addr *dst_in_addr, uint16_t dst_port);
int16_t csps_udp_sendto6(csps_usocket pcb, const void *buf, uint16_t buflen,
                        const struct in6_addr *dst_in_addr, uint16_t dst_port);

CSPS_INLINE void csps_usocket_destroy(csps_usocket usocket)
{
  close(usocket);
}

int csps_usocket_getlocaladdr(csps_usocket usock,
                              struct in_addr *local_in_addr,
                              uint16_t *local_port);

int csps_usocket_getlocaladdr6(csps_usocket usock,
                               struct in6_addr *local_in_addr,
                               uint16_t *local_port);

int csps_set_nonblocking(int fd, bool do_not_block);

CSPS_INLINE int csps_socket_set_nonblocking(csps_socket sock, bool do_not_block)
{
	return csps_set_nonblocking(sock, do_not_block);
}

CSPS_INLINE int csps_usocket_set_nonblocking(csps_usocket sock, bool do_not_block)
{
	return csps_socket_set_nonblocking(sock, do_not_block);
}

#ifdef __cplusplus
}
#endif

#endif // PORT_H
