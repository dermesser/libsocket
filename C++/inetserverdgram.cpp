# include <string.h>
# include <string>

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
 * @file inetserverdgram.cpp
 * @brief Contains class for creating a bound datagram socket.
 *
 * inet_dgram_server provides nothing more than a constructor
 * which binds the UDP socket to the specified host. Everything
 * other, e.g. the I/O functions like rcvfrom and sndto are
 * inherited from inet_dgram.
 */

# include <libinetsocket.h>
# include <exception.hpp>
# include <inetserverdgram.hpp>

#include <fcntl.h>
#ifndef SOCK_NONBLOCK
# define SOCK_NONBLOCK O_NONBLOCK
#endif

namespace libsocket
{
    using std::string;

    /**
     * @brief Create datagram socket and bind it.
     *
     * @param host Bind address (Wildcard: "0.0.0.0"/"::")
     * @param port Bind port
     * @param proto_osi3 `LIBSOCKET_IPv4` or `LIBSOCKET_IPv6` or `LIBSOCKET_BOTH`
     * @param flags Flags for `socket(2)`
     */
    inet_dgram_server::inet_dgram_server(const char* host, const char* port, int proto_osi3, int flags)
    {
	setup(host,port,proto_osi3,flags);
    }

    /**
     * @brief Create datagram socket and bind it.
     *
     * @param host Bind address (Wildcard: "0.0.0.0"/"::")
     * @param port Bind port
     * @param proto_osi3 `LIBSOCKET_IPv4` or `LIBSOCKET_IPv6` or `LIBSOCKET_BOTH`
     * @param flags Flags for `socket(2)`
     */
    inet_dgram_server::inet_dgram_server(const string& host, const string& port, int proto_osi3, int flags)
    {
	setup(host,port,proto_osi3,flags);
    }

    /**
     * @brief Set up socket. **NOT FOR EXTERNAL USE**
     *
     * @param bhost Bind address (Wildcard: "0.0.0.0"/"::")
     * @param bport Bind port
     * @param proto_osi3 `LIBSOCKET_IPv4` or `LIBSOCKET_IPv6` or `LIBSOCKET_BOTH`
     * @param flags Flags for `socket(2)`
     */
    void inet_dgram_server::setup(const char* bhost, const char* bport, int proto_osi3, int flags)
    {
	// No separate call to get_address_family()

	if ( -1 == (sfd = create_inet_server_socket(bhost,bport,LIBSOCKET_UDP,proto_osi3,flags)) )
	    throw socket_exception(__FILE__,__LINE__,"inet_dgram_server::inet_dgram_server() - could not create server socket!");

	host = string(bhost);
	port = string(bport);
	is_nonblocking = flags & SOCK_NONBLOCK;
    }

    /**
     * @brief Set up socket. **NOT FOR EXTERNAL USE**
     *
     * @param bhost Bind address (Wildcard: "0.0.0.0"/"::")
     * @param bport Bind port
     * @param proto_osi3 `LIBSOCKET_IPv4` or `LIBSOCKET_IPv6` or `LIBSOCKET_BOTH`
     * @param flags Flags for `socket(2)`
     */
    void inet_dgram_server::setup(const string& bhost, const string& bport, int proto_osi3, int flags)
    {
	setup(bhost.c_str(),bport.c_str(),proto_osi3,flags);
    }
}
