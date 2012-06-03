# include "../headers/libunixsocket.h"
# include "../headers++/exception.hpp"
# include "../headers++/unixbase.hpp"
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
 * DESCRIPTION FOR UNIXDGRAM.CPP
 * 	Provides the basic I/O functions for every UNIX domain
 * 	datagram socket. Every unix datagram socket class is
 * 	derived from this class because sndto and rcvfrom may be
 * 	called on every datagram socket.
 */

namespace libsocket
{
	class unix_dgram : public unix_socket
	{
		public:

		ssize_t sndto(const void* buf, size_t length, const char* path, int sendto_flags=0);
		ssize_t sndto(const void* buf, size_t length, const string& path, int sendto_flags=0);

		ssize_t rcvfrom(void* buf, size_t length, char* source, size_t source_len, int recvfrom_flags=0);
		ssize_t rcvfrom(void* buf, size_t length, string& source, int recvfrom_flags=0);

	};

	// Main sendto function
	ssize_t unix_dgram::sndto(const void* buf, size_t length, const char* path, int sendto_flags)
	{
		if ( buf == NULL )
			throw socket_exception(__FILE__,__LINE__,"unix_dgram::sndto: Buffer is NULL!\n");

		ssize_t bytes;

		if ( 0 > (bytes = sendto_unix_dgram_socket(sfd,buf,length,path,sendto_flags)) )
			throw socket_exception(__FILE__,__LINE__,"unix_dgram::sndto: Could not send data to peer!\n");

		return bytes;
	}

	ssize_t unix_dgram::sndto(const void* buf, size_t length, const string& path, int sendto_flags)
	{
		return sndto(buf,length,path.c_str(),sendto_flags);
	}


	ssize_t unix_dgram::rcvfrom(void* buf, size_t length, char* source, size_t source_len, int recvfrom_flags)
	{
		if ( buf == NULL )
			throw socket_exception(__FILE__,__LINE__,"unix_dgram::rcvfrom: Buffer is NULL!\n");

		ssize_t bytes;

		bytes = recvfrom_unix_dgram_socket(sfd,buf,length,source,source_len,recvfrom_flags);

		if ( bytes < 0 )
			throw socket_exception(__FILE__,__LINE__,"unix_dgram::rcvfrom: Could not receive data from peer!\n");

		return bytes;
	}

	ssize_t unix_dgram::rcvfrom(void* buf, size_t length, string& source, int recvfrom_flags)
	{
		if ( buf == NULL )
			throw socket_exception(__FILE__,__LINE__,"unix_dgram::rcvfrom: Buffer is NULL!\n");

		ssize_t bytes;

		char* source_cstr = new char[512]; // AFAIK, the address field in struct sockaddr_un is only 108 bytes...
		size_t source_cstr_len;

		memset(source_cstr,0,512);

		bytes = recvfrom_unix_dgram_socket(sfd,buf,length,source_cstr,512,recvfrom_flags);

		if ( bytes < 0 )
			throw socket_exception(__FILE__,__LINE__,"unix_dgram::rcvfrom: Could not receive data from peer!\n");

		source_cstr_len = strlen(source_cstr);

		source.resize(source_cstr_len);

		source = source_cstr;

		return bytes;
	}
}
