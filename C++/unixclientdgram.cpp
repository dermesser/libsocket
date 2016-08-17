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

/**
 * @file unixclientdgram.cpp
 * @brief Client for UNIX domain datagram sockets.
 *
 * An important difference to inet_dgram_client is that the
 * path given at the constructors is the /bind path/, not
 * a socket to which the datagram is connected immediately
 * after creation.
 *
 */

# include <exception.hpp>
# include <libunixsocket.h>
# include <unixclientdgram.hpp>

#include <fcntl.h>
#ifndef SOCK_NONBLOCK
# define SOCK_NONBLOCK O_NONBLOCK
#endif

namespace libsocket
{
    /**
     * @brief Set a UNIX domain datagram socket up
     *
     * @param path The path to bind this socket to
     * @param flags Flags for `socket(2)`
     */
    void unix_dgram_client::setup(const char* path, int flags)
    {
	if ( sfd != -1 )
	    throw socket_exception(__FILE__,__LINE__,"unix_dgram_client::unix_dgram_client: Socket has already been set up!",false);

	sfd = create_unix_dgram_socket(path,flags);

	if ( sfd < 0 )
	    throw socket_exception(__FILE__,__LINE__,"unix_dgram_client::unix_dgram_client: Could not create unix dgram client socket!");

	if ( path )
	    _path.assign(path);


	is_nonblocking = flags & SOCK_NONBLOCK;
    }

    /**
     * @brief Constructor with only `socket()` flags
     *
     * @param flags Flags for `socket(2)`
     */
    unix_dgram_client::unix_dgram_client(int flags)
    {
	setup(NULL,flags); // bind to nowhere
    }

    /**
     * @brief Constructor setting the socket up
     *
     * This constructor binds the socket to the given path.
     *
     * @param path Bind path.
     * @param flags Flags for `socket(2)`
     */
    unix_dgram_client::unix_dgram_client(const char* path, int flags)
    {
	setup(path,flags); // bind to path
    }

    /**
     * @brief Constructor setting the socket up
     *
     * This constructor binds the socket to the given path.
     *
     * @param path Bind path.
     * @param flags Flags for `socket(2)`
     */
    unix_dgram_client::unix_dgram_client(const string& path, int flags)
    {
	setup(path.c_str(),flags);
    }

    /**
     * @brief Connect a UNIX datagram socket
     *
     * This function connects a datagram socket; `connect(2)` says the following about this:
     *
     * > If the socket sockfd is of type SOCK_DGRAM then addr is the address to which datagrams are sent by default, and the only address from which datagrams are  received.
     *
     * @param path The path of the socket to connect this socket to.
     */
    void unix_dgram_client::connect(const char* path)
    {
	if ( sfd == -1 )
	    throw socket_exception(__FILE__,__LINE__,"unix_dgram_client::connect() - Socket has already been closed!",false);
	if ( connect_unix_dgram_socket(sfd,path) < 0 )
	    throw socket_exception(__FILE__,__LINE__,"unix_dgram_client::connect: Could not connect dgram socket!");

	_path.assign(path);

	connected = true;
    }

    /**
     * @brief Connect a UNIX datagram socket
     *
     * This function connects a datagram socket; `connect(2)` says the following about this:
     *
     * > If the socket sockfd is of type SOCK_DGRAM then addr is the address to which datagrams are sent by default, and the only address from which datagrams are  received.
     *
     * @param path The path of the socket to connect this socket to.
     */
    void unix_dgram_client::connect(const string& path)
    {
	connect(path.c_str());
    }

    /**
     * @brief Disconnect a UNIX datagram socket
     *
     * Disconnects a previously connected socket.
     *
     */
    void unix_dgram_client::deconnect(void)
    {
	if ( connect_unix_dgram_socket(sfd,0) < 0 )
	    throw socket_exception(__FILE__,__LINE__,"unix_dgram_client::deconnect: Could not disconnect dgram socket!");

	_path.clear();

	connected = false;
    }
}
