# ifndef _GNU_SOURCE
# define _GNU_SOURCE
# endif

# include <stdlib.h>
# include <stdio.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <unistd.h> // read()/write()
# include <stdint.h>
# include <netdb.h> // getaddrinfo()
# include <string.h>
# include <errno.h>

# include <netinet/in.h> // e.g. struct sockaddr_in on OpenBSD

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

/*
 * Structure of the functions defined here:
 *
 * <Declarations>
 * <Checks on passed arguments>
 * <actual code>
 *
*/

// Macro definitions

//# define VERBOSE // Write errors on stderr?

# define LIBSOCKET_BACKLOG 128 // Linux accepts a backlog value at listen() up to 128
# define LIBSOCKET_CLIENT_NAME_BUF 1024 // How long is the buffer for the client's name?

// Symbolic macros
# define LIBSOCKET_TCP 1
# define LIBSOCKET_UDP 2

# define LIBSOCKET_IPv4 3
# define LIBSOCKET_IPv6 4

# define LIBSOCKET_BOTH 5 // what fits best (TCP/UDP or IPv4/6)

# define LIBSOCKET_READ  1
# define LIBSOCKET_WRITE 2

# define LIBSOCKET_NUMERIC 1


# ifdef VERBOSE
	#define debug_write(str,l)                \
	do {                                      \
		int __verbose_errno_save = errno; \
		write(2,str,l);	                  \
		errno = __verbose_errno_save;     \
	} while (0)
# endif


# ifdef __FreeBSD__
	# define _TRADITIONAL_RDNS
# endif

static inline signed int check_error(int return_value)
{
# ifdef VERBOSE
	const char* errbuf;
# endif
	if ( return_value < 0 )
	{
# ifdef VERBOSE
		errbuf = strerror(errno);
		debug_write(errbuf,strlen(errbuf));
# endif
		return -1;
	}

	return 0;
}

/*
 * Client part
 *
*/

int create_inet_stream_socket(const char* host, const char* service, char proto_osi3, int flags)
{
	int sfd, return_value;
	struct addrinfo hint, *result, *result_check;
# ifdef VERBOSE
	const char* errstring;
# endif

	if ( host == NULL || service == NULL )
		return -1;

	memset(&hint,0,sizeof hint);

	// set address family
	switch ( proto_osi3 )
	{
		case LIBSOCKET_IPv4:
			hint.ai_family = AF_INET;
			break;
		case LIBSOCKET_IPv6:
			hint.ai_family = AF_INET6;
			break;
		case LIBSOCKET_BOTH:
			hint.ai_family = AF_UNSPEC;
			break;
		default:
			return -1;
	}

	// Transport protocol is TCP
	hint.ai_socktype = SOCK_STREAM;

	if ( 0 != (return_value = getaddrinfo(host,service,&hint,&result)))
	{
# ifdef VERBOSE
		errstring = gai_strerror(return_value);
		debug_write(errstring,strlen(errstring));
# endif
		return -1;
	}

	// As described in "The Linux Programming Interface", Michael Kerrisk 2010, chapter 59.11 (p. 1220ff)

	for ( result_check = result; result_check != NULL; result_check = result_check->ai_next ) // go through the linked list of struct addrinfo elements
	{
		sfd = socket(result_check->ai_family, result_check->ai_socktype | flags, result_check->ai_protocol);

		if ( sfd < 0 ) // Error!!!
			continue;

		if ( -1 != connect(sfd,result_check->ai_addr,result_check->ai_addrlen)) // connected without error
			break;

		close(sfd);
	}

	// We do now have a working socket STREAM connection to our target

	if ( result_check == NULL ) // Have we?
	{
# ifdef VERBOSE
		debug_write("Could not connect to any address!\n",34);
# endif
		return -1;
	}
	// Yes :)

	freeaddrinfo(result);

	return sfd;
}

