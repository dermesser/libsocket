#ifndef LIBSOCKET_UNIXBASE_H_0B648A3E27324425A6B7B9F7B262E7D1
#define LIBSOCKET_UNIXBASE_H_0B648A3E27324425A6B7B9F7B262E7D1
# include <string>

# include "socket.hpp"

# include "libunixsocket.h"

/**
 * @file unixbase.hpp
 *
 * Contains the base class for all UNIX-related socket classes.
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
     * @brief Base class for all UNIX socket classes
     *
     * This class provides only the data member _path and is used as base class for every socket class
     * working with UNIX sockets.
     */
    class unix_socket : public virtual socket
    {
	protected:
	    string _path; //!< The path we're connected (client stream) or bound (server stream, server datagram)

	public:
	    unix_socket();

	    string get_path(void);
    };
    /**
     * @}
     */
}
# endif
