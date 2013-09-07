# ifndef _SELECT_H
# define _SELECT_H

# include <vector>
# include <map>
# include <utility>
# include <list>
# include <cstring>

# include <sys/select.h>

/**
 * @file select.hpp
 *
 * The class selectset provides a possibility to wait for data on multiple sockets.
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

# define LIBSOCKET_READ 1
# define LIBSOCKET_WRITE 2

namespace libsocket
{
    /** @addtogroup libsocketplusplus
     * @{
     */
    /**
     * @brief selectset provides a simple interface to the system call select(3).
     *
     * To watch different sockets for new data to read or a possibility to write without using threads, use
     * this class. It is rather simple to use; add file descriptors (socket ids) using `add_fd()` specifying
     * whether to watch them for reading or writing and then call `wait()`; once there's data to be read or written
     * it returns a std::pair with vectors of `libsocket::socket*`, the first vector contains sockets ready for reading,
     * the second one contains those sockets ready for writing. Usually it's necessary to cast them to the actual socket type
     * using `dynamic_cast<>()`.
     */
    class selectset
    {
	private:
	    std::vector<int> filedescriptors; ///< All file descriptors from the socket objects
	    std::map<int,socket*> fdsockmap;  ///< A map containing the relations between the filedescriptors and the socket objects

	    bool set_up; ///< Stores if the class has been initiated

	    fd_set readset; ///< The fd_set for select with the descriptors waiting for read
	    fd_set writeset; ///< and the descriptors waiting for write

	public:

	    selectset();

	    void add_fd(socket& sock, int method);

	    std::pair<std::vector<socket*>, std::vector<socket*> > wait(long long microsecs=0);
    };

    typedef std::pair<std::vector<libsocket::socket*>, std::vector<libsocket::socket*> > ready_socks;

    /**
     * @}
     */
}

#endif
