#ifndef LIBSOCKET_UNIXCLIENTDGRAM_H_1A0CF567F9A946AA80C64654F35DFA9E
#define LIBSOCKET_UNIXCLIENTDGRAM_H_1A0CF567F9A946AA80C64654F35DFA9E

# include "unixdgram.hpp"
# include "dgramclient.hpp"

# include <string>

/**
 * @file unixclientdgram.hpp
 *
 * Contains the class for use of UNIX-domain datagram sockets.
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

using std::string;

namespace libsocket
{
    /** @addtogroup libsocketplusplus
     * @{
     */
    /**
     * @brief Class for UNIX datagram sockets
     */
    class unix_dgram_client : public unix_dgram, public dgram_client_socket
    {
	public:

	    unix_dgram_client(int flags=0);
	    unix_dgram_client(const char* path, int flags=0);
	    unix_dgram_client(const string& path, int flags=0);

	    void setup(const char* path, int flags=0);

	    void connect(const char* path);
	    void connect(const string& path);

	    void deconnect(void);
    };
    /**
     * @}
     */
}

# endif
