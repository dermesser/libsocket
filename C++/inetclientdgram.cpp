# include <iostream>
# include <string>
# include <string.h>

# include "../headers/libinetsocket.h"
# include "../headers++/socket.hpp"
# include "../headers++/inetbase.hpp"
# include "../headers++/inetdgram.hpp"
# include "../headers++/exception.hpp"
# include "../headers++/dgramclient.hpp"

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
 * DESCRIPTION FOR INETCLIENTDGRAM.CPP
 * 	This file provides the class inet_dgram_client which is used
 * 	for internet domain UDP client sockets. You think, in UDP
 * 	there is no difference between client and server? This is
 * 	correct, but in libsocket, the difference is that the UDP
 * 	client sockets may be connected and are not explicitly
 * 	bound to somewhere.
 */

namespace libsocket
{
	using std::string;

/************** inet_dgram class (inet UDP sockets) ************/

	class inet_dgram_client : public inet_dgram, public dgram_client_socket
	{
		public:

		// Only create socket
		inet_dgram_client(int proto_osi3,int flags=0); // Flags: socket()
		// Create socket and connect it
		inet_dgram_client(const char* dsthost, const char* dstport, int proto_osi3, int flags=0); // Flags: socket()

		// actions
		// connect/reconnect
		void connect(const char* dsthost, const char* dstport);
		void deconnect(void);

		// I/Os from dgram_socket and inet_dgram
	};

	// Managing

	inet_dgram_client::inet_dgram_client(int proto_osi3, int flags)
	{
		if ( -1 == (sfd = create_inet_dgram_socket(proto_osi3,flags)) )
			throw socket_exception(__FILE__,__LINE__,"inet_dgram::inet_dgram() - Could not create inet dgram socket!\n");

		proto = proto_osi3;
	}

	inet_dgram_client::inet_dgram_client(const char* dsthost, const char* dstport, int proto_osi3, int flags)
	{
		if ( -1 == (sfd = create_inet_dgram_socket(proto_osi3,flags)) )
			throw socket_exception(__FILE__,__LINE__,"inet_dgram::inet_dgram() - Could not create inet dgram socket!\n");

		try {
			connect(dsthost,dstport);
		} catch (socket_exception exc)
		{
			throw socket_exception(__FILE__,__LINE__,"inet_dgram::inet_dgram() - Could not connect dgram socket\n");
		}
	}

	void inet_dgram_client::connect(const char* dsthost, const char* dstport)
	{
		if ( -1 == (connect_inet_dgram_socket(sfd,dsthost,dstport)) )
			throw socket_exception(__FILE__,__LINE__,"inet_dgram::connect() - Could not connect dgram socket!\n");

		std::cerr << "connecting...\n";
		host = dsthost;
		port = dstport;
		connected = true;
	}

	void inet_dgram_client::deconnect(void)
	{
		if ( -1 == sfd )
			throw socket_exception(__FILE__,__LINE__,"inet_dgram::deconnect() - Socket not connected!\n");
		if ( -1 == (connect_inet_dgram_socket(sfd,0,0)) )
			throw socket_exception(__FILE__,__LINE__,"inet_dgram::deconnect() - Could not deconnect!\n");

		connected = false;
		host.resize(0);
		port.resize(0);
	}
}
