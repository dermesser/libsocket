#ifndef LIBSOCKET_UNIXCLIENTSTREAM_H_9C64A16FB22D46FC9436989C1752F0CF
#define LIBSOCKET_UNIXCLIENTSTREAM_H_9C64A16FB22D46FC9436989C1752F0CF

# include <string>

# include "streamclient.hpp"
# include "unixbase.hpp"

/**
 * @file unixclientstream.hpp
 *
 * Contains the class for use of UNIX-domain stream sockets as client.
 */
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

namespace libsocket
{
    using std::string;

    /** @addtogroup libsocketplusplus
     * @{
     */
    /**
     * @brief Provides an interface for working with UNIX STREAM sockets
     */
    class unix_stream_client : public unix_socket, public stream_client_socket
    {
	public:

	    unix_stream_client(void);
	    unix_stream_client(const char* path, int socket_flags=0);
	    unix_stream_client(const string& path, int socket_flags=0);

	    void connect(const char* path, int socket_flags=0);
	    void connect(const string& path, int socket_flags=0);

	    friend class unix_stream_server; ///< unix_stream_server returns pointer to unix_stream_client objects when accepting connections.
    };
    /**
     * @}
     */
}
# endif
