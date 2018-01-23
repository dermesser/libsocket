#ifndef LIBSOCKET_STREAMCLIENT_H_4EF38CC5CAD740E6B7A55BCF4C48CCFA
#define LIBSOCKET_STREAMCLIENT_H_4EF38CC5CAD740E6B7A55BCF4C48CCFA

# include <string>
# include "socket.hpp"

/**
 * @file streamclient.hpp
 *
 * Contains all functions and data members needed by stream socket classes, both UNIX and INET.
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
    class dgram_over_stream;

    /** @addtogroup libsocketplusplus
     * @{
     */
    /**
     * @brief Generic socket for active stream sockets (TCP client sockets, UNIX Stream clients)
     *
     * This class defines the basic i/o operations for all stream-based sockets.
     */
    class stream_client_socket : public virtual socket
    {
	protected:

	    bool shut_rd; ///< If the socket was shut down for reading (-> no reads anymore)
	    bool shut_wr; ///< If the socket was shut down for writing (-> no writes anymore)

	public:
            stream_client_socket();
            stream_client_socket(const stream_client_socket&) = delete;
            stream_client_socket(stream_client_socket&& other) : socket(std::move(other)), shut_rd(false), shut_wr(false) { }

	    ssize_t snd(const void* buf, size_t len, int flags=0); // flags: send()
	    ssize_t rcv(void* buf, size_t len, int flags=0); // flags: recv()

	    friend stream_client_socket& operator<<(stream_client_socket& sock, const char* str);
	    friend stream_client_socket& operator<<(stream_client_socket& sock, const string& str);
	    friend stream_client_socket& operator>>(stream_client_socket& sock, string& dest);
            friend class dgram_over_stream;

	    void shutdown(int method=LIBSOCKET_WRITE);
    };
    /**
     * @}
     */
}
# endif
