# include "../headers++/exception.hpp"
# include "../headers++/socket.hpp"
# include <string>
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

/*
 * DESCRIPTION FOR DGRAMCLIENT.CPP
 * 	dgramclient.cpp contains the class dgram_client_socket. This
 * 	class provides the read/write functions for connected datagram
 * 	sockets (alias client dgram sockets...). This helps to avoid
 * 	writing the same code more than one time.
 * 	The client datagram classes for specific domains, which are
 * 	unix_dgram_client and inet_dgram_client inherit the functions
 * 	declared and defined here to get the ability to send and receive
 * 	data via connected datagram sockets. It is not possible to use
 * 	a class which has this functions also for stream sockets because
 * 	the functions have to check if the socket is connected (bool connected;).
 * 	In the stream classes, this check is realized by checking if the file
 * 	descriptor is -1 or not.
*/

namespace libsocket
{
	using std::string;

	class dgram_client_socket : public virtual socket
	{
		protected:
		bool connected;

		public:

		dgram_client_socket(void);

		friend dgram_client_socket& operator<<(dgram_client_socket& sock, const char* str);
		friend dgram_client_socket& operator<<(dgram_client_socket& sock, string& str);

		ssize_t snd(const void* buf, size_t len, int flags=0); // flags: send()

		// I
		friend dgram_client_socket& operator>>(dgram_client_socket& sock, string& dest);

		ssize_t rcv(void* buf, size_t len, int flags=0);

		// Getters

		bool getconn(void);
	};

	dgram_client_socket::dgram_client_socket(void) : connected(false) {}

	ssize_t dgram_client_socket::rcv(void* buf, size_t len, int flags)
	{
		ssize_t bytes;

		if ( -1 == sfd )
			throw socket_exception(__FILE__,__LINE__,"dgram_client_socket::rcv() - Socket already closed!\n");

		if ( connected != true )
			throw socket_exception(__FILE__,__LINE__,"dgram_client_socket::rcv() - Socket is not connected!\n");

		if ( -1 == (bytes = recv(sfd,buf,len,flags)) )
			throw socket_exception(__FILE__,__LINE__,"dgram_client_socket::rcv() - recv() failed!\n");

		return bytes;
	}

	dgram_client_socket& operator>>(dgram_client_socket& sock, string& dest)
	{
		ssize_t read_bytes;
		char* buffer;

		buffer = new char[dest.size()];

		if ( sock.sfd == -1 )
			throw socket_exception(__FILE__,__LINE__,">>(dgram_client_socket, std::string) input: Socket not connected!\n");

		if ( -1 == (read_bytes = read(sock.sfd,buffer,dest.size())) )
			throw socket_exception(__FILE__,__LINE__,">>(dgram_client_socket, std::string) input: Error while reading!\n");

		if ( read_bytes < static_cast<ssize_t>(dest.size()) )
			dest.resize(read_bytes); // So the client doesn't print content more than one time
						 // and it can check if the string's length is 0 (end of transmission)

		dest.assign(buffer,read_bytes);

		delete buffer;

		return sock;
	}

	ssize_t dgram_client_socket::snd(const void* buf, size_t len, int flags)
	{
		ssize_t bytes;

		if ( -1 == sfd )
			throw socket_exception(__FILE__,__LINE__,"dgram_client_socket::snd() - Socket already closed!\n");

		if ( connected != true )
			throw socket_exception(__FILE__,__LINE__,"dgram_client_socket::snd() - Socket is not connected!\n");

		if ( -1 == (bytes = send(sfd,buf,len,flags)) )
			throw socket_exception(__FILE__,__LINE__,"dgram_client_socket::snd() - send() failed!\n");

		return bytes;
	}

	dgram_client_socket& operator<<(dgram_client_socket& sock, const char* str)
	{
		if ( sock.sfd == -1 )
			throw socket_exception(__FILE__,__LINE__,"dgram_client_socket <<(const char*) output: Socket not connected!\n");
		if ( str == NULL )
			throw socket_exception(__FILE__,__LINE__,"dgram_client_socket <<(const char*) output: Null buffer given!\n");
		if ( sock.connected == false )
			throw socket_exception(__FILE__,__LINE__,"dgram_client_socket <<(const char*) output: DGRAM socket not connected!\n");

		size_t len = strlen(str);

		if ( -1 == write(sock.sfd,str,len) )
			throw socket_exception(__FILE__,__LINE__,"dgram_client_socket <<(const char*) output: Write failed!\n");

		return sock;
	}

	dgram_client_socket& operator<<(dgram_client_socket& sock, string& str)
	{
		if ( sock.sfd == -1 )
			throw socket_exception(__FILE__,__LINE__,"dgram_client_socket<<(std::string) output: Socket not connected!\n");
		if ( sock.connected == false )
			throw socket_exception(__FILE__,__LINE__,"dgram_client_socket <<(std::string) output: DGRAM socket not connected!\n");
		if ( -1 == write(sock.sfd,str.c_str(),str.size()) )
			throw socket_exception(__FILE__,__LINE__,"dgram_client_socket <<(std::string) output: Write failed!\n");

		return sock;
	}

	// Getters
	bool dgram_client_socket::getconn(void)
	{
		return connected;
	}

}