int create_inet_dgram_socket(char proto_osi3, int flags)
{
	int sfd;

	if (proto_osi3 != LIBSOCKET_IPv4 && proto_osi3 != LIBSOCKET_IPv6)
	{
# ifdef VERBOSE
		debug_write("create_inet_dgram_socket: osi3 argument invalid when using DGRAM sockets\n",48);
# endif
		return -1;
	}

	switch ( proto_osi3 )
	{
		case LIBSOCKET_IPv4 :
			sfd = socket(AF_INET,SOCK_DGRAM|flags,0);
			break;
		case LIBSOCKET_IPv6 :
			sfd = socket(AF_INET6,SOCK_DGRAM|flags,0);
			break;
		default:
			return -1;
	}

	if ( -1 == check_error(sfd) )
		return -1;

	return sfd;
}

//Working
ssize_t sendto_inet_dgram_socket(int sfd, const void* buf, size_t size,const char* host, const char* service, int sendto_flags)
{
	struct sockaddr_storage oldsock;
	struct addrinfo *result, *result_check, hint;
	size_t oldsocklen = sizeof(struct sockaddr_storage);
	int return_value;
# ifdef VERBOSE
	const char* errstring;
# endif

	if ( sfd < 0 )
		return -1;

	if ( buf == NULL || size == 0)
		return -1;

	if ( host == NULL || service == NULL )
		return -1;

	if ( -1 == check_error(getsockname(sfd,(struct sockaddr*)&oldsock,(socklen_t*)&oldsocklen)) )
		return -1;

	memset(&hint,0,sizeof(struct addrinfo));

	hint.ai_family = oldsock.ss_family;
	hint.ai_socktype = SOCK_DGRAM;

	if ( 0 != (return_value = getaddrinfo(host,service,&hint,&result)))
	{
# ifdef VERBOSE
		errstring = gai_strerror(return_value);
		debug_write(errstring,strlen(errstring));
# endif
		return -1;
	}

	for ( result_check = result; result_check != NULL; result_check = result_check->ai_next ) // go through the linked list of struct addrinfo elements
	{
		if ( -1 != (return_value = sendto(sfd,buf,size,sendto_flags,result_check->ai_addr,result_check->ai_addrlen))) // connected without error
		{
			break; // Exit loop if send operation was successful
		} else
		{
			check_error(return_value);
		}
	}

	return return_value;
}


// Get a single UDP packet
// 			 Socket   Target        Size of buffer string for client and its size     client port        its size		     may be NUMERIC (give host and service in numeric form)
ssize_t recvfrom_inet_dgram_socket(int sfd, void* buffer, size_t size, char* src_host, size_t src_host_len, char* src_service, size_t src_service_len, int recvfrom_flags, int numeric)
{
	struct sockaddr_storage client;

# ifdef _TRADITIONAL_RDNS
	struct sockaddr_storage oldsockaddr;
	socklen_t oldsockaddrlen = sizeof(struct sockaddr_storage);
	struct hostent* he;
	void* addrptr;
	size_t addrlen;
	uint16_t sport = 0;
# endif

	ssize_t bytes;

# ifndef _TRADITIONAL_RDNS
	int retval;
# endif

# ifdef VERBOSE
	const char* errstr;
# endif
	if ( sfd < 0 )
		return -1;

	if ( buffer == NULL || size == 0)
		return -1;

	memset(buffer,0,size);
	memset(src_host,0,src_host_len);
	memset(src_service,0,src_service_len);

	socklen_t stor_addrlen = sizeof(struct sockaddr_storage);

	if ( -1 == check_error(bytes = recvfrom(sfd,buffer,size,recvfrom_flags,(struct sockaddr*)&client,&stor_addrlen)))
		return -1;

	if ( src_host_len > 0 || src_service_len > 0 ) // If one of the things is wanted. If you give a null pointer with a positive _len parameter, you won't get the address.
	{
		if ( numeric == LIBSOCKET_NUMERIC )
		{
			numeric = NI_NUMERICHOST | NI_NUMERICSERV;
		}

// getnameinfo() doesn't work on FreeBSD (here)
# ifndef _TRADITIONAL_RDNS
		if ( 0 != (retval = getnameinfo((struct sockaddr*)&client,sizeof(struct sockaddr_storage),src_host,src_host_len,src_service,src_service_len,numeric)) ) // Write information to the provided memory
		{
# ifdef VERBOSE
			errstr = gai_strerror(retval);
			debug_write(errstr,strlen(errstr));
# endif
			return -1;
		}
# endif

// so use traditional methods
# ifdef _TRADITIONAL_RDNS
		if ( -1 == check_error(getsockname(sfd,(struct sockaddr*)&oldsockaddr,&oldsockaddrlen)) )
			return -1;

		if ( oldsockaddrlen > sizeof(struct sockaddr_storage) ) // If getsockname truncated the struct
			return -1;

		if ( oldsockaddr.ss_family == AF_INET )
		{
			addrptr = &(((struct sockaddr_in*)&client)->sin_addr);
			addrlen = sizeof(struct in_addr);
			sport = ntohs(((struct sockaddr_in*)&client)->sin_port);
		} else if ( oldsockaddr.ss_family == AF_INET6 )
		{
			addrptr = &(((struct sockaddr_in6*)&client)->sin6_addr);
			addrlen = sizeof(struct in6_addr);
			sport = ntohs(((struct sockaddr_in6*)&client)->sin6_port);
		}

		if ( NULL == (he = gethostbyaddr(addrptr,addrlen,oldsockaddr.ss_family) ) )
		{
			check_error(-1);
			return -1;
		}

		strncpy(src_host,he->h_name,src_host_len);
		snprintf(src_service,src_service_len,"%u",sport);
# endif
	}

	return bytes;
}

