#ifndef LIBSOCKET_INETSERVERSTREAM_H_D84111E97D5548209F3816859EF080FE
#define LIBSOCKET_INETSERVERSTREAM_H_D84111E97D5548209F3816859EF080FE

# include "inetbase.hpp"
# include "inetclientstream.hpp"

# include <memory>

/**
 * @file inetserverstream.hpp
 *
 * Contains the class for TCP/IP servers.
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
    using std::unique_ptr;

    /**
     * @addtogroup libsocketplusplus
     * @{
     */
    /**
     * @brief TCP server socket (passive TCP socket)
     *
     * This class acts as TCP server. Once set up you may call `accept()` on it. When a client wants
     * to connect, a pointer to an `inet_stream` class is returned. It may be used to communicate with
     * the client.
     *
     * @see inet_stream
     */
    class inet_stream_server : public inet_socket
    {
	public:

	    inet_stream_server(void);
	    inet_stream_server(const char* bindhost, const char* bindport, int proto_osi3, int flags=0);
	    inet_stream_server(const string& bindhost, const string& bindport, int proto_osi3, int flags=0);

	    void setup(const char* bindhost, const char* bindport, int proto_osi3, int flags=0);
	    void setup(const string& bindhost, const string& bindport, int proto_osi3, int flags=0);

	    inet_stream* accept(int numeric=0,int accept_flags=0);
            unique_ptr<inet_stream> accept2(int numeric=0, int accept_flags=0);

	    const string& getbindhost(void);
	    const string& getbindport(void);
    };
    /**
     * @}
     */
}

# endif
