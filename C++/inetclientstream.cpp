# include <iostream>
# include <string>
# include <string.h>

# include "../headers/libinetsocket.h"
# include "../headers/socket.hpp"
# include "../headers/inetbase.hpp"
# include "../headers/exception.hpp"
# include "../headers/streamclient.hpp"

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

/*
 * DESCRIPTION FOR INETCLIENTSTREAM.CPP
 * 	inetclientstream.cpp provides the class inet_stream
 * 	(which should actually be called inet_stream_client).
 * 	This class is used to communicate with TCP servers,
 * 	like HTTP-, SMTP-, POP3-, FTP-, telnet-Servers.
 *
 * 	The I/O abilities are inherited from stream_client_socket.
 */

namespace libsocket
{
	using std::string;

/**************** inet_stream class (TCP inet sockets) *********/

	class inet_stream : public inet_socket, public stream_client_socket
	{
		public:

		inet_stream(void);
		inet_stream(const char* dsthost, const char* dstport, int proto_osi3, int flags=0); // flags: socket()
		inet_stream(const string& dsthost, const string& dstport, int proto_osi3, int flags=0);

		// connect() == setup()
		void connect(const char* dsthost, const char* dstport, int proto_osi3, int flags=0); // flags: socket()
		void connect(const string& dsthost, const string& dstport, int proto_osi3, int flags);

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

		// New file descriptor, therefore reset shutdown flags
		shut_rd = false;
		shut_wr = false;
	}

	void inet_stream::connect(const string& dsthost, const string& dstport, int proto_osi3, int flags)
	{
		connect(dsthost.c_str(),dstport.c_str(),proto_osi3,flags);
	}
}
