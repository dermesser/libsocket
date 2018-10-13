# ifndef _GNU_SOURCE
# define _GNU_SOURCE
# endif

# include "conf.h"
#define LIBSOCKET_VERSION 2.4
#ifdef BD_ANDROID
#define LIBSOCKET_LINUX 0
#else
#define LIBSOCKET_LINUX 1
#endif
#define LIBSOCKET_FREEBSD 0
#define LIBSOCKET_SUNOS 0


# include <stdlib.h>
# include <stdio.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <unistd.h> // read()/write()
# include <stdint.h>
# include <netdb.h> // getaddrinfo()
# include <string.h>
# include <errno.h>
# include <sys/ioctl.h>
# include <net/if.h>
# include <netinet/in.h> // e.g. struct sockaddr_in on OpenBSD

/**
 * @file    libinetsocket.c
 *
 * @brief Contains all C libinetsocket functions.
 *
 * This is the main file for libinetsocket. It contains all functions
 * used to work with INET and INET6 sockets, both TCP and UDP.
 */
/**
 * @addtogroup libinetsocket
 * @{
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

/*
 * Structure of the functions defined here:
 *
 * <Declarations>
 * <Checks on passed arguments>
 * <actual code>
 *
 */

/* Macro definitions */

/*
# define VERBOSE // Write errors on stderr?
*/

# define LIBSOCKET_BACKLOG 128 ///< Linux accepts a backlog value at listen() up to 128

// Symbolic macros
# define LIBSOCKET_TCP 1 ///< Protocol flag
# define LIBSOCKET_UDP 2 ///< Protocol flag

# define LIBSOCKET_IPv4 3 ///< Address family flag
# define LIBSOCKET_IPv6 4 ///< Adress family flag
# define LIBSOCKET_BOTH 5 ///< Adress family flag: what fits best (TCP/UDP or IPv4/6; delegate the decision to `getaddrinfo()`)

# define LIBSOCKET_READ  1 ///< Flag for shutdown
# define LIBSOCKET_WRITE 2 ///< Flag for shutdown

# define LIBSOCKET_NUMERIC 1 ///< May be specified as flag for functions to signalize that the name resolution should not be performed.


/**
 * Writes an error to stderr without modifying errno.
 */
#define debug_write(str)                \
    {                                      \
	int verbose_errno_save = errno; \
	write(2,str,strlen(str));	                  \
	errno = verbose_errno_save;     \
    }


# ifdef __FreeBSD__
# define _TRADITIONAL_RDNS
# endif

/**
 * @brief Checks return value for error.
 *
 * Every value returned by a syscall is passed to this function. It returns 0
 * if the return value is ok or -1 if there was an error.
 * If the macro `VERBOSE` is defined, an appropriate message is printed to STDERR.
 *
 * @param return_value A return value from a syscall.
 *
 * @retval 0 The syscall was successful.
 * @retval -1 There was an error.
 */
static inline signed int check_error(int return_value)
{
# ifdef VERBOSE
    const char* errbuf;
# endif
    if ( return_value < 0 )
    {
# ifdef VERBOSE
	errbuf = strerror(errno);
	debug_write(errbuf);
# endif
	return -1;
    }

    return 0;
}

/**
 * @brief Create and connect a new TCP/IP socket
 *
 * This function returns a working client TCP/IP socket.
 *
 * @param host The host the socket will be connected to (everything resolvable, e.g. "::1", "8.8.8.8", "example.com")
 * @param service The host's port, either numeric or as service name ("http").
 * @param proto_osi3 `LIBSOCKET_IPv4` or `LIBSOCKET_IPv6`.
 * @param flags Flags to be passed to `socket(2)`. Most flags are Linux-only!
 *
 * @return A valid socket file descriptor.
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
	debug_write(errstring);
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
	debug_write("create_inet_stream_socket: Could not connect to any address!\n");
# endif
        int errno_saved = errno;
        close(sfd);
        errno = errno_saved;
        freeaddrinfo(result);
	return -1;
    }
    // Yes :)

    freeaddrinfo(result);

    return sfd;
}

/**
 * @brief Creates a new UDP/IP socket
 *
 * Returns an integer describing a DGRAM (UDP) socket. The socket is automatically bound to some port.
 *
 * @param proto_osi3 is LIBSOCKET_IPv4 (AF_INET) or LIBSOCKET_IPv6 (AF_INET6).
 * @param flags may be the flags specified in socket(2), i.e. SOCK_NONBLOCK and/or SOCK_CLOEXEC. More than one
 * flags may be ORed. This argument is only sensible on Linux >= 2.6.27!
 *
 * @return The socket file descriptor number, on error -1.
 *
 * To send and receive data with this socket use the functions explained below, sendto_inet_dgram_socket() and recvfrom_inet_dgram_socket().
 */
