# include <iostream>
# include <string>
# include <string.h>

# include "../headers/libinetsocket.h"
# include "../headers++/socket.hpp"
# include "../headers++/inetbase.hpp"

# include <unistd.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <stdio.h>

// oo wrapper around libinetsocket
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

/************** inet_dgram class (inet UDP sockets) ************/

	class inet_dgram : public inet_socket
	{
		public:

		// I/O
		// O
		ssize_t sndto(const void* buf, size_t len, const char* dsthost, const char* dstport, int sndto_flags=0); // flags: sendto()

		// I
		ssize_t rcvfrom(void* buf, size_t len, char* srchost, size_t hostlen, char* srcport, size_t portlen, int rcvfrom_flags=0, bool numeric=false);
		ssize_t rcvfrom(void* buf, size_t len, string& srchost, string& srcport, int rcvfrom_flags=0, bool numeric=false);

		// Getters
	};

	// I/O

	// I

	ssize_t inet_dgram::rcvfrom(void* buf, size_t len, char* hostbuf, size_t hostbuflen, char* portbuf, size_t portbuflen, int rcvfrom_flags, bool numeric)
	{
		ssize_t bytes;
		int num = ((numeric == true) ? NUMERIC : 0);

		if ( -1 == sfd )
			throw inet_exception(__FILE__,__LINE__,"inet_dgram::rcvfrom() - Socket already closed!\n");

		if ( -1 == (bytes = recvfrom_inet_dgram_socket(sfd,buf,len,hostbuf,hostbuflen,portbuf,portbuflen,rcvfrom_flags,num)) )
			throw inet_exception(__FILE__,__LINE__,"inet_dgram::rcvfrom() - recvfrom() failed!\n");

		return bytes;
	}


	ssize_t inet_dgram::rcvfrom(void* buf, size_t len, string& srchost, string& srcport, int rcvfrom_flags, bool numeric)
	{
		ssize_t bytes;

		char* host = new char[1024];
		char* port = new char[64];

		memset(host,0,1024);
		memset(port,0,64);

		bytes = rcvfrom(buf,len,host,1024,port,64,rcvfrom_flags,numeric);

		srchost.resize(strlen(host));
		srcport.resize(strlen(port));

		srchost = host;
		srcport = port;

		delete[] host;
		delete[] port;

		return bytes;
	}

	// O

	ssize_t inet_dgram::sndto(const void* buf, size_t len, const char* dsthost, const char* dstport, int sndto_flags)
	{
		ssize_t bytes;

		if ( -1 == sfd )
			throw inet_exception(__FILE__,__LINE__,"inet_dgram::sendto() - Socket already closed!\n");

		if ( -1 == (bytes = sendto_inet_dgram_socket(sfd,buf,len,dsthost,dstport,sndto_flags)) )
			throw inet_exception(__FILE__,__LINE__,"inet_dgram::sndto() - Error at sendto\n");

		return bytes;
	}
}
