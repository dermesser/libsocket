# include <iostream>
# include <string>
# include <string.h>

# include "../headers/libinetsocket.h"
# include "../headers/socket.hpp"
# include "../headers/inetbase.hpp"
# include "../headers/exception.hpp"

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

/*
 * DESCRIPTION FOR INETDGRAM.CPP
 * 	The class inet_dgram provides that functions which
 * 	are used for all UDP sockets: rcvfrom and sndto. These
 * 	may be used for both client and server UDP sockets.
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
		ssize_t sndto(const void* buf, size_t len, const string& dsthost, const string& dstport, int sndto_flags=0);

		ssize_t sndto(const string& buf, const string& dsthost, const string& dstport, int sndto_flags=0);

		// I
		ssize_t rcvfrom(void* buf, size_t len, char* srchost, size_t hostlen, char* srcport, size_t portlen, int rcvfrom_flags=0, bool numeric=false);
		ssize_t rcvfrom(void* buf, size_t len, string& srchost, string& srcport, int rcvfrom_flags=0, bool numeric=false);

		ssize_t rcvfrom(string& buf, string& srchost, string& srcport, int rcvfrom_flags=0, bool numeric=false);

		// Getters
	};

	// I/O

	// I

	ssize_t inet_dgram::rcvfrom(void* buf, size_t len, char* hostbuf, size_t hostbuflen, char* portbuf, size_t portbuflen, int rcvfrom_flags, bool numeric)
	{
		ssize_t bytes;
		int num = ((numeric == true) ? NUMERIC : 0);

		if ( -1 == sfd )
			throw socket_exception(__FILE__,__LINE__,"inet_dgram::rcvfrom() - Socket already closed!\n");

		if ( -1 == (bytes = recvfrom_inet_dgram_socket(sfd,buf,len,hostbuf,hostbuflen,portbuf,portbuflen,rcvfrom_flags,num)) )
			throw socket_exception(__FILE__,__LINE__,"inet_dgram::rcvfrom() - recvfrom() failed!\n");

		return bytes;
	}


	ssize_t inet_dgram::rcvfrom(void* buf, size_t len, string& srchost, string& srcport, int rcvfrom_flags, bool numeric)
	{
		ssize_t bytes;

		char* from_host = new char[1024]; // Let's say, that's enough
		char* from_port = new char[32];

		memset(from_host,0,1024);
		memset(from_port,0,32);
		memset(buf,0,len);

		bytes = rcvfrom(buf,len,from_host,1024,from_port,64,rcvfrom_flags,numeric);

		srchost.resize(strlen(from_host));
		srcport.resize(strlen(from_port));

		srchost = from_host;
		srcport = from_port;

		delete[] from_host;
		delete[] from_port;

		return bytes;
	}

	ssize_t inet_dgram::rcvfrom(string& buf, string& srchost, string& srcport, int rcvfrom_flags, bool numeric)
	{
		ssize_t bytes;

		char* cbuf = new char[buf.size()];

		memset(cbuf,0,buf.size());

		bytes = rcvfrom(cbuf,static_cast<size_t>(buf.size()),srchost,srcport,rcvfrom_flags,numeric); // calling inet_dgram::rcvfrom(void*, size_t, string&, string&, int, bool)

		buf.resize(bytes);

		buf = cbuf;

		delete[] cbuf;

		return bytes;
	}

	// O

	ssize_t inet_dgram::sndto(const void* buf, size_t len, const char* dsthost, const char* dstport, int sndto_flags)
	{
		ssize_t bytes;

		if ( -1 == sfd )
			throw socket_exception(__FILE__,__LINE__,"inet_dgram::sendto() - Socket already closed!\n");

		if ( -1 == (bytes = sendto_inet_dgram_socket(sfd,buf,len,dsthost,dstport,sndto_flags)) )
			throw socket_exception(__FILE__,__LINE__,"inet_dgram::sndto() - Error at sendto\n");

		return bytes;
	}

	ssize_t inet_dgram::sndto(const void* buf, size_t len, const string& dsthost, const string& dstport, int sndto_flags)
	{
		ssize_t bytes;

		bytes = sndto(buf,len,dsthost.c_str(),dstport.c_str(),sndto_flags);

		return bytes;
	}

	ssize_t inet_dgram::sndto(const string& buf, const string& dsthost, const string& dstport, int sndto_flags)
	{
		ssize_t bytes;

		bytes = sndto(buf.c_str(),buf.size(),dsthost.c_str(),dstport.c_str(),sndto_flags);

		return bytes;
	}
}