int create_inet_dgram_socket(char proto_osi3, int flags)
{
    int sfd;

    if (proto_osi3 != LIBSOCKET_IPv4 && proto_osi3 != LIBSOCKET_IPv6)
    {
# ifdef VERBOSE
	debug_write("create_inet_dgram_socket: osi3 argument invalid for DGRAM sockets\n");
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

/**
 * @brief This function is the equivalent to `sendto(2)`
 *
 * @param sfd is the *Socket File Descriptor* (every socket file descriptor argument in libsocket is called sfd) which
 * you got from create_inet_dgram_socket(). *The usage with STREAM sockets is not recommended and the result is undefined!*
 * @param buf is a pointer to some data.
 * @param size is the length of the buffer to which buf points.
 * @param host is the host to which we want to send the data. It's a string so you may specify everything what's resolved by
 * getaddrinfo(), i.e. an IP ("193.21.34.21") or a hostname ("example.net").
 * @param service is the port on the remote host. Like in host, you may specify the port either as number ("123") or as service string ("ntp", "http", "gopher").
 * @param sendto_flags is available on all platforms. The value given here goes directly to the internal sendto() call. The flags which may be specified differ between the
 * platforms.
 *
 * If it is not possible to send data at the moment, this call blocks excepted you specified SOCK_NONBLOCK when creating the socket.
 *
 * @retval n *n* bytes of data could be sent.
 * @retval -1 Error.
 */
ssize_t sendto_inet_dgram_socket(int sfd, const void* buf, size_t size, const char* host, const char* service, int sendto_flags)
{
    struct sockaddr_storage oldsock;
    struct addrinfo *result, *result_check, hint;
    socklen_t oldsocklen = sizeof(struct sockaddr_storage);
    int return_value;
# ifdef VERBOSE
    const char* errstring;
# endif

    if ( sfd < 0 )
	return -1;

    if ( buf == NULL )
	return -1;

    if ( size == 0 )
	return 0;

    if ( host == NULL || service == NULL )
	return -1;

    if ( -1 == check_error(getsockname(sfd,(struct sockaddr*)&oldsock,(socklen_t*)&oldsocklen)) )
	return -1;

    memset(&hint,0,sizeof(struct addrinfo));

/*
 * This works for Linux > 2.6.32
    socklen_t dom_len = sizeof(hint.ai_family);
    getsockopt(sfd,SOL_SOCKET,SO_DOMAIN,&hint.ai_family,&dom_len);
*/
    hint.ai_family = oldsock.ss_family;
    hint.ai_socktype = SOCK_DGRAM;

    if ( 0 != (return_value = getaddrinfo(host,service,&hint,&result)))
    {
# ifdef VERBOSE
	errstring = gai_strerror(return_value);
	debug_write(errstring);
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

    freeaddrinfo(result);

    return return_value;
}

/**
 * @brief Receive data from a UDP/IP socket
 *
 * Receives data like `recvfrom(2)`. Pointers may be `NULL`, then the information (e.g. the source port) is lost (you may use
 * NULL pointers if you're not interested in some information)
 *
 * @param sfd The socket file descriptor.
 * @param buffer Where the data will be written
 * @param size The size of `buffer`
 * @param src_host Where the sending host's name/IP will be stored
 * @param src_host_len `src_host`'s length
 * @param src_service Where the port on remote side will be written to
 * @param src_service_len `src_service`'s length
 * @param recvfrom_flags Flags for `recvfrom(2)`
 * @param numeric `LIBSOCKET_NUMERIC` if you want the names to remain unresolved.
 *
 * @retval n *n* bytes of data were received.
 * @retval 0 Peer sent EOF.
 * @retval <0 An error occurred.
 */
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
    else
	memset(buffer,0,size);

    if ( src_host )
	memset(src_host,0,src_host_len);
    if ( src_service )
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
	    debug_write(errstr);
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

/**
 * @brief Connect a UDP socket.
 *
 * If a datagram socket is connected, all data written to it (using `write(2)`) is sent to the peer
 * connected to and all data `read(2)` from it is data sent by the peer. Usually used by clients only.
 *
 * @param sfd The socket file descriptor
 * @param host The host to connect to
 * @param service The port/service specifier
 *
 * @retval 0 Success
 * @retval -1 Error.
 */
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
	debug_write(errstring);
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
	debug_write("connect_inet_dgram_socket: Could not connect to any address!\n");
# endif
        freeaddrinfo(result);
	return -1;
    }

    freeaddrinfo(result);

    return 0;
}

/**
 * @brief Close a socket.
 *
 * This function closes a socket. You may also use `close(2)`.
 *
 * @param sfd The file descriptor
 *
 * @retval 0 Closed socket successfully
 * @retval -1 Socket was already closed (other errors are very unlikely to occur)
 */
int destroy_inet_socket(int sfd)
{
    if ( sfd < 0 )
	return -1;

    if ( -1 == check_error(close(sfd)))
	return -1;

    return 0;
}

/**
 * @brief Perform a `shutdown(2)` call on a socket
 *
 * If you're done with writing or reading from a socket
 * you may signalize this to the OS and/or the peer. For
 * example, shutting down a socket for writing sends
 * the peer an EOF signal.
 *
 * @param sfd The socket
 * @param method `LIBSOCKET_READ` or `LIBSOCKET_WRITE` or the combination via `|`
 *
 * @retval 0 Everything's fine.
 * @retval -1 Something went wrong, e.g. the socket was closed, the file descriptor is invalid etc.
 */
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

/**
 * @brief Create a TCP or UDP server socket
 *
 * To accept connections from clients via TCP or receive datagrams via UDP, you need to create
 * a server socket. This function creates such a socket and `bind(2)`s
 * it to the specified address. If `proto_osi4` is `LIBSOCKET_TCP`, `listen(2)` is called, too.
 *
 * @param bind_addr Address to bind to. If you want to bind to every address use "0.0.0.0" or "::" (IPv6 wildcard)
 * @param bind_port The port to bind to. If you write a webserver, this will be "http" or "80" or "https" or "443".
 * @param proto_osi4 Either `LIBSOCKET_TCP` or `LIBSOCKET_UDP`. Server sockets in TCP and UDP differ only in that TCP sockets need a call to `listen(2)`
 * @param proto_osi3 Either `LIBSOCKET_IPv4`, `LIBSOCKET_IPv6` or `LIBSOCKET_BOTH`; latter means that the DNS resolver should decide.
 * @param flags The `flags` argument is passed ORed to the `type` argument of `socket(2)`; everything other than 0 does not make sense on other OSes than Linux.
 *
 * @retval >0 A working passive socket. Call `accept_inet_stream_socket()` next.
 * @retval <0 Something went wrong; for example, the addresses where garbage or the port was not free.
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
	debug_write(errstr);
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
        {
            close(sfd);
	    continue;
        }

	if (type == LIBSOCKET_TCP)
	    retval = listen(sfd,LIBSOCKET_BACKLOG);

	if ( retval == 0 ) // If we came until here, there wasn't an error anywhere. It is safe to cancel the loop here
	    break;
        else
            close(sfd);
    }

    if ( result_check == NULL )
    {
# ifdef VERBOSE
	debug_write("create_inet_server_socket: Could not bind to any address!\n");
# endif
        freeaddrinfo(result);
	return -1;
    }

    // We do now have a working socket on which we may call accept()

    freeaddrinfo(result);

    return sfd;
}

/**
 * @brief Accept a connection attempt on a server socket.
 *
 * This function accepts an incoming connection on a server socket.
 *
 * (the `src_*` arguments may be `NULL`, in which case the address is not stored)
 *
 * @param sfd The server socket
 * @param src_host Buffer where the client's address is copied to
 * @param src_host_len `src_host`'s length. If the hostname is longer than this, it is truncated.
 * @param src_service Buffer in which the client's port is stored
 * @param src_service_len Its size. If shorter than the hostname it gets truncated.
 * @param flags May be `LIBSOCKET_NUMERIC`; then there is no rDNS lookup and the IP and port number are stored as-is.
 * @param accept_flags Flags for `accept4(2)` (which is only used on Linux)
 *
 * @retval >0 A socket file descriptor which can be used to talk to the client
 * @retval <0 Error.
 */
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
# if LIBSOCKET_LINUX
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
	    debug_write(errstr);
# endif
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
            // Don't return with error on name resolution failure
	}

	strncpy(src_host,he->h_name,src_host_len);
	snprintf(src_service,src_service_len,"%u",sport);
