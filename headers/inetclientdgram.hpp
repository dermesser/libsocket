#ifndef LIBSOCKET_INETDGRAMCLIENT_H_1E7B8B3247594112BB2CEB2895E916E4
#define LIBSOCKET_INETDGRAMCLIENT_H_1E7B8B3247594112BB2CEB2895E916E4

# include <string>
# include <sys/types.h>
# include <sys/socket.h>

# include "inetdgram.hpp"
# include "dgramclient.hpp"

/**
 * @file inetclientdgram.hpp
 *
 * Contains the inet_dgram_client class.
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
     * @brief Using UDP/IP easy as never before.
     * Class for connectable datagram sockets in the INET domain. You may connect this sockets
     * to permanently associate a peer to which all data is sent and from which all data is received
     * when using the functions defined in class `dgram_client_socket`
     */
    class inet_dgram_client : public inet_dgram, public dgram_client_socket
    {
	public:

	    // Only create socket
	    inet_dgram_client(int proto_osi3,int flags=0); // Flags: socket()
	    // Create socket and connect it
	    inet_dgram_client(const char* dsthost, const char* dstport, int proto_osi3, int flags=0); // Flags: socket()
	    inet_dgram_client(const string& dsthost, const string& dstport, int proto_osi3, int flags=0);

	    // actions
	    // connect/reconnect
	    void connect(const char* dsthost, const char* dstport);
	    void connect(const string& dsthost, const string& dstport);

	    void deconnect(void);

	private:
	    void setup(int proto_osi3, int flags=0);
	    void setup(const char* dsthost, const char* dstport, int proto_osi3, int flags=0);
	    void setup(const string& dsthost, const string& dstport, int proto_osi3, int flags=0);
    };
    /**
     * @}
     */
}
# endif
