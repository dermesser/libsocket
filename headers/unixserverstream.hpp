#ifndef LIBSOCKET_UNIXSERVERSTREAM_H_DBD900942A364273A5E9C89F0096D9AF
#define LIBSOCKET_UNIXSERVERSTREAM_H_DBD900942A364273A5E9C89F0096D9AF

# include <memory>
# include <string>

# include "unixbase.hpp"
# include "unixclientstream.hpp"

/**
 * @file unixserverstream.hpp
 *
 * UNIX-domain STREAM server socket class
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
    using std::unique_ptr;

    /** @addtogroup libsocketplusplus
     * @{
     */
    /**
     * @brief Socket class for UNIX-domain STREAM socket server.
     */
    class unix_stream_server : public unix_socket
    {
	public:

	    unix_stream_server(void);
	    unix_stream_server(const char* path, int flags=0);
	    unix_stream_server(const string& path, int flags=0);

	    void setup(const char* path, int flags=0);
	    void setup(const string& path, int flags=0);

	    unix_stream_client* accept(int flags=0);
            unique_ptr<unix_stream_client> accept2(int flags=0);
    };
    /**
     * @}
     */
}
# endif
