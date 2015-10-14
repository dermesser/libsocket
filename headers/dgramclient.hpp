#ifndef LIBSOCKET_DGRAMCLIENT_H_A6969EEDFC57408B89EA3E965C00E811
#define LIBSOCKET_DGRAMCLIENT_H_A6969EEDFC57408B89EA3E965C00E811

# include "socket.hpp"
# include <string>
# include <unistd.h>
# include <string.h>

using std::string;

/**
 * @file dgramclient.hpp
 *
 * Contains the dgram_client_socket base class
 *
 *
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
    /**
     * @addtogroup libsocketplusplus
     * @{
     */

    /**
     * @brief Represents a generic "client" datagram socket, i.e. a datagram socket which can be connected.
     */
    class dgram_client_socket : public virtual socket
    {
	protected:
	    bool connected; ///< Stores the connection state of the socket so other functions in this class can check if the socket is connected.
			    ///< The `connection()` method itself is implemented in derived classes, e.g. inet_dgram_client

	public:

	    dgram_client_socket(void);

	    friend dgram_client_socket& operator<<(dgram_client_socket& sock, const char* str);
	    friend dgram_client_socket& operator<<(dgram_client_socket& sock, const string& str);

	    ssize_t snd(const void* buf, size_t len, int flags=0); // flags: send()

	    // I
	    friend dgram_client_socket& operator>>(dgram_client_socket& sock, string& dest);

	    ssize_t rcv(void* buf, size_t len, int flags=0);

	    // Getters

	    bool getconn(void) const;
    };

    /**
     * @}
     */
}
# endif
