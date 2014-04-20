# include <string>
# include <stdio.h>
# include <errno.h>
# include <sstream>
# include <cstring>

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
 * @file exception.cpp
 * @brief Exception class in libsocket **CLASS FOR INTERNAL USE ONLY**
 *
 * The class defined here, socket_exception, is a
 * class for exception objects. Objects instantiated
 * from this class are thrown if something goes wrong.
 *
 * The class contains only the data member mesg and the
 * constructor which constructs the error string in mesg.
 * Typically, you'd create an object of this class with the
 * following call: `socket_exception(__FILE__,__LINE__,"Error Message");`
 *
 * The tokens are substituded by the preprocessor and show where
 * the error occurred.
 *
 * @addtogroup libsocketplusplus
 * @{
 */

# include <exception.hpp>

namespace libsocket
{
    using std::string;

    /**
     * @brief	Constructor of a socket_exception object
     *
     * This constructor creates a new socket_exception object.
     *
     * @param	f   File in which the error comes (__FILE__)
     * @param	l   Line (__LINE__)
     * @param	m   Description of the error.
     */
    socket_exception::socket_exception(const string& file, int line, const string& message, bool show_errno)
    {
	std::ostringstream message_stream;

	// Saving errno here should be safe
	err = errno;

	message_stream << file << ":" << line << ": " << message;

	if ( show_errno )
	    message_stream << " (" << std::strerror(errno) << ")";

	message_stream << "\n";

	mesg = message_stream.str();
    }
}

/**
 * @}
 */
