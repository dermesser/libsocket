#ifndef LIBSOCKET_UNIXSERVERDGRAM_H_110A6B56F8A4414DAB6BE7B527FDA490
#define LIBSOCKET_UNIXSERVERDGRAM_H_110A6B56F8A4414DAB6BE7B527FDA490

# include "unixdgram.hpp"

# include <string>

/**
 * @file unixserverdgram.hpp
 *
 * Contains the class for bound UNIX-domain datagram sockets.
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
     * @brief Provides an interface to UNIX-domain datagram sockets.
     *
     * The difference to unix_dgram_client is that this class cannot be connected to another socket.
     */
    class unix_dgram_server : public unix_dgram
    {
	private:
	    bool bound; ///< Shows if the socket is already bound (for setup routines)

	public:

	    unix_dgram_server(void);
	    unix_dgram_server(const char* bindpath, int socket_flags=0);
	    unix_dgram_server(const string& bindpath, int socket_flags=0);

	    void setup(const char* bindpath, int socket_flags=0);
	    void setup(const string& bindpath, int socket_flags=0);
    };
    /**
     * @}
     */
}

# endif
