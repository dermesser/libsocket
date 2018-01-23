#ifndef LIBSOCKET_EXCEPTION_H_07F57E018FF44ADBB169FA2F685EA87E
#define LIBSOCKET_EXCEPTION_H_07F57E018FF44ADBB169FA2F685EA87E

# include <string>

/**
 * @file exception.hpp
 *
 * Contains the libsocket exception class, socket_exception
 *
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
     * @brief This class is instantiated and thrown when an error occurs.
     * If there's an error somewhere in libsocket++, the function in which the error occurs /always/
     * throws a `socket_exception` object showing why the error occurred.
     */
    struct socket_exception
    {
	int err; //!< This is the value of errno when the error occurred.
	string mesg; //!< This is the message, showing file, line and a description. If your program is verbose, simply print it to STDERR.
		     //!< It contains 1. why the error occurred 2. in which file it occurred 3. in which line it occurred. It's a bit like rsync.
		     //!< A typical message looks like the following one: "../C++/inetclientstream.cpp:167: <<(std::string) output: Socket not connected!"

	socket_exception(const string& file,int line,const string& message, bool show_errno = true);
    };
    /**
     * @}
     */
}
# endif
