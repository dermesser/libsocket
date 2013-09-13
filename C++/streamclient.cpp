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
 * @file streamclient.cpp
 * @brief Basic stream I/O (read/write/send/recv)
 *
 * This class provides, similar to dgramclient.cpp resp.
 * dgram_client_socket, the basic I/O functions for all
 * stream-based sockets (TCP and UNIX-SOCK_STREAM).
 */

# include <string>
# include <unistd.h>
# include <string.h>
# include <memory>
// Inclusion here prevents further inclusion from headers/socket.hpp
namespace BERKELEY {
# include <sys/socket.h>
} // Against conflicts (shutdown(2)...)

# include <libsocket/libinetsocket.h>
# include <libsocket/exception.hpp>
# include <libsocket/streamclient.hpp>

namespace libsocket
{
    /**
     * @brief Void constructor
     */
    stream_client_socket::stream_client_socket()
	: shut_rd(false), shut_wr(false)
    {}

    /**
     * @brief Receive data from socket
     *
     * ...and puts it in `buf`.
     *
     * @param buf A writable memory buffer of length `len`
     * @param len Length of `buf`
     * @param flags Flags for `recv(2)`. WARNING: Throws an exception if `recv()` returns -1; this may be the case if the flag `MSG_DONTWAIT` is used.
     *
     * @returns The length of received data
     */
    ssize_t stream_client_socket::rcv(void* buf, size_t len, int flags)
    {
	ssize_t recvd;

	if ( shut_rd == true )
	    throw socket_exception(__FILE__,__LINE__,"stream_client_socket::rcv() - Socket has already been shut down!\n");

	if ( sfd == -1 )
	    throw socket_exception(__FILE__,__LINE__,"stream_client_socket::rcv() - Socket is not connected!\n");

	if ( buf == NULL || len == 0 )
	    throw socket_exception(__FILE__,__LINE__,"stream_client_socket::rcv() - Buffer or length is null!\n");

	memset(buf,0,len);

	if ( -1 == (recvd = BERKELEY::recv(sfd,buf,len,flags)) )
	    throw socket_exception(__FILE__,__LINE__,"stream_client_socket::rcv() - Error while reading!\n");

	return recvd;
    }

    /**
     * @brief Receive data from socket to a string
     *
     * Receives n bytes of data (where `n == dest.size()`) and writes it to a string.
     *
     * Application: infix; `sock >> dest1 [>> dest2...];`
     *
     * @param sock the socket.
     * @param dest the destination string. Its length determines how much data is received.
     *
     * @returns the same socket which was given as parameter. (See "Application")
     */
    stream_client_socket& operator>>(stream_client_socket& sock, string& dest)
    {
	ssize_t read_bytes;

	if ( sock.shut_rd == true )
	    throw socket_exception(__FILE__,__LINE__,"stream_client_socket::operator>>(std::string) - Socket has already been shut down!\n");

        using std::unique_ptr;
        unique_ptr<char[]> buffer(new char[dest.size()]);

	memset(buffer.get(),0,dest.size());

	if ( sock.sfd == -1 )
	{
	    throw socket_exception(__FILE__,__LINE__,">>(std::string) input: Socket not connected!\n");
	}

	if ( -1 == (read_bytes = read(sock.sfd,buffer.get(),dest.size())) )
	{
	    throw socket_exception(__FILE__,__LINE__,">>(std::string) input: Error while reading!\n");
	}

	if ( read_bytes < static_cast<ssize_t>(dest.size()) )
	    dest.resize(read_bytes); // So the client doesn't print content more than one time
                                     // and it can check if the string's length is 0 (end of transmission)

	dest.assign(buffer.get(),read_bytes);

	return sock;
    }

    // O

    /**
     * @brief Send data to socket
     *
     * Sends data to socket using stream-like syntax:
     *
     * `socket << "Hello " << "World" << "\n";`
     *
     * Important: Only overloaded for C and C++ strings, not for numbers, chars etc.!
     *
     * @param sock A socket.
     * @param str Data to be sent; do a `static_cast<>()` to send raw data.
     *
     */
    stream_client_socket& operator<<(stream_client_socket& sock, const char* str)
    {
	if ( sock.shut_wr == true )
	    throw socket_exception(__FILE__,__LINE__,"stream_client_socket::operator<<(const char*) - Socket has already been shut down!\n");
	if ( sock.sfd == -1 )
	    throw socket_exception(__FILE__,__LINE__,"<<(const char*) output: Socket not connected!\n");
	if ( str == NULL )
	    throw socket_exception(__FILE__,__LINE__,"<<(const char*) output: Null buffer given!\n");

	size_t len = strlen(str);

	if ( -1 == write(sock.sfd,str,len) )
	    throw socket_exception(__FILE__,__LINE__,"<<(const char*) output: Write failed!\n");

	return sock;
    }

