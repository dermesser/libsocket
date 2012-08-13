# include "../headers/libinetsocket.h"
# include "../headers/socket.hpp"
# include <string>
# include "../headers/exception.hpp"
# include <unistd.h>
# include <string.h>
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

# define READ 1
# define WRITE 2

/*
 * DESCRIPTION FOR STREAMCLIENT.CPP
 * 	This class provides, similar to dgramclient.cpp resp.
 * 	dgram_client_socket, the basic I/O functions for all
 * 	stream-based sockets (TCP and UNIX-SOCK_STREAM).
 */

namespace libsocket
{
	class stream_client_socket : public virtual socket
	{
		public:

		ssize_t snd(const void* buf, size_t len, int flags=0); // flags: send()
		ssize_t rcv(void* buf, size_t len, int flags=0); // flags: recv()

		friend stream_client_socket& operator<<(stream_client_socket& sock, const char* str);
		friend stream_client_socket& operator<<(stream_client_socket& sock, string& str);
		friend stream_client_socket& operator>>(stream_client_socket& sock, string& dest);

		void shutdown(int method=WRITE);
	};

	ssize_t stream_client_socket::rcv(void* buf, size_t len, int flags)
	{
		ssize_t recvd;

		if ( sfd == -1 )
			throw socket_exception(__FILE__,__LINE__,"stream_client_socket::rcv() - Socket is not connected!\n");

		if ( buf == NULL || len == 0 )
			throw socket_exception(__FILE__,__LINE__,"stream_client_socket::rcv() - Buffer or length is null!\n");

		if ( -1 == (recvd = recv(sfd,buf,len,flags)) )
			throw socket_exception(__FILE__,__LINE__,"stream_client_socket::rcv() - Error while reading!\n");

		return recvd;
	}

	stream_client_socket& operator>>(stream_client_socket& sock, string& dest)
	{
		ssize_t read_bytes;
		char* buffer;

		buffer = new char[dest.size()];

		if ( sock.sfd == -1 )
			throw socket_exception(__FILE__,__LINE__,">>(std::string) input: Socket not connected!\n");

		if ( -1 == (read_bytes = read(sock.sfd,buffer,dest.size())) )
			throw socket_exception(__FILE__,__LINE__,">>(std::string) input: Error while reading!\n");

		if ( read_bytes < static_cast<ssize_t>(dest.size()) )
			dest.resize(read_bytes); // So the client doesn't print content more than one time
						 // and it can check if the string's length is 0 (end of transmission)

		dest.assign(buffer,read_bytes);

		delete buffer;

		return sock;
	}

	// O

	stream_client_socket& operator<<(stream_client_socket& sock, const char* str)
	{
		if ( sock.sfd == -1 )
			throw socket_exception(__FILE__,__LINE__,"<<(const char*) output: Socket not connected!\n");
		if ( str == NULL )
			throw socket_exception(__FILE__,__LINE__,"<<(const char*) output: Null buffer given!\n");

		size_t len = strlen(str);

		if ( -1 == write(sock.sfd,str,len) )
			throw socket_exception(__FILE__,__LINE__,"<<(const char*) output: Write failed!\n");

		return sock;
	}

	stream_client_socket& operator<<(stream_client_socket& sock, string& str)
	{
		if ( sock.sfd == -1 )
			throw socket_exception(__FILE__,__LINE__,"<<(std::string) output: Socket not connected!\n");

		if ( -1 == write(sock.sfd,str.c_str(),str.size()) )
			throw socket_exception(__FILE__,__LINE__,"<<(std::string) output: Write failed!\n");

		return sock;
	}

	ssize_t stream_client_socket::snd(const void* buf, size_t len, int flags)
	{
		ssize_t snd_bytes;

		if ( sfd == -1 )
			throw socket_exception(__FILE__,__LINE__,"stream_client_socket::snd() - Socket not connected!\n");
		if ( buf == NULL || len == 0 )
			throw socket_exception(__FILE__,__LINE__,"stream_client_socket::snd() - Buffer or length is null!\n");

		if ( -1 == (snd_bytes = send(sfd,buf,len,flags)) )
			throw socket_exception(__FILE__,__LINE__,"stream_client_socket::snd() - Error while sending\n");

		return snd_bytes;
	}

	void stream_client_socket::shutdown(int method)
	{
		if ( 0 > shutdown_inet_stream_socket(sfd,method)) // It's equal whether we use this or its brother from libunixsocket
		{
			throw socket_exception(__FILE__,__LINE__,"stream_client_socket::shutdown() - Could not shutdown socket\n");
		}
	}
}
