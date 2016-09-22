#ifndef LIBSOCKET_INETBASE_H_6EDE111E3CDD4B07A94ECF4BD4E353C1
#define LIBSOCKET_INETBASE_H_6EDE111E3CDD4B07A94ECF4BD4E353C1

# include <string>
# include "socket.hpp"
# include "libinetsocket.h"
/**
 * @file inetbase.hpp
 *
 * Contains the IP socket base class.
 *
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
     * @brief	Internet-socket base class
     * This class provides some fields for all internet sockets, like remote host and port and the used protocol version.
     */

    class inet_socket : public virtual socket
    {
	protected:
	    /// The address we're bound or connected to
	    string host;
	    /// The port we're bound or connected to
	    string port;
	    /// Which internet protocol version we're using
	    int proto;

	public:

	    inet_socket();

	    const string& gethost(void) const;
	    const string& getport(void) const;
    };
    /**
     * @}
     */
}

# endif
