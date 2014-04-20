# include <iostream>
# include <string>
# include <string.h>

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

/**
 * @file inetclientstream.cpp
 * @brief TCP/IP socket class.
 *
 * 	inetclientstream.cpp provides the class inet_stream
 * 	(which should actually be called inet_stream_client).
 * 	This class is used to communicate with TCP servers,
 * 	like HTTP-, SMTP-, POP3-, FTP-, telnet-Servers.
 *
 * 	The I/O abilities are inherited from stream_client_socket.
 */

# include <libinetsocket.h>
# include <exception.hpp>
# include <inetclientstream.hpp>

namespace libsocket
{
    using std::string;

    /// Void constructor; call connect() before using the socket!
    inet_stream::inet_stream(void)
    {
    }

    /**
     * @brief Connecting constructor
     *
     * Creates TCP/IP client socket and connects.
     *
     * @param dsthost Remote host
     * @param dstport Remote port
     * @param proto_osi3 `LIBSOCKET_IPv4` or `LIBSOCKET_IPv6` or `LIBSOCKET_BOTH`
     * @param flags Flags for `socket(2)`
     */
    inet_stream::inet_stream(const char* dsthost, const char* dstport, int proto_osi3, int flags)
    {
	connect(dsthost,dstport,proto_osi3,flags);
    }

    /**
     * @brief Connecting constructor
     *
     * Creates TCP/IP client socket and connects.
     *
     * @param dsthost Remote host
     * @param dstport Remote port
     * @param proto_osi3 `LIBSOCKET_IPv4` or `LIBSOCKET_IPv6` or `LIBSOCKET_BOTH`
     * @param flags Flags for `socket(2)`
     */
    inet_stream::inet_stream(const string& dsthost, const string& dstport, int proto_osi3, int flags)
    {
	connect(dsthost.c_str(),dstport.c_str(),proto_osi3,flags);
    }

    /**
     * @brief Set up socket if not already done.
     *
     * Creates TCP/IP client socket and connects. Fails if the socket is already set up.
     *
     * @param dsthost Remote host
     * @param dstport Remote port
     * @param proto_osi3 `LIBSOCKET_IPv4` or `LIBSOCKET_IPv6` or `LIBSOCKET_BOTH`
     * @param flags Flags for `socket(2)`
     */
    void inet_stream::connect(const char* dsthost, const char* dstport, int proto_osi3, int flags)
    {
	if ( sfd != -1 )
	    throw socket_exception(__FILE__,__LINE__,"inet_stream::connect() - Already connected!",false);

	sfd = create_inet_stream_socket(dsthost,dstport,proto_osi3,flags);

	if ( sfd < 0 )
	    throw socket_exception(__FILE__,__LINE__,"inet_stream::connect() - Could not create socket");

	host = dsthost;
	port = dstport;
	proto = proto_osi3;

	// New file descriptor, therefore reset shutdown flags
	shut_rd = false;
	shut_wr = false;
    }

    /**
     * @brief Set up socket if not already done.
     *
     * Creates TCP/IP client socket and connects. Fails if the socket is already set up.
     *
     * @param dsthost Remote host
     * @param dstport Remote port
     * @param proto_osi3 `LIBSOCKET_IPv4` or `LIBSOCKET_IPv6` or `LIBSOCKET_BOTH`
     * @param flags Flags for `socket(2)`
     */
    void inet_stream::connect(const string& dsthost, const string& dstport, int proto_osi3, int flags)
    {
	connect(dsthost.c_str(),dstport.c_str(),proto_osi3,flags);
    }
}
