# include <iostream>
# include <string>
# include <string.h>
# include <memory>

# include <unistd.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <stdio.h>

// oo wrapper around libinetsocket
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
 * @file inetdgram.cpp
 * @brief The base class for all internet-datagram sockets (UDP/IP)
 *
 * 	The class inet_dgram provides that functions which
 * 	are used for all UDP sockets: rcvfrom and sndto. These
 * 	may be used for both client and server UDP sockets.
 */

# include <libinetsocket.h>
# include <exception.hpp>
# include <inetdgram.hpp>

namespace libsocket
{
    using std::string;

    // I/O

    // I

    /**
     * @brief Receives data from peer
     *
     * rcvfrom is the equivalent to `recvfrom(2)`.
     *
     * @param buf Target memory
     * @param len The size of the target memory
     * @param hostbuf Buffer to write the peer's hostname to
     * @param hostbuflen Its length
     * @param portbuf Like `hostbuf`, but for the remote port
     * @param portbuflen `portbuf`'s length
     * @param rcvfrom_flags Flags to be passed to `recvfrom(2)`
     * @param numeric If this is `true`, host and port are saved numerically (25 instead of "smtp")
     *
     * @retval >0 n bytes of data were read into `buf`.
     * @retval 0 Peer sent EOF
     * @retval -1 Socket is non-blocking and returned without any data.
     *
     * Every error makes the function throw an exception.
     */
    ssize_t inet_dgram::rcvfrom(void* buf, size_t len, char* hostbuf, size_t hostbuflen, char* portbuf, size_t portbuflen, int rcvfrom_flags, bool numeric)
    {
	ssize_t bytes;
	int num = ((numeric == true) ? LIBSOCKET_NUMERIC : 0);

	if ( -1 == sfd )
	    throw socket_exception(__FILE__,__LINE__,"inet_dgram::rcvfrom() - Socket is closed!",false);

	if ( -1 == (bytes = recvfrom_inet_dgram_socket(sfd,buf,len,hostbuf,hostbuflen,portbuf,portbuflen,rcvfrom_flags,num)) )
	{
	    if ( is_nonblocking && errno == EWOULDBLOCK )
		return -1;
	    else
		throw socket_exception(__FILE__,__LINE__,"inet_dgram::rcvfrom() - recvfrom() failed -- could not receive data from peer!");
	}

	return bytes;
    }


    /**
     * @brief rcvfrom for C++ strings
     *
     * Works like `rcvfrom()`, but takes C++ strings instead of pointers.
     *
     * @param buf Buffer to copy the received data to
     * @param len The buffer's length
     * @param srchost String to place the remote host's name to
     * @param srcport Like `srchost` but for the remote port
     * @param rcvfrom_flags Flags to be passed to `recvfrom(2)`
     * @param numeric If remote host and port should be saved numerically
     *
     * @retval >0 n bytes of data were read into `buf`.
     * @retval 0 Peer sent EOF
     * @retval -1 Socket is non-blocking and returned without any data.
     *
     * Every error makes the function throw an exception.
     */
    ssize_t inet_dgram::rcvfrom(void* buf, size_t len, string& srchost, string& srcport, int rcvfrom_flags, bool numeric)
    {
	ssize_t bytes;

	using std::unique_ptr;
	unique_ptr<char[]> from_host(new char[1024]);
	unique_ptr<char[]> from_port(new char[32]);

	memset(from_host.get(),0,1024);
	memset(from_port.get(),0,32);
	memset(buf,0,len);

	// Error checking already done in rcvfrom() method.
	bytes = rcvfrom(buf,len,from_host.get(),1023,from_port.get(),31,rcvfrom_flags,numeric);

	srchost.resize(strlen(from_host.get()));
	srcport.resize(strlen(from_port.get()));

	srchost.assign(from_host.get());
	srcport.assign(from_port.get());

	return bytes;
    }

