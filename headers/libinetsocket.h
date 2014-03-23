#ifndef LIBSOCKET_LIBINETSOCKET_H_C1A9FFEDF5E94B2FB010A0FAA0E92A2F
#define LIBSOCKET_LIBINETSOCKET_H_C1A9FFEDF5E94B2FB010A0FAA0E92A2F
/**
 * @file libinetsocket.h
 *
 * @brief Contains all functions available in the C libsocket.
 *
 */
/*
	The committers of the libsocket project, all rights reserved
	(c) 2012, dermesser <lbo@spheniscida.de>

	Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
	following conditions are met:

		1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
		disclaimer.
		2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
		disclaimer in the documentation and/or other materials provided with the distribution.

	THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
	NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
	EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
	CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
	PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
	OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
	POSSIBILITY OF SUCH DAMAGE.
*/

// Headers (e.g. for flags)
# include <sys/types.h>
# include <sys/socket.h>

// Macro definitions

# define LIBSOCKET_TCP 1
# define LIBSOCKET_UDP 2

# define LIBSOCKET_IPv4 3
# define LIBSOCKET_IPv6 4

# define LIBSOCKET_BOTH 5 // what fits best (TCP/UDP or IPv4/6)

# define LIBSOCKET_READ 1
# define LIBSOCKET_WRITE 2

# define LIBSOCKET_NUMERIC 1

# ifdef __cplusplus
# ifdef MIXED
extern "C" {
# endif
# endif

extern int create_inet_stream_socket(const char* host, const char* service, char proto_osi3, int flags);
extern int create_inet_dgram_socket(char proto_osi3, int flags);
extern ssize_t sendto_inet_dgram_socket(int sfd,const void* buf, size_t size, const char* host, const char* service, int sendto_flags);
extern ssize_t recvfrom_inet_dgram_socket(int sfd, void* buffer, size_t size, char* src_host, size_t src_host_len, char* src_service, size_t src_service_len, int recvfrom_flags, int numeric);
extern int connect_inet_dgram_socket(int sfd, const char* host, const char* service);
extern int destroy_inet_socket(int sfd);
extern int shutdown_inet_stream_socket(int sfd, int method);
extern int create_inet_server_socket(const char* bind_addr, const char* bind_port, char proto_osi4, char proto_osi3, int flags);
extern int accept_inet_stream_socket(int sfd, char* src_host, size_t src_host_len, char* src_service, size_t src_service_len, int flags,int accept_flags);
extern int get_address_family(const char* hostname);

# ifdef __linux__
extern int create_multicast_socket(const char* group, const char* port, const char* local);
# endif

# ifdef __cplusplus
# ifdef MIXED
}
# endif
# endif

# endif
