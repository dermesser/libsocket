# include <string>
# include <unistd.h>
# include <string.h>

# include <memory>

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
 * @file unixdgram.cpp
 * @brief I/O for UNIX sockets
 *
 * Provides the basic I/O functions for every UNIX domain
 * datagram socket. Every unix datagram socket class is
 * derived from this class because sndto and rcvfrom may be
 * called on every datagram socket.
 */

# include <libunixsocket.h>
# include <exception.hpp>
# include <unixdgram.hpp>

namespace libsocket
{
    /**
     * @brief Send data to datagram socket
     *
     * @param buf Pointer to data.
     * @param length Length of `buf`
     * @param path Path of destination
     * @param sendto_flags Flags for `sendto(2)`
     *
     * @returns How many bytes were sent. Returns -1 if the socket was created with SOCK_NONBLOCK and errno is EWOULDBLOCK.
     */
    ssize_t unix_dgram::sndto(const void* buf, size_t length, const char* path, int sendto_flags)
    {
	if ( buf == NULL )
	    throw socket_exception(__FILE__,__LINE__,"unix_dgram::sndto: Buffer is NULL!",false);

	ssize_t bytes;

	if ( 0 > (bytes = sendto_unix_dgram_socket(sfd,buf,length,path,sendto_flags)) )
	{
	    if ( is_nonblocking && errno == EWOULDBLOCK )
		return -1;
	    else
		throw socket_exception(__FILE__,__LINE__,"unix_dgram::sndto: Could not send data to peer!");
	}

	return bytes;
    }

    /**
     * @brief Send data to datagram socket
     *
     * @param buf Pointer to data.
     * @param length Length of `buf`
     * @param path Path of destination
     * @param sendto_flags Flags for `sendto(2)`
     *
     * @returns How many bytes were sent. Returns -1 if the socket was created with SOCK_NONBLOCK and errno is EWOULDBLOCK.
     */
    ssize_t unix_dgram::sndto(const void* buf, size_t length, const string& path, int sendto_flags)
    {
	return sndto(buf,length,path.c_str(),sendto_flags);
    }

    /**
     * @brief Send data to datagram socket
     *
     * @param buf Pointer to data.
     * @param path Path of destination
     * @param sendto_flags Flags for `sendto(2)`
     *
     * @returns How many bytes were sent. Returns -1 if the socket was created with SOCK_NONBLOCK and errno is EWOULDBLOCK.
     */
    ssize_t unix_dgram::sndto(const string& buf, const string& path, int sendto_flags)
    {
	return sndto(static_cast<const void*>(buf.c_str()),buf.size(),path.c_str(),sendto_flags);
    }

    /**
     * @brief Receive data and store the sender's address
     *
     * @param buf Receive buffer
     * @param length Length of `buf`
     * @param source Buffer for sender's path
     * @param source_len `source`'s length
     * @param recvfrom_flags Flags for `recvfrom(2)`
     *
     * @returns How many bytes were received. Returns -1 if the socket was created with SOCK_NONBLOCK and errno is EWOULDBLOCK.
     */
    ssize_t unix_dgram::rcvfrom(void* buf, size_t length, char* source, size_t source_len, int recvfrom_flags)
    {
	if ( buf == NULL )
	    throw socket_exception(__FILE__,__LINE__,"unix_dgram::rcvfrom: Buffer is NULL!",false);

	ssize_t bytes;

	bytes = recvfrom_unix_dgram_socket(sfd,buf,length,source,source_len,recvfrom_flags);

	if ( bytes < 0 )
	{
	    if ( is_nonblocking && errno == EWOULDBLOCK )
		return -1;
	    else
		throw socket_exception(__FILE__,__LINE__,"unix_dgram::rcvfrom: Could not receive data from peer!");
	}

	return bytes;
    }

    /**
     * @brief Receive data and store the sender's address
     *
     * @param buf Receive buffer
     * @param length Length of `buf`
     * @param source Buffer for sender's path. The path is truncated to `source.size()` characters.
     * @param recvfrom_flags Flags for `recvfrom(2)`
     *
     * @returns How many bytes were received. Returns -1 if the socket was created with SOCK_NONBLOCK and errno is EWOULDBLOCK.
     */
    ssize_t unix_dgram::rcvfrom(void* buf, size_t length, string& source, int recvfrom_flags)
    {
	if ( buf == NULL )
	    throw socket_exception(__FILE__,__LINE__,"unix_dgram::rcvfrom: Buffer is NULL!",false);

	ssize_t bytes;

	using std::unique_ptr;

	unique_ptr<char[]> source_cstr(new char[108]); // AFAIK, the address field in struct sockaddr_un is only 108 bytes long...
	size_t source_cstr_len;

	memset(source_cstr.get(),0,108);

	bytes = recvfrom_unix_dgram_socket(sfd,buf,length,source_cstr.get(),107,recvfrom_flags);

	if ( bytes < 0 )
	{
	    if ( is_nonblocking && errno == EWOULDBLOCK )
		return -1;
	    else
		throw socket_exception(__FILE__,__LINE__,"unix_dgram::rcvfrom: Could not receive data from peer!");
	}

	source_cstr_len = strlen(source_cstr.get());

	source.resize(source_cstr_len);

	source = source_cstr.get();

	return bytes;
    }

    /**
     * @brief Receive data and store the sender's address
     *
     * @param buf Receive buffer. The data is truncated to `buf.size()` characters.
     * @param source Buffer for sender's path. The path is truncated to `source.size()` characters.
     * @param recvfrom_flags Flags for `recvfrom(2)`
     *
     * @returns How many bytes were received. Returns -1 if the socket was created with SOCK_NONBLOCK and errno is EWOULDBLOCK.
     */
    ssize_t unix_dgram::rcvfrom(string& buf, string& source, int recvfrom_flags)
    {
	if ( buf.empty() )
	    throw socket_exception(__FILE__,__LINE__,"unix_dgram::rcvfrom: Buffer is empty!",false);

	ssize_t bytes;

	using std::unique_ptr;

	unique_ptr<char[]> source_cstr(new char[108]); // AFAIK, the address field in struct sockaddr_un is only 108 bytes...
	unique_ptr<char[]> cbuf(new char[buf.size()]);

	size_t source_cstr_len;

	memset(source_cstr.get(),0,108);

	bytes = recvfrom_unix_dgram_socket(sfd,cbuf.get(),buf.size(),source_cstr.get(),107,recvfrom_flags);

	if ( bytes < 0 )
	{
	    if ( is_nonblocking && errno == EWOULDBLOCK )
		return -1;
	    else
		throw socket_exception(__FILE__,__LINE__,"unix_dgram::rcvfrom: Could not receive data from peer!");
	}

	source_cstr_len = strlen(source_cstr.get());

	source.resize(source_cstr_len);
	buf.resize(bytes);

	buf.assign(cbuf.get(),bytes);
	source.assign(source_cstr.get(),source_cstr_len);

	return bytes;
    }
}
