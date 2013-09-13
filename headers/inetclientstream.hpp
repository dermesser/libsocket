#ifndef LIBSOCKET_INETCLIENTSTREAM_H_64300E32D2CB4AD99D557F79F8D340CA
#define LIBSOCKET_INETCLIENTSTREAM_H_64300E32D2CB4AD99D557F79F8D340CA

# include <string>
# include <sys/types.h>
# include <sys/socket.h>

# include "inetbase.hpp"
# include "streamclient.hpp"

/**
 * @file inetclientstream.hpp
 *
 * inet_stream is the TCP/IP socket class
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

    /**
     * @addtogroup libsocketplusplus
     * @{
     */
    /**
     * @brief Provides TCP/IP client sockets.
     * This class is the most used socket class in libsocket++. It provides plain TCP client sockets which
     * can be used for almost everything.
     */
    class inet_stream : public inet_socket, public stream_client_socket
    {
	public:
	    inet_stream(void);
	    inet_stream(const char* dsthost, const char* dstport, int proto_osi3, int flags=0); // flags: socket()
	    inet_stream(const string& dsthost, const string& dstport, int proto_osi3, int flags=0);

	    void connect(const char* dsthost, const char* dstport, int proto_osi3, int flags=0); // flags: socket()
	    void connect(const string& dsthost, const string& dstport, int proto_osi3, int flags=0);

	    friend class inet_stream_server; ///< `inet_stream_server` is our friend so he may manipulate private members
					     ///< as `sfd` when returning an instance (e.g. at `accept()`)
    };
    /**
     * @}
     */
}
# endif
