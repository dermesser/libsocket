# include <string.h>
# include <string>
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
 * @file inetserverstream.cpp
 * @brief INET server class
 *
 * 	inet_stream_server provides the TCP server part of libsocket.
 *	It's main function is accept() which returns a pointer to
 *	a dynamically allocated inet_stream (client socket) class which
 *	provides the connection to the client. You may setup the socket
 *	either with the second constructor or with setup()
 */

# include <conf.h>

# include <libinetsocket.h>
# include <inetclientstream.hpp>
# include <exception.hpp>
# include <inetserverstream.hpp>

#include <fcntl.h>
#ifndef SOCK_NONBLOCK
# define SOCK_NONBLOCK O_NONBLOCK
#endif

namespace libsocket
{
    using std::string;

    /**
     * @brief Void constructor; don't forget to setup() the socket before use!
     */
    inet_stream_server::inet_stream_server(void)
    {
    }

    /**
     * @brief Set up a server socket.
     *
     * This constructor initializes a server socket for TCP/IP communication.
     *
     * @param bindhost The address the server should listen on
     * @param bindport The port the server should listen on
     * @param proto_osi3 The protocol: `LIBSOCKET_IPv4/LIBSOCKET_IPv6`
     * @param flags Flags for `socket(2)`
     */
    inet_stream_server::inet_stream_server(const char* bindhost, const char* bindport, int proto_osi3, int flags)
    {
	setup(bindhost,bindport,proto_osi3,flags);
    }

    /**
     * @brief Set up a server socket.
     *
     * This constructor initializes a server socket for TCP/IP communication.
     *
     * @param bindhost The address the server should listen on
     * @param bindport The port the server should listen on
     * @param proto_osi3 The protocol: `LIBSOCKET_IPv4/LIBSOCKET_IPv6`
     * @param flags Flags for `socket(2)`
     */
    inet_stream_server::inet_stream_server(const string& bindhost, const string& bindport, int proto_osi3, int flags)
    {
	setup(bindhost,bindport,proto_osi3,flags);
    }

    /**
     * @brief Set up a server socket.
     *
     * If the zero-argument constructor was used, this method
     * initializes a server socket for TCP/IP communication.
     *
     * @param bindhost The address the server should listen on
     * @param bindport The port the server should listen on
     * @param proto_osi3 The protocol: `LIBSOCKET_IPv4/LIBSOCKET_IPv6`
     * @param flags Flags for `socket(2)`
     */
    void inet_stream_server::setup(const char* bindhost, const char* bindport, int proto_osi3, int flags)
    {
	if ( sfd != -1 )
	    throw socket_exception(__FILE__,__LINE__,"inet_stream_server::inet_stream_server() - already bound and listening!",false);
	if ( bindhost == 0 || bindport == 0 )
	    throw socket_exception(__FILE__,__LINE__,"inet_stream_server::inet_stream_server() - at least one bind argument invalid!",false);
	if ( -1 == (sfd = create_inet_server_socket(bindhost,bindport,LIBSOCKET_TCP,proto_osi3,flags)) )
	    throw socket_exception(__FILE__,__LINE__,"inet_stream_server::inet_stream_server() - could not create server socket!");

	host = string(bindhost);
	port = string(bindport);

	is_nonblocking = flags & SOCK_NONBLOCK;
    }

    /**
     * @brief Set up a server socket.
     *
     * If the zero-argument constructor was used, this method
     * initializes a server socket for TCP/IP communication.
     *
     * @param bindhost The address the server should listen on
     * @param bindport The port the server should listen on
     * @param proto_osi3 The protocol: `LIBSOCKET_IPv4/LIBSOCKET_IPv6`
     * @param flags Flags for `socket(2)`
     */
    void inet_stream_server::setup(const string& bindhost, const string& bindport, int proto_osi3, int flags)
    {
	if ( sfd != -1 )
	    throw socket_exception(__FILE__,__LINE__,"inet_stream_server::inet_stream_server() - already bound and listening!",false);
	if ( bindhost.empty() || bindport.empty() )
	    throw socket_exception(__FILE__,__LINE__,"inet_stream_server::inet_stream_server() - at least one bind argument invalid!",false);
	if ( -1 == (sfd = create_inet_server_socket(bindhost.c_str(),bindport.c_str(),LIBSOCKET_TCP,proto_osi3,flags)) )
	    throw socket_exception(__FILE__,__LINE__,"inet_stream_server::inet_stream_server() - could not create server socket!");

	host = string(bindhost);
	port = string(bindport);

	is_nonblocking = flags & SOCK_NONBLOCK;
    }

    /**
     * @brief Accept a connection and return a socket connected to the client.
     *
     * Waits for a client to connect and returns a pointer to a inet_stream object which can
     * be used to communicate with the client.
     *
     * @param numeric Specifies if the client's parameter (IP address, port) should be delivered numerically in the src_host/src_port parameters.
     * @param accept_flags Flags specified in `accept(2)`
     *
     * @returns A pointer to a connected TCP/IP client socket object.
     */
    inet_stream* inet_stream_server::accept(int numeric,int accept_flags)
    {
	if ( sfd < 0 )
	    throw socket_exception(__FILE__,__LINE__,"inet_stream_server::accept() - stream server socket is not in listening state -- please call first setup()!");

	using std::unique_ptr;
	unique_ptr<char[]> src_host(new char[1024]);
	unique_ptr<char[]> src_port(new char[32]);

	memset(src_host.get(),0,1024);
	memset(src_port.get(),0,32);

	int client_sfd;
	inet_stream* client = new inet_stream;

	if ( -1 == (client_sfd = accept_inet_stream_socket(sfd,src_host.get(),1023,src_port.get(),31,numeric,accept_flags)) )
	{
	    if ( ! is_nonblocking && errno != EWOULDBLOCK )
	    {
		throw socket_exception(__FILE__,__LINE__,"inet_stream_server::accept() - could not accept new connection on stream server socket!");
	    } else
	    {
		return nullptr; // Only return NULL but don't throw an exception if the socket is nonblocking
	    }
	}

	client->sfd = client_sfd;
	client->host = string(src_host.get()); // these strings are destructed automatically when the returned object is deleted. (http://stackoverflow.com/a/6256543)
	client->port = string(src_port.get()); //
	client->proto = proto;

	return client;
    }

    /**
     * @brief Accept a connection and return a socket connected to the client.
     *
     * The caller owns the client socket.
     *
     * @param numeric Specifies if the client's parameter (IP address, port) should be delivered numerically in the src_host/src_port parameters.
     * @param accept_flags Flags specified in `accept(2)`
     *
     * @returns An owned pointer to a connected TCP/IP client socket object.
     */
    unique_ptr<inet_stream> inet_stream_server::accept2(int numeric, int accept_flags)
    {
        return unique_ptr<inet_stream>(accept(numeric, accept_flags));
    }

    const string& inet_stream_server::getbindhost(void)
    {
	return gethost();
    }

    const string& inet_stream_server::getbindport(void)
    {
	return getport();
    }
}
