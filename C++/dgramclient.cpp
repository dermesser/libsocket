# include <string>
# include <unistd.h>
# include <string.h>

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
 * @file dgramclient.cpp
 * @brief Base class for all datagram-based socket classes
 *
 * 	dgramclient.cpp contains the class dgram_client_socket. This
 * 	class provides the read/write functions for connected datagram
 * 	sockets (alias client dgram sockets...). This helps to avoid
 * 	writing the same code more than one time.
 * 	The client datagram classes for specific domains which are
 * 	unix_dgram_client and inet_dgram_client inherit the functions
 * 	declared here to get the ability to send and receive
 * 	data via connected datagram sockets. It is not possible to use
 * 	a class which has this functions also for stream sockets because
 * 	the functions have to check if the socket is connected (bool connected).
 * 	In the stream classes, this check is realized by checking if the file
 * 	descriptor is -1 or not.
 * @addtogroup libsocketplusplus
 * @{
 */

# include <exception.hpp>
# include <dgramclient.hpp>

namespace libsocket
{
    using std::string;

    dgram_client_socket::dgram_client_socket(void)
	: connected(false)
    {}

    /**
     * @brief Receive data from a connected DGRAM socket
     *
     * If a datagram socket is connected, this function may be called to receive data sent from
     * the host connected to.
     *
     * @param buf Area to write the data to
     * @param len How many data we want to receive
     * @param flags Flags to be passed to `recv(2)`
     *
     * @retval >0 n bytes were received.
     * @retval 0 0 bytes were received. (EOF?)
     * @retval -1 Something went wrong.
     */
    ssize_t dgram_client_socket::rcv(void* buf, size_t len, int flags)
    {
	ssize_t bytes;

	memset(buf,0,len);

	if ( -1 == (bytes = recv(sfd,buf,len,flags)) )
	    throw socket_exception(__FILE__,__LINE__,"dgram_client_socket::rcv() - recv() failed!");

	return bytes;
    }

    /**
     * @brief Receive data from connected datagram socket
     *
     * If a datagram socket is connected, you may receive data from it using stream-like functions.
     *
     * @param sock The socket to receive data from
     * @param dest The string to write data to. This string has to be resized to the number of bytes you wish to receive.
     */
    dgram_client_socket& operator>>(dgram_client_socket& sock, string& dest)
    {
	ssize_t read_bytes;

	char* buffer = new char[dest.size()];

	memset(buffer,0,dest.size());

	if ( -1 == (read_bytes = read(sock.sfd,buffer,dest.size())) )
	{
	    delete[] buffer;
	    if ( sock.is_nonblocking && errno == EWOULDBLOCK )
	    {
		dest.clear();
		return sock;
	    } else
		throw socket_exception(__FILE__,__LINE__,">>(dgram_client_socket, std::string) input: Error while reading!");
	}

	if ( read_bytes < static_cast<ssize_t>(dest.size()) )
	    dest.resize(read_bytes); // So the client doesn't print content more than one time
	// and it can check if the string's length is 0 (end of transmission)

	dest.assign(buffer,read_bytes);

	delete[] buffer;

	return sock;
    }

    /**
     * @brief Send data to connected socket
     *
     * @param buf Pointer to the data
     * @param len The length of the buffer
     * @param flags Flags for `send(2)`
     *
     * @retval n *n* bytes were sent
     * @retval <0 An error occurred.
     */
    ssize_t dgram_client_socket::snd(const void* buf, size_t len, int flags)
    {
	ssize_t bytes;

	if ( connected != true )
	    throw socket_exception(__FILE__,__LINE__,"dgram_client_socket::snd() - Socket is not connected!", false);

	if ( -1 == (bytes = send(sfd,buf,len,flags)) )
	    throw socket_exception(__FILE__,__LINE__,"dgram_client_socket::snd() - send() failed!");

	return bytes;
    }

    /**
     * @brief Send data to connected peer
     *
     * Usage: `socket << "Abcde";`
     */
    dgram_client_socket& operator<<(dgram_client_socket& sock, const char* str)
    {
	if ( str == NULL )
	    throw socket_exception(__FILE__,__LINE__,"dgram_client_socket <<(const char*) output: Null buffer given!");
	if ( sock.connected == false )
	    throw socket_exception(__FILE__,__LINE__,"dgram_client_socket <<(const char*) output: DGRAM socket not connected!");

	size_t len = strlen(str);

	if ( -1 == write(sock.sfd,str,len) )
	    throw socket_exception(__FILE__,__LINE__,"dgram_client_socket <<(const char*) output: Write failed!");

	return sock;
    }

    /**
     * @brief Send data to connected peer
     *
     * Usage: `socket << "Abcde";`
     */
    dgram_client_socket& operator<<(dgram_client_socket& sock, const string& str)
    {
	if ( sock.connected == false )
	    throw socket_exception(__FILE__,__LINE__,"dgram_client_socket <<(std::string) output: DGRAM socket not connected!");
	if ( -1 == write(sock.sfd,str.c_str(),str.size()) )
	    throw socket_exception(__FILE__,__LINE__,"dgram_client_socket <<(std::string) output: Write failed!");

	return sock;
    }

    /**
     * @deprecated (use is_connected())
     *
     * Look up if socket is connected.
     */
    bool dgram_client_socket::getconn(void) const
    {
	return is_connected();
    }

    /**
     * Returns true if the socket is in a connected state.
     */
    bool dgram_client_socket::is_connected(void) const
    {
        return connected;
    }
}

/**
 * @}
 */
