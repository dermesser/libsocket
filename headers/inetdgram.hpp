#ifndef LIBSOCKET_INETDGRAM_H_DD09F601418D47C8893DB605109E3E03
#define LIBSOCKET_INETDGRAM_H_DD09F601418D47C8893DB605109E3E03

# include <iostream>
# include <string>
# include <string.h>

# include "inetbase.hpp"

# include <unistd.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <stdio.h>

/**
 * @file inetdgram.hpp
 *
 * Contains the inet_dgram class
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
     *
     * @brief Base class for UDP/IP sockets
     *
     * This classes provides the Send/Receive functions shared by all classes using Internet Datagram sockets.
     */
    class inet_dgram : public inet_socket
    {
	public:

	    // I/O
	    // O
	    ssize_t sndto(const void* buf, size_t len, const char* dsthost, const char* dstport, int sndto_flags=0); // flags: sendto()
	    ssize_t sndto(const void* buf, size_t len, const string& dsthost, const string& dstport, int sndto_flags=0);

	    ssize_t sndto(const string& buf, const string& dsthost, const string& dstport, int sndto_flags=0);

	    // I
	    ssize_t rcvfrom(void* buf, size_t len, char* srchost, size_t hostlen, char* srcport, size_t portlen, int rcvfrom_flags=0, bool numeric=false);
	    ssize_t rcvfrom(void* buf, size_t len, string& srchost, string& srcport, int rcvfrom_flags=0, bool numeric=false);

	    ssize_t rcvfrom(string& buf, string& srchost, string& srcport, int rcvfrom_flags=0, bool numeric=false);
    };
    /**
     * @}
     */
}

# endif