# endif
    }

    return client_sfd;
}

/**
 * @brief Look up which address families a host supports.
 *
 * If you want to send a datagram to a host but you don't know
 * if it supports IPv4 or IPv6, use this function. It returns
 * the address family returned by a DNS lookup. On most systems IPv6
 * is the preferred address family.
 *
 * @param hostname The hostname of the host you want to look up.
 *
 * @retval LIBSOCKET_IPv4 Host supports only IPv4
 * @retval LIBSOCKET_IPv6 Host supports IPv6 (usually it supports IPv4 then, too)
 * @retval <0 Error.
 */
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
	debug_write(errstring);
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

/**
 * @brief Create a datagram socket and join to the multicast group `address`.
 *
 * This function creates a multicast socket bound to `address`. The only option set is
 * the `IP_MULTICAST_IF` (`IPV6_MULTICAST_IF`) option to avoid an explicit routing
 * entry for the multicast address.
 *
 * An option you want to set very likely is `IP_MULTICAST_LOOP`. Refer to `ip(7)` respectively
 * `ipv6(7)` for `setsockopt()` options; for example:
 *
 *     int c = 0;
 *     setsockopt(sfd,IPPROTO_IP,IP_MULTICAST_LOOP,&c,4);
 *
 * The group address and port is also used as arguments to `bind(2)`. After creating this socket, you
 * may use the usual I/O functions on it, i.e. sendto_inet_dgram_socket and recvfrom_inet_dgram_socket.
 *
 * @param group Group address. This address is also used to bind the socket
 * @param port Multicast port.
 * @param local For IPv4 multicast groups: The address of the interface to be used. Ignored for IPv6, NULL for kernel's choice
 *
 * @retval <0 Error (Check errno or use `LIBSOCKET_VERBOSE`)
 * @retval >=0 A valid file descriptor.
 *
 */
