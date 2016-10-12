#ifndef LIBSOCKET_SOCKET_H_CCC0882809A540AAAD842D9AE3DB47BB
#define LIBSOCKET_SOCKET_H_CCC0882809A540AAAD842D9AE3DB47BB

# include <sys/socket.h>
# include <sys/types.h>

# include <errno.h>

# include "exception.hpp"

/**
 * @file socket.hpp
 *
 * Contains the very base class of libsocket++
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

enum {
    LIBSOCKET_TCP = 1,
    LIBSOCKET_UDP,    

    LIBSOCKET_IPv4,
    LIBSOCKET_IPv6,

    LIBSOCKET_BOTH, // what fits best (TCP/UDP or IPv4/6)

    LIBSOCKET_READ = 1,
    LIBSOCKET_WRITE,

    LIBSOCKET_NUMERIC = 1
}

/**
 * @brief Contains libsocket elements
 *
 * All libsocket classes etc are defined within the namespace "libsocket".
 */
namespace libsocket
{
    /**
     * @addtogroup libsocketplusplus
     * @{
     */
    /**
     * @brief socket is the base class of every other libsocket++ object.
     *
     * It provides the most important field: The sfd field holds the file descriptor for every socket.
     */
    class socket
    {

	protected:
	    /// sfd is the sockets API file descriptor
	    int sfd;
	    bool is_nonblocking;
            /// Default is true; if set to false, the file descriptor is not closed when the destructor is called.
	    bool close_on_destructor;

	public:

	    socket(void);
	    socket(const socket&) = delete;
            socket(socket&&);

	    virtual ~socket();

	    virtual int destroy(void);

	    int getfd(void) const;

            int set_sock_opt(int level, int optname, const char* optval, socklen_t optlen) const;
    };
    /**
     * @}
     */
}
#endif
