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

extern int create_unix_stream_socket(const char* path);
extern int create_unix_dgram_socket(void);
extern int connect_unix_dgram_socket(int sfd, const char* path);
extern int destroy_unix_socket(int sfd);
extern int shutdown_unix_stream_socket(int sfd, int method);
extern int create_unix_server_socket(char* path, int socktype);
extern int accept_unix_stream_socket(int sfd, int flags);
extern ssize_t recvfrom_unix_dgram_socket(int sfd, void* buf, size_t size, char* from, size_t from_size);
extern ssize_t sendto_unix_dgram_socket(int sfd, void* buf, size_t size, char* path);

# endif
