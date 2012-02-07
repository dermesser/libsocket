# include <stdlib.h>
# include <sys/socket.h> 
# include <sys/types.h>
# include <unistd.h> // read()/write()
# include <sys/un.h> // UNIX domain sockets
# include <stdint.h>
# include <netdb.h> // getaddrinfo()
# include <string.h>

/*
 * libsocket

    Copyright 2012 dermesser <lbo@spheniscida.de>. All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
    following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
    disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
    following disclaimer in the documentation and/or other materials provided with the distribution.

            THIS SOFTWARE IS PROVIDED BY THE LIBSOCKET PROJECT "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
            LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
            SHALL THE LIBSOCKET PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
            CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
            PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
            OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
            POSSIBILITY OF SUCH DAMAGE.

    The views and conclusions contained in the software and documentation are those of the authors and should not be
    interpreted as representing official policies, either expressed or implied, of the libsocket Project.

*/

// Macro definitions

# define TCP 1
# define UDP 2

# define IPv4 3
# define IPv6 4

# define BOTH 5 // what fits best (TCP/UDP or IPv4/6)

// Creates socket, connects it and gives it back
//                Hostname          Port/Service         Transport protocol (TCP or UDP)  Network Protocol (IPv4 or IPv6)
int create_socket(const char* host, const char* service, char proto_osi4,                 char proto_osi3)
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
	}

	return_value = getaddrinfo(host,service,&hint,&result);

	if ( return_value != 0 )
	{
		errstring = gai_strerror(return_value);
		write(2,errstring,strlen(errstring));
		exit(EXIT_FAILURE);
	}

	// As described in "The Linux Programming Interface", Michael Kerrisk 2010, chapter 59.11 (p. 1220ff)
	
	for ( result_check = result; result_check != NULL; result_check = result_check->ai_next ) // go through the linked list of struct addrinfo elements
	{
		sfd = socket(result_check->ai_family, result_check->ai_socktype, result_check->ai_protocol);

		if ( sfd < 0 ) // Error!!!
			continue;

		return_value = connect(sfd,result_check->ai_addr,result_check->ai_addrlen);

		if ( return_value != -1 ) // connected without error
			break;

		close(sfd);
	}
	
	// We do now have a valid socket connection to our target

	if ( result_check == NULL )
	{
		write(2,"Could not connect to any address!\n",34);
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(result);
	
	return sfd;
}

// Destroy a socket
//		   Socket file descriptor
int destroy_socket(int sfd)
{
	int return_value;
	const char* errbuf;

	return_value = shutdown(sfd,SHUTD_RDWR);

	if ( return_value != 0 )
	{
		errbuf = strerror(errno);
		write(2,errbuf,strlen(errbuf));
		exit(EXIT_FAILURE);
	}

	close(sfd);
}
