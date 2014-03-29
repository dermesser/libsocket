#ifndef LIBSOCKET_INETSERVERDGRAM_H_369D8FFD83DD4A78A5A2E3F12EFD0A60
#define LIBSOCKET_INETSERVERDGRAM_H_369D8FFD83DD4A78A5A2E3F12EFD0A60

# include "inetbase.hpp"
# include "inetdgram.hpp"

/**
 * @file inetserverdgram.hpp
 *
 * Contains the class for UDP/IP server.
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
    /** @addtogroup libsocketplusplus
     * @{
     */
    /**
     * @brief Plain UDP sockets
     *
     * Although called `..._server`, this class may also be used as client (UDP is symmetric). It has some
     * special features, e.g. it's not possible to connect it.
     */
    class inet_dgram_server : public inet_dgram
    {
	public:

	    inet_dgram_server(const char* host, const char* port, int proto_osi3, int flags=0);
	    inet_dgram_server(const string& host, const string& port, int proto_osi3, int flags=0);

	private:
	    void setup(const char* host, const char* port, int proto_osi3, int flags=0);
	    void setup(const string& host, const string& port, int proto_osi3, int flags=0);
    };
    /**
     * @}
     */
}

# endif
