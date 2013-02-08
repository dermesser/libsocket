# include <string>
# include <string.h>

# include <unistd.h>
# include <sys/socket.h>
# include <sys/types.h>

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
 * DESCRIPTION FOR UNIXCLIENTSTREAM.CPP
 * 	unix_stream_client is the client socket
 * 	for UNIX domain SOCK_STREAM client sockets.
 * 	You may create and connect the socket after
 * 	instantiation using connect().
 */

# include "../headers/libunixsocket.h"
# include "../headers/exception.hpp"
# include "../headers/unixclientstream.hpp"

namespace libsocket
{
    using std::string;

    unix_stream_client::unix_stream_client(void) {}
    // Server path!
    unix_stream_client::unix_stream_client(const char* path, int socket_flags)
    {
	connect(path,socket_flags);
    }

    unix_stream_client::unix_stream_client(const string& path, int socket_flags)
    {
	connect(path.c_str(),socket_flags);
    }

    void unix_stream_client::connect(const char* path, int socket_flags)
    {
	if ( sfd != -1 )
	    throw socket_exception(__FILE__,__LINE__,"unix_stream_client::connect: Already connected!\n");

	sfd = create_unix_stream_socket(path,socket_flags);

	_path.assign(path);

	if ( sfd < 0 )
	    throw socket_exception(__FILE__,__LINE__,"unix_stream_client::unix_stream_client: Could not create and connect UNIX socket!\n");

	// New file descriptor, therefore reset shutdown flags
	shut_rd = false;
	shut_wr = false;
    }

    void unix_stream_client::connect(const string& path, int socket_flags)
    {
	connect(path.c_str(),socket_flags);
    }
}
