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

# include "exception.hpp"
# include <sys/select.h>

/**
 * @file select.hpp
 *
 * @brief Contains the class selectset which provides a neat interface for watching several sockets
 *
 * The class selectset implements a wrapper for the syscall select() which allows
 * to accept connections on more than one socket or communicate with multiple clients
 * without multithreading.
 *
 * New sockets may be added with add_fd(), accepting a child class of libsocket::socket.
 *
 * When all sockets are added, the select() call can be triggered by calling wait(). This
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
     * @brief selectset provides a simple interface to the system call select(3).
     *
     * To watch different sockets for new data to read or a possibility to write without using threads, use
     * this class. It is rather simple to use; add file descriptors (socket ids) using `add_fd()` specifying
     * whether to watch them for reading or writing and then call `wait()`; once there's data to be read or written
     * it returns a std::pair with vectors of `SocketT*`; the first vector contains sockets ready for reading,
     * the second one contains those sockets ready for writing.
     *
     * If you select on multiple sockets, you will need to use a superclass as template argument (e.g. `socket` or `inet_socket`)
     * and `dynamic_cast`.
     */
    template<typename SocketT>
    class selectset
    {
	private:
	    std::vector<int> filedescriptors; ///< All file descriptors from the socket objects
	    std::map<int,SocketT*> fdsockmap;  ///< A map containing the relations between the filedescriptors and the socket objects

	    bool set_up; ///< Stores if the class has been initiated

	    fd_set readset; ///< The fd_set for select with the descriptors waiting for read
	    fd_set writeset; ///< and the descriptors waiting for write

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
	: filedescriptors(0), set_up(false)
    {
	FD_ZERO(&readset);
	FD_ZERO(&writeset);
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
	    FD_SET(fd,&readset);
	    filedescriptors.push_back(fd);
	    fdsockmap[fd] = const_cast<SocketT*>(&sock);
	    set_up = true;

	} else if ( method == LIBSOCKET_WRITE )
	{
	    FD_SET(fd,&writeset);
	    filedescriptors.push_back(fd);
	    fdsockmap[fd] = const_cast<SocketT*>(&sock);
	    set_up = true;
	} else if ( method == (LIBSOCKET_READ|LIBSOCKET_WRITE) )
	{ // don't put the fd in our data structures twice.
	    FD_SET(fd,&readset);
	    FD_SET(fd,&writeset);
	    filedescriptors.push_back(fd);
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

	struct timeval *timeout = NULL;

	if ( microsecs != 0 )
	{
	    struct timeval _timeout;

	    timeout = &_timeout;

	    long long micropart = microsecs % 1000000;
	    long long secpart   = microsecs / 1000000;

	    _timeout.tv_sec  = secpart;
	    _timeout.tv_usec = micropart;
	}

	n = select(highestfd(filedescriptors)+1,&readset,&writeset,NULL,timeout);

	ready_socks rwfds;

	if ( n < 0 )
	{
	    std::string err(strerror(errno));

	    throw socket_exception(__FILE__,__LINE__,"selectset::wait(): Error at select(): " + err);

	} else if ( n == 0 ) // time is over, no filedescriptor is ready
	{
	    rwfds.first.resize(0);
	    rwfds.second.resize(0);

	    return rwfds;
	}

	std::vector<int>::iterator end = filedescriptors.end();

	for ( std::vector<int>::iterator cur = filedescriptors.begin(); cur != end; cur++ )
	{
	    if ( FD_ISSET(*cur,&readset) )
		rwfds.first.push_back(fdsockmap[*cur]);

	    if ( FD_ISSET(*cur,&writeset) )
		rwfds.second.push_back(fdsockmap[*cur]);
	}

	return rwfds;
    }

}

#endif
