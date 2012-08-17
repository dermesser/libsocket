# include "../headers/exception.hpp"
# include "../headers/socket.hpp"
# include "../headers/unixbase.hpp"
# include "../headers/unixdgram.hpp"
# include "../headers/dgramclient.hpp"
# include "../headers/libunixsocket.h"
# include <string>

using std::string;

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
 * DESCRIPTION FOR UNIXCLIENTDGRAM.CPP
 * 	Client for UNIX domain datagram sockets.
 * 	An important difference to inet_dgram_client is that the
 * 	path given at the constructors is the /bind path/, not
 * 	a socket to which the datagram is connected immediately
 * 	after creation.
 */

# include "../headers/unixclientdgram.hpp"

namespace libsocket
{
	//				Bind path, not connect!
	void unix_dgram_client::setup(const char* path, int flags)
	{
		if ( sfd != -1 )
			throw socket_exception(__FILE__,__LINE__,"unix_dgram_client::unix_dgram_client: Socket is already set up!\n");
		sfd = create_unix_dgram_socket(path,flags);

		if ( path )
			_path.assign(path);

		if ( sfd < 0 )
			throw socket_exception(__FILE__,__LINE__,"unix_dgram_client::unix_dgram_client: Could not create unix dgram client socket!\n");

	}

	unix_dgram_client::unix_dgram_client(int flags)
	{
		setup(0,flags); // bind to nowhere
	}

	unix_dgram_client::unix_dgram_client(const char* path, int flags)
	{
		setup(path,flags); // bind to path
	}

	unix_dgram_client::unix_dgram_client(const string& path, int flags)
	{
		setup(path.c_str(),flags);
	}

	void unix_dgram_client::connect(const char* path)
	{
		if ( sfd == -1 )
			throw socket_exception(__FILE__,__LINE__,"unix_dgram_client::connect() - Socket has already been closed!\n");
		if ( connect_unix_dgram_socket(sfd,path) < 0 )
			throw socket_exception(__FILE__,__LINE__,"unix_dgram_client::connect: Could not connect dgram socket!\n");

		connected = true;
	}

	void unix_dgram_client::connect(const string& path)
	{
		connect(path.c_str());
	}

	void unix_dgram_client::deconnect(void)
	{
		if ( connect_unix_dgram_socket(sfd,0) < 0 )
			throw socket_exception(__FILE__,__LINE__,"unix_dgram_client::deconnect: Could not disconnect dgram socket!\n");

		_path.clear();

		connected = false;
	}
}