int connect_inet_dgram_socket(int sfd, const char* host, const char* service)
{
	struct addrinfo *result, *result_check, hint;
	struct sockaddr_storage oldsockaddr;
	struct sockaddr deconnect;
	socklen_t oldsockaddrlen = sizeof(struct sockaddr_storage);
	int return_value;
# ifdef VERBOSE
	const char* errstring;
# endif

	if ( sfd < 0 )
		return -1;

	if ( host == NULL )
	{
// This does not work on FreeBSD systems. We pretend to disconnect the socket although we don't do so. This is not very severe for the application
# ifndef __FreeBSD__
		memset(&deconnect,0,sizeof(struct sockaddr));

		deconnect.sa_family = AF_UNSPEC;

		if ( check_error(connect(sfd,&deconnect,sizeof(struct sockaddr))) )
			return -1;
# endif
		return 0;
	}

	if ( -1 == check_error(getsockname(sfd,(struct sockaddr*)&oldsockaddr,&oldsockaddrlen)) )
		return -1;

	if ( oldsockaddrlen > sizeof(struct sockaddr_storage) ) // If getsockname truncated the struct
		return -1;

	memset(&hint,0,sizeof(struct addrinfo));

	hint.ai_family = ((struct sockaddr_in*)&oldsockaddr)->sin_family; // AF_INET or AF_INET6 - offset is same at sockaddr_in and sockaddr_in6
	hint.ai_socktype = SOCK_DGRAM;

	if ( 0 != (return_value = getaddrinfo(host,service,&hint,&result)))
	{
# ifdef VERBOSE
		errstring = gai_strerror(return_value);
		debug_write(errstring,strlen(errstring));
# endif
		return -1;
	}

	// As described in "The Linux Programming Interface", Michael Kerrisk 2010, chapter 59.11 (p. 1220ff)

	for ( result_check = result; result_check != NULL; result_check = result_check->ai_next ) // go through the linked list of struct addrinfo elements
	{
		if ( -1 != (return_value = connect(sfd,result_check->ai_addr,result_check->ai_addrlen))) // connected without error
		{
			break;
		} else
		{
			check_error(return_value);
		}
	}

	// We do now have a working (updated) socket connection to our target

	if ( result_check == NULL ) // or not?
	{
# ifdef VERBOSE
		debug_write("Could not connect to any address!\n",34);
# endif
		return -1;
	}

	freeaddrinfo(result);

	return 0;
}