# ifdef LIBSOCKET_LINUX
int create_multicast_socket(const char* group, const char* port, const char* if_name)
{
    int sfd, return_value;
    struct sockaddr maddr, localif;
    struct addrinfo hints, *result;
    struct ip_mreqn mreq4;
    struct ipv6_mreq mreq6;
    struct in_addr any;
    struct ifreq interface;

    memset(&maddr,0,sizeof(maddr));
    memset(&localif,0,sizeof(localif));
    memset(&mreq4,0,sizeof(mreq4));
    memset(&mreq6,0,sizeof(mreq6));
    memset(&hints,0,sizeof(hints));
    memset(&interface,0,sizeof(interface));

    if ( -1 == check_error(sfd = create_inet_server_socket(group,port,LIBSOCKET_UDP,LIBSOCKET_BOTH,0)) )
    {
        return -1;
    }

    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_family = AF_UNSPEC;

    if ( 0 != (return_value = getaddrinfo(group,port,&hints,&result)) )
    {
        int errno_saved = errno;
# ifdef VERBOSE
	const char* errstring = gai_strerror(return_value);
	debug_write(errstring);
# endif
        close(sfd);
        errno = errno_saved;

        return -1;
    }

    if ( result->ai_family == AF_INET )
    {
        // Result is IPv4 address.
        mreq4.imr_multiaddr = ((struct sockaddr_in*)result->ai_addr)->sin_addr;

        if ( if_name == NULL )
        {
            mreq4.imr_ifindex = 0;
            any.s_addr = INADDR_ANY;
            mreq4.imr_address = any;
        } else
        {
            memcpy(interface.ifr_name,if_name,strlen(if_name) > IFNAMSIZ ? IFNAMSIZ : strlen(if_name));

            if ( -1 == check_error(ioctl(sfd,SIOCGIFINDEX,&interface)) )
            {
                int errno_saved = errno;
                close(sfd);
                errno = errno_saved;
                return -1;
            }

            mreq4.imr_ifindex = interface.ifr_ifindex;
        }

        if ( -1 == check_error(setsockopt(sfd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq4,sizeof(struct ip_mreqn))) )
        {
            int errno_saved = errno;
            close(sfd);
            errno = errno_saved;
            return -1;
        }
        if ( -1 == check_error(setsockopt(sfd,IPPROTO_IP,IP_MULTICAST_IF,&mreq4,sizeof(struct ip_mreqn))) )
        {
            int errno_saved = errno;
            close(sfd);
            errno = errno_saved;
            return -1;
        }

        // Setup finished.
        //
    } else if ( result->ai_family == AF_INET6 )
    {

        mreq6.ipv6mr_multiaddr = ((struct sockaddr_in6*)result->ai_addr)->sin6_addr;
        mreq6.ipv6mr_interface = 0;

        if ( if_name == NULL )
            mreq6.ipv6mr_interface = 0;
        else
        {
            memcpy(interface.ifr_name,if_name,strlen(if_name) > IFNAMSIZ ? IFNAMSIZ : strlen(if_name));

            if ( -1 == check_error(ioctl(sfd,SIOCGIFINDEX,&interface)) )
            {
                int errno_saved = errno;
                close(sfd);
                errno = errno_saved;
                return -1;
            }

            mreq6.ipv6mr_interface = interface.ifr_ifindex;
        }

        if ( -1 == check_error(setsockopt(sfd,IPPROTO_IPV6,IPV6_ADD_MEMBERSHIP,&mreq6,sizeof(struct ipv6_mreq))) )
        {
            int errno_saved = errno;
            close(sfd);
            errno = errno_saved;
            return -1;
        }
        if ( -1 == check_error(setsockopt(sfd,IPPROTO_IPV6,IPV6_MULTICAST_IF,&mreq6.ipv6mr_interface,sizeof(mreq6.ipv6mr_interface))) )
        {
            int errno_saved = errno;
            close(sfd);
            errno = errno_saved;
            return -1;
        }

    }

    return sfd;
}

# endif

/**
 * @}
 */

#undef debug_write
