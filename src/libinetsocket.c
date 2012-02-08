# include <stdlib.h>
# include <sys/socket.h> 
# include <sys/types.h>
# include <unistd.h> // read()/write()
# include <stdint.h>
# include <netdb.h> // getaddrinfo()
# include <string.h>
# include <errno.h>

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

// Macro definitions

# define VERBOSE // Write errors on stderr?

# define BACKLOG 128 // Linux accepts a backlog value at listen() up to 128

// Symbolic macros

# define TCP 1
# define UDP 2

# define IPv4 3
# define IPv6 4

# define BOTH 5 // what fits best (TCP/UDP or IPv4/6)

# define READ  1
# define WRITE 2

static inline signed int check_error(int return_value)
{
	const char* errbuf;

	if ( return_value < 0 )
	{
# ifdef VERBOSE
		errbuf = strerror(errno);
		write(2,errbuf,strlen(errbuf));
#endif
		return -1;
	}

	return 0;
}

/*
 * Client part
 *
*/

// Creates socket, connects it and gives it back
//                Hostname          Port/Service         Transport protocol (TCP or UDP)  Network Protocol (IPv4 or IPv6)
int create_isocket(const char* host, const char* service, char proto_osi4,                 char proto_osi3)
{
	int sfd, return_value;
	struct addrinfo hint, *result, *result_check;
        const char* errstring;

	memset(&hint,0,sizeof hint);

	// set address family
	switch ( proto_osi3 )
	{
		case IPv4:
			hint.ai_family = AF_INET;
			break;
		case IPv6:
			hint.ai_family = AF_INET6;
			break;
		case BOTH:
			hint.ai_family = AF_UNSPEC;
			break;
		default:
			return -1;
	}

	// set transport protocol
	switch ( proto_osi4 )
	{
		case TCP:
			hint.ai_socktype = SOCK_STREAM;
			break;
		case UDP:
			hint.ai_socktype = SOCK_DGRAM;
			break;
		case BOTH:
			// memset set struct to 0 - we don't have to set it again to 0
			break;		
		default:
			return -1;
	}


	if ( 0 != (return_value = getaddrinfo(host,service,&hint,&result)))
	{
		errstring = gai_strerror(return_value);
#ifdef VERBOSE
		write(2,errstring,strlen(errstring));
#endif
		return -1;
	}

	// As described in "The Linux Programming Interface", Michael Kerrisk 2010, chapter 59.11 (p. 1220ff)
	
	for ( result_check = result; result_check != NULL; result_check = result_check->ai_next ) // go through the linked list of struct addrinfo elements
	{
		sfd = socket(result_check->ai_family, result_check->ai_socktype, result_check->ai_protocol);

		if ( sfd < 0 ) // Error!!!
			continue;

		if ( -1 != connect(sfd,result_check->ai_addr,result_check->ai_addrlen)) // connected without error
			break;

		close(sfd);
	}
	
	// We do now have a working socket connection to our target

	if ( result_check == NULL )
	{
#ifdef VERBOSE
		write(2,"Could not connect to any address!\n",34);
#endif
		return -1;
	}

	freeaddrinfo(result);
	
	return sfd;
}

// Destroy a socket
//		   Socket file descriptor
int destroy_isocket(int sfd)
{
	if ( -1 == check_error(shutdown(sfd,SHUT_RDWR)))
		return -1;

	if ( -1 == check_error(close(sfd)))
		return -1;

	return 0;
}

int shutdown_isocket(int sfd, int method)
{
	if ( (method & READ) == method ) // READ is set (0001 && 0001 => 0001)
	{
		if ( -1 == check_error(shutdown(sfd,SHUT_RD)))
			return -1;

	} else if ( (method & WRITE) == method ) // WRITE is set (0010 && 0010 => 0010)
	{
		if ( -1 == check_error(shutdown(sfd,SHUT_WR)))
			return -1;

	} else if ( (method & (READ|WRITE)) == method ) // READ | WRITE is set (0011 && 0011 => 0011)
	{
		if ( -1 == check_error(shutdown(sfd,SHUT_RDWR)))
			return -1;
	}
	return 0;
}

/*
 * Server part
 *
*/
// create_issocket() (Create Internet Server Socket)
//		   Bind address		   Port			  TCP/UDP	   IPv4/6
int create_issocket(const char* bind_addr, const char* bind_port, char proto_osi4, char proto_osi3)
{
	int sfd, domain, type, retval;
	struct addrinfo *result, *result_check, hints;
	const char* errstr;

	switch ( proto_osi4 )
	{
		case TCP:
			type = SOCK_STREAM;
			break;
		case UDP:
			type = SOCK_DGRAM;
			break;
		default:
			return -1;
	}
	switch ( proto_osi3 )
	{
		case IPv4:
			domain = AF_INET;
			break;
		case IPv6:
			domain = AF_INET6;
			break;
		default:
			return -1;
	}

	memset(&hints,0,sizeof(struct addrinfo));

	hints.ai_socktype = type;
	hints.ai_family = domain;

	if ( -1 == (retval = getaddrinfo(bind_addr,bind_port,&hints,&result)) )
	{
# ifdef VERBOSE
		errstr = gai_strerror(retval);
		write(2,errstr,strlen(errstr));
# endif
		return -1;
	}

	// As described in "The Linux Programming Interface", Michael Kerrisk 2010, chapter 59.11 (p. 1220ff)
	for ( result_check = result; result_check != NULL; result_check = result_check->ai_next ) // go through the linked list of struct addrinfo elements
	{
		sfd = socket(result_check->ai_family, result_check->ai_socktype, result_check->ai_protocol);

		if ( sfd < 0 ) // Error at socket()!!!
			continue;

		retval = bind(sfd,result_check->ai_addr,(socklen_t)result_check->ai_addrlen);

		if ( retval != 0 ) // Error at bind()!!!
			continue;

		retval = listen(sfd,BACKLOG);

		if ( retval == 0 ) // If we came until here, there wasn't an error anywhere. It is safe to cancel the loop here
			break;

	}

	if ( result_check == NULL )
	{
#ifdef VERBOSE
		write(2,"Could not connect to any address!\n",34);
#endif
		return -1;
	}

	// We do now have a working socket connection to our target on which we may call accept()

	return sfd;
}