int destroy_inet_socket(int sfd)
{
	if ( sfd < 0 )
		return -1;

	if ( -1 == check_error(close(sfd)))
		return -1;

	return 0;
}

int shutdown_inet_stream_socket(int sfd, int method)
{
	if ( sfd < 0 )
		return -1;

	if ( (method != LIBSOCKET_READ) && (method != LIBSOCKET_WRITE) && (method != (LIBSOCKET_READ|LIBSOCKET_WRITE)) )
		return -1;

	if ( method & LIBSOCKET_READ ) // READ is set (0001 && 0001 => 0001 => true)
	{
		if ( -1 == check_error(shutdown(sfd,SHUT_RD)))
			return -1;

	}

	if ( method & LIBSOCKET_WRITE ) // WRITE is set (0010 && 0010 => 0010 => true)
	{
		if ( -1 == check_error(shutdown(sfd,SHUT_WR)))
			return -1;
	}

	return 0;
}

/*
 * Server part
 *
*/

//		              Bind address	   Port			  TCP/UDP	   IPv4/6
int create_inet_server_socket(const char* bind_addr, const char* bind_port, char proto_osi4, char proto_osi3, int flags)
{
	int sfd, domain, type, retval;
	struct addrinfo *result, *result_check, hints;
# ifdef VERBOSE
	const char* errstr;
# endif

	//if ( flags != SOCK_NONBLOCK && flags != SOCK_CLOEXEC && flags != (SOCK_CLOEXEC|SOCK_NONBLOCK) && flags != 0 )
	//	return -1;

	if ( bind_addr == NULL || bind_port == NULL )
		return -1;

	switch ( proto_osi4 )
	{
		case LIBSOCKET_TCP:
			type = SOCK_STREAM;
			break;
		case LIBSOCKET_UDP:
			type = SOCK_DGRAM;
			break;
		default:
			return -1;
	}
	switch ( proto_osi3 )
	{
		case LIBSOCKET_IPv4:
			domain = AF_INET;
			break;
		case LIBSOCKET_IPv6:
			domain = AF_INET6;
			break;
		case LIBSOCKET_BOTH:
			domain = AF_UNSPEC;
			break;
		default:
			return -1;
	}

	memset(&hints,0,sizeof(struct addrinfo));

	hints.ai_socktype = type;
	hints.ai_family = domain;
	hints.ai_flags = AI_PASSIVE;

	if ( 0 != (retval = getaddrinfo(bind_addr,bind_port,&hints,&result)) )
	{
# ifdef VERBOSE
		errstr = gai_strerror(retval);
		debug_write(errstr,strlen(errstr));
# endif
		return -1;
	}

	// As described in "The Linux Programming Interface", Michael Kerrisk 2010, chapter 59.11 (p. 1220ff)
	for ( result_check = result; result_check != NULL; result_check = result_check->ai_next ) // go through the linked list of struct addrinfo elements
	{
		sfd = socket(result_check->ai_family, result_check->ai_socktype | flags, result_check->ai_protocol);

		if ( sfd < 0 ) // Error at socket()!!!
			continue;

		retval = bind(sfd,result_check->ai_addr,(socklen_t)result_check->ai_addrlen);

		if ( retval != 0 ) // Error at bind()!!!
			continue;

		if (type == LIBSOCKET_TCP)
			retval = listen(sfd,LIBSOCKET_BACKLOG);

		if ( retval == 0 ) // If we came until here, there wasn't an error anywhere. It is safe to cancel the loop here
			break;
	}

	if ( result_check == NULL )
	{
# ifdef VERBOSE
		debug_write("Could not bind to any address!\n",34);
# endif
		return -1;
	}

	// We do now have a working socket on which we may call accept()

	freeaddrinfo(result);

	return sfd;
}

