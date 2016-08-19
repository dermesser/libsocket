
# include <sys/types.h>
# include <sys/socket.h>

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
 * @file unixserverdgram.cpp
 * @brief Class for UNIX domain datagram servers
 *
 * The only difference to "client" datagram socket is that this socket type is not `connect()`-able.
 */

# include <libunixsocket.h>
# include <exception.hpp>
# include <unixserverdgram.hpp>

#include <fcntl.h>
#ifndef SOCK_NONBLOCK
# define SOCK_NONBLOCK O_NONBLOCK
#endif


namespace libsocket
{
    using std::string;

    /**
     * @brief Void constructor.
     */
    unix_dgram_server::unix_dgram_server(void)
	: bound(false)
    { }

    /**
     * @brief Constructs and binds a socket
     *
     * Constructs a socket and binds it to `bindpath`. Then calls `listen()`.
     *
     * @param bindpath Bind path.
     * @param socket_flags Flags for `socket(2)`
     */
    unix_dgram_server::unix_dgram_server(const char* bindpath, int socket_flags)
    {
	setup(bindpath,socket_flags);
    }

    /**
     * @brief Constructs and binds a socket
     *
     * Constructs a socket and binds it to `bindpath`. Then calls `listen()`.
     *
     * @param bindpath Bind path.
     * @param socket_flags Flags for `socket(2)`
     */
    unix_dgram_server::unix_dgram_server(const string& bindpath, int socket_flags)
    {
	setup(bindpath.c_str(),socket_flags);
    }

    /**
     * @brief Binds a socket
     *
     * Binds a socket. (Also calls `listen()`)
     *
     * @param bindpath Bind path.
     * @param socket_flags Flags for `socket(2)`
     */
    void unix_dgram_server::setup(const char* bindpath, int socket_flags)
    {
	if ( sfd != -1 )
	    throw socket_exception(__FILE__,__LINE__,"unix_dgram_server::setup: Already set up!",false);

	sfd = create_unix_server_socket(bindpath, LIBSOCKET_DGRAM, socket_flags);

	if ( sfd < 0 )
	    throw socket_exception(__FILE__,__LINE__,"unix_dgram_server::setup: Could not create server!");

	_path.assign(bindpath);
	bound = true;
	is_nonblocking = socket_flags & SOCK_NONBLOCK;
    }

    /**
     * @brief Binds a socket
     *
     * Binds a socket. (Also calls `listen()`)
     *
     * @param bindpath Bind path.
     * @param socket_flags Flags for `socket(2)`
     */
    void unix_dgram_server::setup(const string& bindpath, int socket_flags)
    {
	setup(bindpath.c_str(),socket_flags);
    }
}
