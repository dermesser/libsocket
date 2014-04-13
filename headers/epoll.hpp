# ifndef LIBSOCKET_EPOLL_H_E58EF2DF7057FA0C7A95D6E753414229
# define LIBSOCKET_EPOLL_H_E58EF2DF7057FA0C7A95D6E753414229

/*
   The committers of the libsocket project, all rights reserved
   (c) 2014, dermesser <lbo@spheniscida.de>

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
 * @file epoll.hpp
 * @brief [LINUX-only] epoll API abstraction layer.
 *
 * This template file contains the epollset class which may be used to access the modern epoll API of Linux
 * kernels newer than 2.6.
 */

# include <vector>
# include <memory>
# include <cstring>

# include <sys/epoll.h>

# include "exception.hpp"
# include "socket.hpp"

using std::vector;

namespace libsocket
{
    /**
     * @addtogroup libsocketplusplus
     * @{
     */

    /**
     * @brief Class abstracting calls to the `epoll` API of Linux.
     *
     * Being a template, this class needs to be initialized with a class. The choice is easy if you only have one
     * type of socket to run epoll on; however, if you have (for example) an `inet_stream` and an `inet_stream_server` socket,
     * you will need to use either `socket` or `inet_socket` as type parameter so the automatic upcasting works as intended.
     *
     * Upon return of `wait()`, you'll have to identify the sockets (using the address -- this class is practically zero-copy)
     * and cast them back with the help of `dynamic_cast`.
     */
    template<typename SocketT>
    class epollset
    {
    public:
	typedef std::pair<std::vector<SocketT*>, std::vector<SocketT*> > ready_socks;

	epollset(unsigned int maxevents = 128);
	epollset(const epollset&) = delete;
	epollset(epollset&&);
	~epollset(void);

	void add_fd( SocketT& sock, int method );
	void del_fd(const SocketT& sock);
	ready_socks wait(int timeout = -1);

    private:

	/// maxevents is passed to `epoll_wait`.
	unsigned int maxevents;
	/// The file descriptor used by the epoll API
	int epollfd;
	/// Array of structures, filled on the return of `epoll_wait`.
	struct epoll_event* events;
    };

    /**
     * @}
     */

    /**
     * @brief Construct a new epollset
     *
     * @param maxevs Maximum event number returned by `epoll_wait`. Default is 128.
     */
    template<typename SocketT>
    epollset<SocketT>::epollset (unsigned int maxevs)
	: maxevents(maxevs),
	events(new struct epoll_event[maxevs])
    {
	epollfd = epoll_create1(0);

	if ( epollfd < 0 )
	    throw socket_exception(__FILE__,__LINE__,string("epoll_create1 failed: ") + strerror(errno));
    }

    /**
     * @brief Move constructor for epollsets.
     */
    template<typename SocketT>
    epollset<SocketT>::epollset (epollset&& new_epollset)
    {
	maxevents = new_epollset.maxevents;
	epollfd = new_epollset.epollfd;
	events = new_epollset.events;

	new_epollset.epollfd = -1;
	new_epollset.events = nullptr;
    }

    template<typename SocketT>
    epollset<SocketT>::~epollset ( void )
    {
	delete[] events;
    }

    /**
     * @brief Add a socket to an `epollset`.
     *
     * @param sock The socket to be added.
     * @param method Any combination of `LIBSOCKET_READ` and `LIBSOCKET_WRITE`.
     */
    template<typename SocketT>
    void epollset<SocketT>::add_fd(SocketT& sock, int method)
    {
	struct epoll_event new_event;

	new_event.data.ptr = 0; // ptr is the largest field (8 bytes on 64bit)
	new_event.events = 0;

	if ( method & LIBSOCKET_READ )
	    new_event.events |= EPOLLIN;
	if ( method & LIBSOCKET_WRITE )
	    new_event.events |= EPOLLOUT;

	new_event.data.ptr = &sock;

	if ( 0 > epoll_ctl(epollfd,EPOLL_CTL_ADD,sock.getfd(),&new_event) )
	    throw socket_exception(__FILE__,__LINE__,string("epoll_ctl failed: ") + strerror(errno));
    }

    /**
     * @brief Remove a file descriptor from an epoll set.
     *
     * @param sock The socket to remove.
     */
    template<typename SocketT>
    void epollset<SocketT>::del_fd(const SocketT& sock)
    {
	if ( 0 > epoll_ctl(epollfd,EPOLL_CTL_DEL,sock.getfd(),nullptr) )
	    throw socket_exception(__FILE__,__LINE__,string("epoll_ctl failed: ") + strerror(errno));
    }

    /**
     * @brief Wait for an event on any file descriptor.
     *
     * @param timeout (default: -1) Timeout, in milliseconds, after which to return if no event has occurred yet. -1 for
     * indefinite waiting, 0 for non-blocking access.
     *
     * @return A pair of vectors containing pointers to SocketTs: (ready_for_reading[],ready_for_writing[]). With `r` being
     * the returned pair, access the sockets using statements like `r.first.size() > 0 ? r.first[0] : nullptr` or the like.
     *
     * *Hint*: If you need to know what socket has been returned, maintain a mapping from socket pointers to some identification
     * code. Using that mapping, you will be able to identify the sockets.
     *
     * *Hint 2*: It never does any harm to check the length of the returned `vector`s; with the included example
     * `http_epoll.cpp`, spurious empty-returning epoll calls could be observed. However, it is not clear if this is
     * a bug in the libsocket implementation or usual behavior of epoll.
     */
    template<typename SocketT>
    typename epollset<SocketT>::ready_socks epollset<SocketT>::wait (int timeout)
    {
	int nfds;
	ready_socks ready;

	if ( 0 > (nfds = epoll_wait(epollfd, events, maxevents, timeout)) )
	    throw socket_exception(__FILE__,__LINE__,string("epoll_wait failed: ") + strerror(errno));

	for ( int i = 0; i < nfds; i++ )
	{
	    if ( events[i].events == EPOLLIN )
		ready.first.push_back(static_cast<SocketT*>(events[i].data.ptr));
	    if ( events[i].events == EPOLLOUT )
		ready.second.push_back(static_cast<SocketT*>(events[i].data.ptr));
	}

	return ready;
    }

}
# endif
