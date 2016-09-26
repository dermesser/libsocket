# include <unistd.h>
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

/**
 * @file socket.cpp
 * @brief The very base class of libsocket.
 *
 * 	Provides the basic socket functionality, like a field
 * 	for the file descriptor and the destructor respectively
 * 	a close()-wrapper destroy(). This is the root of the
 * 	class graph.
 */

# include <socket.hpp>

namespace libsocket
{
    /**
     * @brief Constructor. Sets `sfd` to -1.
     *
     */
    socket::socket(void) : sfd(-1), is_nonblocking(false), close_on_destructor(true) { }

    /**
     * @brief Move constructor.
     */
    socket::socket(socket&& other) : sfd(other.sfd), is_nonblocking(false), close_on_destructor(true) {
            other.sfd = -1;
    }

    /**
     * @brief Destructor: closes socket.
     */
    socket::~socket(void)
    {
        /*
         * This is possible because socket::~socket(const socket&) is deleted.
         */
	if ( close_on_destructor )
                destroy();
    }

    /**
     * @brief	Destroys a socket.
     *
     * @retval 0    Fine!
     * @retval <0   Most likely the socket was already closed before.
     */
    int socket::destroy(void)
    {
        if ( 0 > sfd )
            return 0;

	if ( 0 > close(sfd))
	    return -1;

	sfd = -1;

	return 0;
    }

    /**
     * @brief Returns the socket file descriptor.
     *
     * @return The socket file descriptor of the class.
     *
     * getfd() is a getter you may use to obtain the file descriptor
     * for raw operations on it. It's relatively uncritical as libsocket
     * just wraps other syscalls and doesn't manipulate it using unusual ways.
     */
    int socket::getfd(void) const
    {
	return sfd;
    }

    /**
     * @brief Set socket options on the underlying socket.
     * 
     * @return The return value of setsockopt(2).
     *
     * Sets socket options using setsockopt(2). See setsockopt(2), tcp(7), udp(7), unix(7) for documentation
     * on how to use this function.
     */
    int socket::set_sock_opt(int level, int optname, const char* optval, socklen_t optlen) const {
        return setsockopt(sfd, level, optname, optval, optlen);
    }
}
