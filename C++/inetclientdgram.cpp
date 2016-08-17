# include <iostream>
# include <string>
# include <cstring>

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

/**
 * @file inetclientdgram.cpp
 * @brief Class for connectable UDP sockets
 *
 * 	This file provides the class inet_dgram_client which is used
 * 	for internet domain UDP client sockets. You think, in UDP
 * 	there is no difference between client and server? This is
 * 	correct, but in libsocket, the difference is that the UDP
 * 	client sockets may be connected and are not explicitly
 * 	bound to somewhere.
 */

# include <libinetsocket.h>
# include <exception.hpp>
# include <inetclientdgram.hpp>

#include <fcntl.h>
#ifndef SOCK_NONBLOCK
# define SOCK_NONBLOCK O_NONBLOCK
#endif

namespace libsocket
{
    using std::string;

    // Constructors

    /**
     * @brief Create normal datagram socket (connectable).
     *
     * @param proto_osi3 `LIBSOCKET_IPv4` or `LIBSOCKET_IPv6` or `LIBSOCKET_BOTH`
     * @param flags Flags for `socket(2)`.
     */
    inet_dgram_client::inet_dgram_client(int proto_osi3,int flags)
    {
	setup(proto_osi3,flags);
    }

    /**
     * @brief Create datagram socket and connect it immediately to the given host and port.
     *
     * @param dsthost Remote host name
     * @param dstport Remote port
     * @param proto_osi3 `LIBSOCKET_IPv4` or `LIBSOCKET_IPv6` or `LIBSOCKET_BOTH`
     * @param flags Flags for `socket(2)`
     */
    inet_dgram_client::inet_dgram_client(const char* dsthost, const char* dstport, int proto_osi3, int flags)
    {
	setup(dsthost,dstport,proto_osi3,flags);
    }

    /**
     * @brief Create datagram socket and connect it immediately to the given host and port.
     *
     * @param dsthost Remote host name
     * @param dstport Remote port
     * @param proto_osi3 `LIBSOCKET_IPv4` or `LIBSOCKET_IPv6` or `LIBSOCKET_BOTH`
     * @param flags Flags for `socket(2)`
     */
    inet_dgram_client::inet_dgram_client(const string& dsthost, const string& dstport, int proto_osi3, int flags)
    {
	setup(dsthost,dstport,proto_osi3,flags);
    }

    /**
     * @brief Set up normal datagram socket (connectable). [NOT FOR EXTERNAL USE]
     *
     * @param proto_osi3 `LIBSOCKET_IPv4` or `LIBSOCKET_IPv6` or `LIBSOCKET_BOTH`
     * @param flags Flags for `socket(2)`.
     */
    void inet_dgram_client::setup(int proto_osi3, int flags)
    {
	if ( -1 == (sfd = create_inet_dgram_socket(proto_osi3,flags)) )
	    throw socket_exception(__FILE__,__LINE__,"inet_dgram_client::inet_dgram_client() - Could not create inet dgram socket!");
	proto = proto_osi3;

	is_nonblocking = flags & SOCK_NONBLOCK;
    }

    /**
     * @brief Set up datagram socket and connect it immediately to the given host and port. [NOT FOR EXTERNAL USE]
     *
     * @param dsthost Remote host name
     * @param dstport Remote port
     * @param proto_osi3 `LIBSOCKET_IPv4` or `LIBSOCKET_IPv6` or `LIBSOCKET_BOTH`
     * @param flags Flags for `socket(2)`
     */
    void inet_dgram_client::setup(const char* dsthost, const char* dstport, int proto_osi3, int flags)
    {
	// Retrieve address family
	if (proto_osi3 == LIBSOCKET_BOTH)
	    proto_osi3 = get_address_family(dsthost);

	if ( -1 == (sfd = create_inet_dgram_socket(proto_osi3,flags)) )
	    throw socket_exception(__FILE__,__LINE__,"inet_dgram_client::inet_dgram_client() - Could not create inet dgram socket!");

	inet_dgram_client::connect(dsthost,dstport);

	proto = proto_osi3;
	is_nonblocking = flags & SOCK_NONBLOCK;
    }

    /**
     * @brief Set up datagram socket and connect it immediately to the given host and port. [NOT FOR EXTERNAL USE]
     *
     * @param dsthost Remote host name
     * @param dstport Remote port
     * @param proto_osi3 `LIBSOCKET_IPv4` or `LIBSOCKET_IPv6` or `LIBSOCKET_BOTH`
     * @param flags Flags for `socket(2)`
     */
    void inet_dgram_client::setup(const string& dsthost, const string& dstport, int proto_osi3, int flags)
    {
	setup(dsthost.c_str(),dstport.c_str(),proto_osi3,flags);
    }

    /**
     * @brief Connect datagram socket.
     *
     * Connect a datagram socket to a remote peer so only its packets are received
     * and all data written is sent to it.
     *
     * @param dsthost Destination host
     * @param dstport Destination port
     */
    void inet_dgram_client::connect(const char* dsthost, const char* dstport)
    {
	if ( sfd == -1 )
	    throw socket_exception(__FILE__,__LINE__,"inet_dgram_client::connect() - Socket has already been closed!",false);
	if ( -1 == (connect_inet_dgram_socket(sfd,dsthost,dstport)) )
	    throw socket_exception(__FILE__,__LINE__,"inet_dgram_client::connect() - Could not connect dgram socket! (Maybe this socket has a wrong address family?)");

	host = dsthost;
	port = dstport;
	connected = true;
    }

    /**
     * @brief Connect datagram socket.
     *
     * Connect a datagram socket to a remote peer so only its packets are received
     * and all data written is sent to it.
     *
     * @param dsthost Destination host
     * @param dstport Destination port
     */
    void inet_dgram_client::connect(const string& dsthost, const string& dstport)
    {
	if ( sfd == -1 )
	    throw socket_exception(__FILE__,__LINE__,"inet_dgram_client::connect() - Socket has already been closed!",false);
	if ( -1 == (connect_inet_dgram_socket(sfd,dsthost.c_str(),dstport.c_str())) )
	    throw socket_exception(__FILE__,__LINE__,"inet_dgram_client::connect() - Could not connect dgram socket! (Maybe this socket has a wrong address family?)");

	host = dsthost;
	port = dstport;
	connected = true;
    }

    /*
     * @brief Break association to host. Does not close the socket.
     *
     * *Should actually be called 'disconnect'*
     *
     */
    void inet_dgram_client::deconnect(void)
    {
	if ( -1 == (connect_inet_dgram_socket(sfd,NULL,NULL)) )
	    throw socket_exception(__FILE__,__LINE__,"inet_dgram_client::deconnect() - Could not disconnect!");

	connected = false;
	host.clear();
	port.clear();
    }
}
