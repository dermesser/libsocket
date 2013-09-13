#ifndef LIBSOCKET_UNIXDGRAM_H_B1DCD9EE9E7E4B379FD5FCA79EF4B63F
#define LIBSOCKET_UNIXDGRAM_H_B1DCD9EE9E7E4B379FD5FCA79EF4B63F

# include "unixbase.hpp"

/**
 * @file unixdgram.hpp
 *
 * All functions necessary for UNIX-domain datagram sockets.
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
     * @brief Generic class for all UNIX datagram (DGRAM) sockets, implementing i/o functions.
     */
    class unix_dgram : public unix_socket
    {
	public:

	    ssize_t sndto(const void* buf, size_t length, const char* path, int sendto_flags=0);
	    ssize_t sndto(const void* buf, size_t length, const string& path, int sendto_flags=0);

	    ssize_t sndto(const string& buf, const string& path, int sendto_flags=0);

	    ssize_t rcvfrom(void* buf, size_t length, char* source, size_t source_len, int recvfrom_flags=0);
	    ssize_t rcvfrom(void* buf, size_t length, string& source, int recvfrom_flags=0);

	    ssize_t rcvfrom(string& buf, string& source, int recvfrom_flags=0);

    };
    /**
     * @}
     */
}

# endif