// Accept connections on TCP sockets
//	 		       Socket    Src string      Src str len          Src service        Src service len         NUMERIC?
int accept_inet_stream_socket(int sfd, char* src_host, size_t src_host_len, char* src_service, size_t src_service_len, int flags, int accept_flags)
{
	struct sockaddr_storage client_info;
	int client_sfd;

# ifndef _TRADITIONAL_RDNS
	int retval;
# endif

# ifdef _TRADITIONAL_RDNS
	struct sockaddr_storage oldsockaddr;
	socklen_t oldsockaddrlen = sizeof(struct sockaddr_storage);
	struct hostent* he;
	void* addrptr;
	size_t in_addrlen;
	uint16_t sport = 0;
# endif

# ifdef VERBOSE
	const char* errstr;
# endif
	socklen_t addrlen = sizeof(struct sockaddr_storage);

	// Portable behavior
# ifdef __linux__
	if ( -1 == check_error((client_sfd = accept4(sfd,(struct sockaddr*)&client_info,&addrlen,accept_flags)))) // blocks
		return -1;
# else
	if ( -1 == check_error((client_sfd = accept(sfd,(struct sockaddr*)&client_info,&addrlen)))) // blocks
		return -1;
# endif

	if ( src_host_len > 0 || src_service_len > 0 ) // If one of the things is wanted. If you give a null pointer with a positive _len parameter, you won't get the address.
	{
		if ( flags == LIBSOCKET_NUMERIC )
		{
			flags = NI_NUMERICHOST | NI_NUMERICSERV;
		} else
		{
			flags = 0; // To prevent errors: Unknown flags are ignored
		}

# ifndef _TRADITIONAL_RDNS
		if ( 0 != (retval = getnameinfo((struct sockaddr*)&client_info,sizeof(struct sockaddr_storage),src_host,src_host_len,src_service,src_service_len,flags)) ) // Write information to the provided memory
		{
# ifdef VERBOSE
			errstr = gai_strerror(retval);
			debug_write(errstr,strlen(errstr));
# endif
			return -1;
		}
# endif

# ifdef _TRADITIONAL_RDNS
		if ( -1 == check_error(getsockname(sfd,(struct sockaddr*)&oldsockaddr,&oldsockaddrlen)) )
			return -1;

		if ( oldsockaddrlen > sizeof(struct sockaddr_storage) ) // If getsockname truncated the struct
			return -1;

		if ( oldsockaddr.ss_family == AF_INET )
		{
			addrptr = &(((struct sockaddr_in*)&client_info)->sin_addr);
			in_addrlen = sizeof(struct in_addr);
			sport = ntohs(((struct sockaddr_in*)&client_info)->sin_port);
		} else if ( oldsockaddr.ss_family == AF_INET6 )
		{
			addrptr = &(((struct sockaddr_in6*)&client_info)->sin6_addr);
			in_addrlen = sizeof(struct in6_addr);
			sport = ntohs(((struct sockaddr_in6*)&client_info)->sin6_port);
		}

		if ( NULL == (he = gethostbyaddr(addrptr,in_addrlen,oldsockaddr.ss_family) ) )
		{
			check_error(-1);
			return -1;
		}

		strncpy(src_host,he->h_name,src_host_len);
		snprintf(src_service,src_service_len,"%u",sport);
# endif
	}

	return client_sfd;
}

int get_address_family(const char* hostname)
{
	int return_value;
	struct addrinfo hint, *result;
# ifdef VERBOSE
	const char* errstring;
# endif
	int af;

	if ( hostname == NULL )
		return -1;

	memset(&hint,0,sizeof hint);

	hint.ai_family = AF_UNSPEC;

	if ( 0 != (return_value = getaddrinfo(hostname,"0",&hint,&result)))
	{
# ifdef VERBOSE
		errstring = gai_strerror(return_value);
		debug_write(errstring,strlen(errstring));
# endif
		return -1;
	}

	if ( result == NULL )
		return -1;

	if ( result->ai_family == AF_INET )
	{
		af = LIBSOCKET_IPv4;
	} else if ( result->ai_family == AF_INET6 )
	{
		af = LIBSOCKET_IPv6;
	} else
	{
		af = -1;
	}

	return af;
}

#undef debug_write