    /**
     * @brief rcvfrom for C++ strings, implemented consistently
     *
     * Works like every other `rcvfrom()` library call, but places the received memory to the C++ string `buf`.
     *
     * @param buf The string where the received data should be stored at. Its length determines how much data will be stored; the library
     * will not resize `buf`.
     * @param srchost String to place the remote host's name to
     * @param srcport Like `srchost` but for the remote port
     * @param rcvfrom_flags Flags to be passed to `recvfrom(2)`
     * @param numeric If remote host and port should be saved numerically

     * @retval >0 n bytes of data were read into `buf`.
     * @retval 0 Peer sent EOF
     * @retval -1 Socket is non-blocking and returned without any data.
     *
     * Every error makes the function throw an exception.
     */
    ssize_t inet_dgram::rcvfrom(string& buf, string& srchost, string& srcport, int rcvfrom_flags, bool numeric)
    {
	ssize_t bytes;

	using std::unique_ptr;
	unique_ptr<char[]> cbuf(new char[buf.size()]);

        memset(cbuf.get(),0,buf.size());

	bytes = rcvfrom(cbuf.get(),static_cast<size_t>(buf.size()),srchost,srcport,rcvfrom_flags,numeric); // calling inet_dgram::rcvfrom(void*, size_t, string&, string&, int, bool)

	buf.resize(bytes);

	buf.assign(cbuf.get(),bytes);

	return bytes;
    }

    // O

    /**
     * @brief Send data to UDP peer
     *
     * This is the counterpart to system's `sendto(2)`. It sends data to a UDP peer.
     *
     * @param buf The data to be sent
     * @param len Length of transmission
     * @param dsthost Target host
     * @param dstport Target port
     * @param sndto_flags Flags for `sendto(2)`
     *
     * @retval >0 n bytes of data were sent.
     * @retval 0 Nothing was sent
     * @retval -1 Socket is non-blocking and didn't send any data.
     *
     * Every error makes the function throw an exception.
     */
    ssize_t inet_dgram::sndto(const void* buf, size_t len, const char* dsthost, const char* dstport, int sndto_flags)
    {
	ssize_t bytes;

	if ( -1 == sfd )
	    throw socket_exception(__FILE__,__LINE__,"inet_dgram::sendto() - Socket already closed!",false);

	if ( -1 == (bytes = sendto_inet_dgram_socket(sfd,buf,len,dsthost,dstport,sndto_flags)) )
	{
	    if ( is_nonblocking && errno == EWOULDBLOCK )
		return -1;
	    else
		throw socket_exception(__FILE__,__LINE__,"inet_dgram::sndto() - Error at sendto");
	}

	return bytes;
    }

    /**
     * @brief Send data to UDP peer; C++ string host and port
     *
     * This is the counterpart to system's `sendto(2)`. It sends data to a UDP peer.
     *
     * @param buf The data to be sent
     * @param len Length of transmission
     * @param dsthost Target host
     * @param dstport Target port
     * @param sndto_flags Flags for `sendto(2)`
     *
     * Every error makes the function throw an exception, except for EWOULDBLOCK.
     *
     * @retval -1 Socket is non-blocking and didn't send any data.
     */
    ssize_t inet_dgram::sndto(const void* buf, size_t len, const string& dsthost, const string& dstport, int sndto_flags)
    {
	ssize_t bytes;

	bytes = sndto(buf,len,dsthost.c_str(),dstport.c_str(),sndto_flags);

	return bytes;
    }

    /**
     * @brief Send data to UDP peer; using C++ strings only.
     *
     * This is the counterpart to system's `sendto(2)`. It sends data to a UDP peer.
     *
     * @param buf The data to be sent
     * @param dsthost Target host
     * @param dstport Target port
     * @param sndto_flags Flags for `sendto(2)`
     *
     * Every error makes the function throw an exception.
     *
     * @retval -1 Socket is non-blocking and didn't send any data.
     */
    ssize_t inet_dgram::sndto(const string& buf, const string& dsthost, const string& dstport, int sndto_flags)
    {
	ssize_t bytes;

	bytes = sndto(buf.c_str(),buf.size(),dsthost.c_str(),dstport.c_str(),sndto_flags);

	return bytes;
    }
}
