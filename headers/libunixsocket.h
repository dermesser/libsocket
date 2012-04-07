# ifndef LIBUNIXSOCKET_HEAD
# define LIBUNIXSOCKET_HEAD
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

# define STREAM 1
# define DGRAM  2

# define READ  1
# define WRITE 2

// Create new socket
extern int create_usocket(const char* path, int socktype, const char* bind_path);
// Close a socket
extern int destroy_usocket(int sfd);
// Shutdown a socket (method is READ, WRITE or READ|WRITE)
extern int shutdown_usocket(int sfd, int method);
// socktype is STREAM or DGRAM
extern int create_ussocket(char* path, int socktype);
// flags is SOCK_NONBLOCK or SOCK_CLOEXEC (or ORed)
extern int accept_ussocket(int sfd, int flags);
// Reconnect DGRAM socket
extern int reconnect_usocket(int sfd, const char* path);
// Recv data sent to a DGRAM socket
ssize_t recv_ussocket(int sfd, void* buf, size_t size);

# endif
