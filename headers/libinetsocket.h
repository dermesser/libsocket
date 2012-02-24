# ifndef LIBINETSOCKET_HEAD
# define LIBINETSOCKET_HEAD
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

// Macro definitions

# define TCP 1
# define UDP 2

# define IPv4 3
# define IPv6 4

# define BOTH 5 // what fits best (TCP/UDP or IPv4/6)

# define READ 1
# define WRITE 2

# define NUMERIC 1

// Creates socket, connects it and gives it back
//                Hostname          Port/Service         Transport protocol (TCP or UDP)  Network Protocol (IPv4 or IPv6)  may be SOCK_NONBLOCK (socket won't block) and SOCK_CLOEXEC (closes socket on exec() call)
extern int create_isocket(const char* host, const char* service, char proto_osi4,         char proto_osi3                 
# ifdef __linux__
		, int flags);
# else
);
# endif

// Reconnect a socket to another peer - works only for UDP!
//			     Socket,  new peer,   new port
extern int reconnect_isocket(int sfd, char* host, char* service);

// Destroy a socket
//		   Socket file descriptor
extern int destroy_isocket(int sfd);

// Shutdown a socket
// 			       FD       READ, WRITE, READ | WRITE
extern int shutdown_isocket(int sfd, int method);

// Create, bind and let a new server socket listen (stands for create internet server socket)
// at which address to bind, at which port to bind, which protocol (TCP/UDP), which protocol (IPv4/IPv6)
extern int create_issocket(const char* bind_addr, const char* bind_port, char proto_osi4, char proto_osi3);

// Accept connections (like accept()). May block
// Socket fd, memory for hostname, its length, Memory for service name, its length, Flags (NUMERIC)
extern int accept_issocket(int sfd, char* src_host, size_t src_host_len, char* src_service, size_t src_service_len, int flags);

// Receive some bytes from a (mostly UDP/SOCK_DGRAM) socket
// 				File desc. Buffer and  its length   Client address and its length        Client port/service and its length         may be NUMERIC (gives host and service in numeric form)
extern size_t recvfrom_issocket(int sfd, void* buffer, size_t size, char* src_host, size_t src_host_len, char* src_service, size_t src_service_len, int flags);

# endif