    /**
     * @brief Send data to socket
     *
     * Sends data to socket using stream-like syntax:
     *
     * `socket << "Hello " << "World" << "\n";`
     *
     * Important: Only overloaded for C and C++ strings, not for numbers, chars etc.!
     *
     * @param sock A socket.
     * @param str Data to be sent; do a `static_cast<>()` to send raw data.
     *
     */
    stream_client_socket& operator<<(stream_client_socket& sock, string& str)
    {
	if ( sock.shut_wr == true )
	    throw socket_exception(__FILE__,__LINE__,"stream_client_socket::operator<<(std::string) - Socket has already been shut down!\n");
	if ( sock.sfd == -1 )
	    throw socket_exception(__FILE__,__LINE__,"<<(std::string) output: Socket not connected!\n");

	if ( -1 == write(sock.sfd,str.c_str(),str.size()) )
	    throw socket_exception(__FILE__,__LINE__,"<<(std::string) output: Write failed!\n");

	return sock;
    }

    /**
     * @brief Send data to socket
     *
     * @param buf Data to be sent
     * @param len Length of `buf`
     * @param flags Flags for `send(2)`. WARNING: Throws an exception if `send()` returns -1; this may be the case if the flag `MSG_DONTWAIT` is used.
     *
     * @returns The number of bytes sent to the peer.
     *
     */
    ssize_t stream_client_socket::snd(const void* buf, size_t len, int flags)
    {
	ssize_t snd_bytes;

	if ( shut_wr == true )
	    throw socket_exception(__FILE__,__LINE__,"stream_client_socket::snd() - Socket has already been shut down!\n");
	if ( sfd == -1 )
	    throw socket_exception(__FILE__,__LINE__,"stream_client_socket::snd() - Socket not connected!\n");
	if ( buf == NULL || len == 0 )
	    throw socket_exception(__FILE__,__LINE__,"stream_client_socket::snd() - Buffer or length is null!\n");

	if ( -1 == (snd_bytes = BERKELEY::send(sfd,buf,len,flags)) )
	    throw socket_exception(__FILE__,__LINE__,"stream_client_socket::snd() - Error while sending\n");

	return snd_bytes;
    }

    /**
     * @brief Shut a socket down
     *
     * Shuts a socket down using `shutdown(2)`.
     *
     * @param method `LIBSOCKET_READ/LIBSOCKET_WRITE` or an `OR`ed combination.
     */
    void stream_client_socket::shutdown(int method)
    {
	int u_method = 0; // unix flags

	// Already shut down using this method...
	if ( (method & (LIBSOCKET_READ|LIBSOCKET_WRITE)) && (shut_rd == true) && (shut_wr == true) )
	    return;
	if ( (method & LIBSOCKET_READ) && (shut_rd == true) )
	    return;
	if ( (method & LIBSOCKET_WRITE) && (shut_wr == true) )
	    return;

	if ( method == (LIBSOCKET_READ|LIBSOCKET_WRITE) )
	    u_method = BERKELEY::SHUT_RDWR;
	else if ( method == LIBSOCKET_READ )
	    u_method = BERKELEY::SHUT_RD;
	else if ( method == LIBSOCKET_WRITE )
	    u_method = BERKELEY::SHUT_WR;
	else // With no valid combination
	    return;

	if ( 0 > BERKELEY::shutdown(sfd,u_method)) // It's equal whether we use this or its brother from libunixsocket
	{
	    throw socket_exception(__FILE__,__LINE__,"stream_client_socket::shutdown() - Could not shutdown socket\n");
	}

	if ( method & LIBSOCKET_READ )
	    shut_rd = true;
	if ( method & LIBSOCKET_WRITE )
	    shut_wr = true;
    }
}
