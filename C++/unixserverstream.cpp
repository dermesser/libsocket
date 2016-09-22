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
 * @file unixserverstream.cpp
 * @brief Provides a UNIX domain STREAM server.
 *
 *  The path given at the constructors is the bind
 *  path. accept() accepts a new connection and returns
 *  (like inet_stream_server) a pointer to a dynamically allocated
 *  instance of unix_stream_client which may be used to
 *  communicate with the connected client.
 */

# include <libunixsocket.h>
# include <exception.hpp>
# include <unixserverstream.hpp>

namespace libsocket
{
    using std::string;

    /**
     * @brief Void constructor
     */
    unix_stream_server::unix_stream_server(void)
    {
    }

    /**
     * @brief Constructs and binds server socket.
     *
     * @param path Bind path.
     * @param flags Flags for `socket(2)`
     */
    unix_stream_server::unix_stream_server(const char* path, int flags)
    {
	setup(path,flags);
    }

    /**
     * @brief Constructs and binds server socket.
     *
     * @param path Bind path.
     * @param flags Flags for `socket(2)`
     */
    unix_stream_server::unix_stream_server(const string& path, int flags)
    {
	setup(path,flags);
    }

    /**
     * @brief Sets a server socket up.
     *
     * @param path Bind path.
     * @param flags Flags for `socket(2)`
     */
    void unix_stream_server::setup(const char* path, int flags)
    {
	if ( sfd != -1 )
	    throw socket_exception(__FILE__,__LINE__,"unix_stream_server::setup: Socket already set up!",false);
	if ( path == NULL )
	    throw socket_exception(__FILE__,__LINE__,"unix_stream_server::setup: Path is NULL!",false);

	sfd = create_unix_server_socket(path,LIBSOCKET_STREAM,flags);

	if ( sfd < 0 )
	    throw socket_exception(__FILE__,__LINE__,"unix_stream_server::setup: Error at creating UNIX stream server socket!");

	_path.assign(path);
    }

    /**
     * @brief Sets a server socket up.
     *
     * @param path Bind path.
     * @param flags Flags for `socket(2)`
     */
    void unix_stream_server::setup(const string& path, int flags)
    {
	setup(path.c_str(),flags);
    }

    /**
     * @brief Accepts incoming connections on a UNIX domain stream server socket.
     *
     * @param flags Flags for Linux' `accept4()`; useless on other implementations.
     */
    unix_stream_client* unix_stream_server::accept(int flags)
    {
	int cfd;

	if ( sfd == -1 )
	    throw socket_exception(__FILE__,__LINE__,"unix_stream_server::accept: Socket not set up yet!",false);

	unix_stream_client* client = new unix_stream_client;

	cfd = accept_unix_stream_socket(sfd,flags);

	if ( cfd < 0 )
	{
	    if ( is_nonblocking && errno == EWOULDBLOCK )
		return nullptr;
	    else
		throw socket_exception(__FILE__,__LINE__,"unix_stream_server::accept: Error at accepting new connection!");
	}

	client->sfd = cfd;

	return client;
    }

    /**
     * @brief Accepts an incoming connection on a UNIX domain stream server socket and returns an owned pointer.
     *
     * The owned pointer will be closed on destruction.
     *
     * @param flags Flags for `accept4()`; useless on other implementations.
     */
    unique_ptr<unix_stream_client> unix_stream_server::accept2(int flags)
    {
        return unique_ptr<unix_stream_client>(accept(flags));
    }
}
