# include <iostream>
# include <string>
# include <string.h>

# include "../headers/libinetsocket.h"
# include "../headers++/socket.hpp"
# include "../headers++/inetbase.hpp"
# include "../headers++/exception.hpp"

# include <unistd.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <stdio.h>
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

namespace libsocket
{
	using std::string;

/**************** inet_stream class (TCP inet sockets) *********/

	class inet_stream : public inet_socket
	{
		public:

		inet_stream(void);
		inet_stream(const char* dsthost, const char* dstport, int proto_osi3, int flags=0); // flags: socket()
		inet_stream(const string& dsthost, const string& dstport, int proto_osi3, int flags=0);

		// Real actions
		void connect(const char* dsthost, const char* dstport, int proto_osi3, int flags=0); // flags: socket()
		void shutdown(int method);

		// I/O
		// O
		friend inet_stream& operator<<(inet_stream& sock, const char* str);
		friend inet_stream& operator<<(inet_stream& sock, string& str);

		ssize_t snd(const void* buf, size_t len, int flags=0); // flags: send()

		// I
		friend inet_stream& operator>>(inet_stream& sock, string& dest);

		ssize_t rcv(void* buf, size_t len, int flags=0); // flags: recv()

		// Other friends

		friend class inet_stream_server; // So it's possible for inet_stream_server::accept() to construct an instance with given fd
	};

	inet_stream::inet_stream(void)
	{
	}

	inet_stream::inet_stream(const char* dsthost, const char* dstport, int proto_osi3, int flags)
	{
		connect(dsthost,dstport,proto_osi3,flags);
	}

	inet_stream::inet_stream(const string& dsthost, const string& dstport, int proto_osi3, int flags)
	{
		connect(dsthost.c_str(),dstport.c_str(),proto_osi3,flags);
	}

	void inet_stream::connect(const char* dsthost, const char* dstport, int proto_osi3, int flags)
	{
		if ( sfd != -1 )
			throw socket_exception(__FILE__,__LINE__,"inet_stream::connect() - Already connected!\n");

		sfd = create_inet_stream_socket(dsthost,dstport,proto_osi3,flags);

		if ( sfd < 0 )
			throw socket_exception(__FILE__,__LINE__,"inet_stream::connect() - Could not create socket\n");

		host = dsthost;
		port = dstport;
		proto = proto_osi3;
	}


	void inet_stream::shutdown(int method)
	{
		if ( 0 > shutdown_inet_stream_socket(sfd,method))
		{
			throw socket_exception(__FILE__,__LINE__,"inet_stream::shutdown() - Could not shutdown socket\n");
		}
	}

	// I/O

	// I

	ssize_t inet_stream::rcv(void* buf, size_t len, int flags)
	{
		ssize_t recvd;

		if ( sfd == -1 )
			throw socket_exception(__FILE__,__LINE__,"inet_stream::rcv() - Socket is not connected!\n");

		if ( buf == NULL || len == 0 )
			throw socket_exception(__FILE__,__LINE__,"inet_stream::rcv() - Buffer or length is null!\n");

		if ( -1 == (recvd = recv(sfd,buf,len,flags)) )
			throw socket_exception(__FILE__,__LINE__,"inet_stream::rcv() - Error while reading!\n");

		return recvd;
	}

	inet_stream& operator>>(inet_stream& sock, string& dest)
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

	inet_stream& operator<<(inet_stream& sock, const char* str)
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

	inet_stream& operator<<(inet_stream& sock, string& str)
	{
		if ( sock.sfd == -1 )
			throw socket_exception(__FILE__,__LINE__,"<<(std::string) output: Socket not connected!\n");

		if ( -1 == write(sock.sfd,str.c_str(),str.size()) )
			throw socket_exception(__FILE__,__LINE__,"<<(std::string) output: Write failed!\n");

		return sock;
	}

	ssize_t inet_stream::snd(const void* buf, size_t len, int flags)
	{
		ssize_t snd_bytes;

		if ( sfd == -1 )
			throw socket_exception(__FILE__,__LINE__,"inet_stream::snd() - Socket not connected!\n");
		if ( buf == NULL || len == 0 )
			throw socket_exception(__FILE__,__LINE__,"inet_stream::snd() - Buffer or length is null!\n");

		if ( -1 == (snd_bytes = send(sfd,buf,len,flags)) )
			throw socket_exception(__FILE__,__LINE__,"inet_stream::snd() - Error while sending\n");

		return snd_bytes;
	}
}
