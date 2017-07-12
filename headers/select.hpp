#ifndef LIBSOCKET_SELECT_H_7F761B91E8A84EB685E898542039D68F
#define LIBSOCKET_SELECT_H_7F761B91E8A84EB685E898542039D68F

# include <vector>
# include <map>
# include <utility>
# include <list>
# include <cstring>

# include <unistd.h>
# include <sys/select.h>
# include <errno.h>
# include <sys/time.h>
# include <sys/types.h>

namespace poll {
# include <poll.h>
}

# include "exception.hpp"


/**
 * @file select.hpp
 *
 * @brief Contains the class selectset which provides a neat interface for watching several sockets
 *
 * The class selectset implements a wrapper for the syscall poll() (formerly select()) which allows
 * to accept connections on more than one socket or communicate with multiple clients
 * without multithreading.
 *
 * New sockets may be added with add_fd(), accepting a child class of libsocket::socket.
 *
 * When all sockets are added, the poll() call can be triggered by calling wait(). This
 * function returns a pair of vector<int>s, the first with the sockets ready for reading and
 * the second containing the sockets ready for writing.
 *
 * This is a template class; use the appropriate socket class as template argument. If you want to
 * select on several different sockets (e.g. an INET server and an INET client), use some superclass
 * and `dynamic_cast` to cast the pointers returned by `wait()`.
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

# include "socket.hpp"

namespace libsocket
{
    /**
     * @addtogroup libsocketplusplus
     * @{
     */
    /**
     * @brief selectset provides a simple abstraction over -- contrary to its name -- poll(2).
     *
     * To watch different sockets for new data to read or a possibility to
     * write without using threads, use this class. It is rather simple to use;
     * add file descriptors (socket ids) using `add_fd()` specifying whether to
     * watch them for reading or writing and then call `wait()`; once there's
     * data to be read or written it returns a std::pair with vectors of
     * `SocketT*`; the first vector contains sockets ready for reading, the
     * second one contains those sockets ready for writing.
     *
     * If you poll sockets of different types, you need to use a superclass as
     * template argument to selectset (usually `socket` or `inet_socket`) and
     * then `dynamic_cast` the sockets returned in the ready_socks structure
     * (see `examples++/server.cpp` for an (artificial) example).
     *
     * In order to determine which socket in a ready_socks set belongs to which
     * connection/server/etc., you should either use the pointer values of
     * submitted sockets or the file descriptors managed by your sockets (via
     * `Socket::getfd()`).
     */
    template<typename SocketT>
    class selectset
    {
	private:
	    std::map<int,SocketT*> fdsockmap;  ///< A map containing the relations between the filedescriptors and the socket objects

	    bool set_up; ///< Stores if the class has been initiated
            
            std::vector<poll::pollfd> pollfd_set; // Set of pollfd structs to poll

	public:

	    selectset();

	    void add_fd(const SocketT& sock, int method);

	    std::pair<std::vector<SocketT*>, std::vector<SocketT*> > wait(long long microsecs=0);
	    typedef std::pair<std::vector<SocketT*>, std::vector<SocketT*> > ready_socks;
    };
    /**
     * @}
     */

    extern int highestfd(const std::vector<int>& v);

    /**
     * @brief Constructor.
     *
     * Initializes the sets.
     */
    template<typename SockT>
    selectset<SockT>::selectset(void)
        : set_up(false)
    {
    }

    /**
     * @brief Add a socket to the internal sets
     *
     * @param sock Some socket. May be server or client socket.
     * @param method `LIBSOCKET_READ`/`LIBSOCKET_WRITE` or an `OR`ed combination thereof. Determines if the socket is checked on the possibility to read or to write.
     *
     */
    template<typename SocketT>
    void selectset<SocketT>::add_fd(const SocketT& sock, int method )
    {
	int fd = sock.getfd();

	if ( method == LIBSOCKET_READ )
	{
            poll::pollfd fdinfo{fd, POLLIN, 0};
            pollfd_set.push_back(fdinfo);
	    fdsockmap[fd] = const_cast<SocketT*>(&sock);
	    set_up = true;

	} else if ( method == LIBSOCKET_WRITE )
	{
            poll::pollfd fdinfo{fd, POLLOUT, 0};
            pollfd_set.push_back(fdinfo);
	    fdsockmap[fd] = const_cast<SocketT*>(&sock);
	    set_up = true;
	} else if ( method == (LIBSOCKET_READ|LIBSOCKET_WRITE) )
	{ // don't put the fd in our data structures twice.
            poll::pollfd fdinfo{fd, (POLLIN|POLLOUT), 0};
            pollfd_set.push_back(fdinfo);
	    fdsockmap[fd] = const_cast<SocketT*>(&sock);
	    set_up = true;
	}
    }

    /**
     * @brief Waits for a possibility to read or write data to emerge.
     *
     * @param microsecs A timeout in microseconds (for 5 seconds simply write 5e6, for ten seconds 10e6 and so on). 0 for no timeout (possibly infinite waiting)
     *
     * @returns A pair of vectors of pointers to sockets. Information about the type of socket is lost;
     * use `dynamic_cast<>()` and check for `NULL` to re-convert it. The first vector contains sockets available for reading,
     * the second one those available for writing.
     * 
     * *Hint*: Save pointers to the added objects to be able to compare and distinguish them after `wait()`.
     */
    template<typename SockT>
    typename selectset<SockT>::ready_socks selectset<SockT>::wait(long long microsecs)
    {
	int n = 0;
        
        struct timespec *timeout = NULL;
        struct timespec _timeout;
        
        if ( microsecs != 0 )
	{
	    timeout = &_timeout;
        
            long long nanosecs = microsecs * 1000;
            long long nanopart = nanosecs % 1000000000;
            long long secpart  = (nanosecs - nanopart) / 1000000000;
            
            _timeout.tv_sec = secpart;
            _timeout.tv_nsec = nanopart;
        }

        n = ppoll((poll::pollfd *)pollfd_set.data(), pollfd_set.size(),
                  timeout, NULL);
                
	ready_socks rwfds;

	if ( n < 0 )
	{
	    std::string err(strerror(errno));

	    throw socket_exception(__FILE__,__LINE__,"selectset::wait(): Error at ppoll(): " + err);

	} else if ( n == 0 ) // time is over, no filedescriptor is ready
	{
	    rwfds.first.resize(0);
	    rwfds.second.resize(0);

	    return rwfds;
	}
        
        std::vector<poll::pollfd>::iterator end = pollfd_set.end();
        
        for (std::vector<poll::pollfd>::iterator iter = pollfd_set.begin(); iter != end; ++iter)
        {
            if (iter->revents & POLLIN)
                rwfds.first.push_back(fdsockmap[iter->fd]);
            
            if (iter->revents & POLLOUT)
                rwfds.second.push_back(fdsockmap[iter->fd]);
        }

	return rwfds;
    }

}

#endif
