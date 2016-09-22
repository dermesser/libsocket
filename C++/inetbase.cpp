# include <string>
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

/**
 *  @file inetbase.cpp
 *  @brief The base class for all internet-based sockets.
 *
 * 	inetbase.cpp defines the class inet_socket. This class
 * 	is used as base class in the hierarchy between socket and
 * 	the next specific class. Inheriting classes are inet_dgram,
 * 	inet_stream_client and inet_stream_server.
 *
 * 	The class contains the data elements host and port. If the inheriting
 * 	class is a client, like inet_stream_client or inet_dgram_client (via inet_dgram),
 * 	this fields contain the remote peer. If the inheriting class is a server socket,
 * 	this fields contain the address and the port to which the server is bound.
 *
 * 	The getters, gethost() and getport() may be used for a
 * 	server application to get the paramters of the remote peer.
 */

# include <inetbase.hpp>

namespace libsocket
{
    using std::string;

    inet_socket::inet_socket() : host(""), port("") {}

    /**
     * For sockets behaving as client: Returns the remote host.
     * For sockets behaving as server: Returns the address bound to.
     */
    const string& inet_socket::gethost(void) const
    {
	return host;
    }

    /**
     * For sockets behaving as client: Returns the remote port.
     * For sockets behaving as server: Returns the port bound to.
     */
    const string& inet_socket::getport(void) const
    {
	return port;
    }
}
