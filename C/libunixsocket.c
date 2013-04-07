# ifndef _GNU_SOURCE
# define _GNU_SOURCE // accept4()
# endif

# include <sys/socket.h>
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h> // read()/write()
# include <sys/un.h> // UNIX domain sockets
# include <stdint.h>
# include <string.h>
# include <errno.h>

/**
 * @addtogroup libunixsocket
 * @{
 * @file libunixsocket.c
 *
 * Contains all C functions to handle UNIX domain sockets.

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

//# define VERBOSE

// Macro definitions

# define LIBSOCKET_BACKLOG 128

# define LIBSOCKET_STREAM 1
# define LIBSOCKET_DGRAM  2

# define LIBSOCKET_READ  1
# define LIBSOCKET_WRITE 2

# ifdef VERBOSE
#define debug_write(str,l)                \
    do {                                      \
	int __verbose_errno_save = errno; \
	write(2,str,l);	                  \
	errno = __verbose_errno_save;     \
    } while (0)
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

int create_unix_stream_socket(const char* path, int flags)
{
    struct sockaddr_un saddr;
    int sfd;

    if ( path == NULL )
	return -1;

    if ( -1 == check_error(sfd = socket(AF_UNIX,SOCK_STREAM|flags,0)) )
	return -1;

    memset(&saddr,0,sizeof(struct sockaddr_un));

    if ( strlen(path) > (sizeof(saddr.sun_path)-1) )
    {
# ifdef VERBOSE
	debug_write("UNIX socket destination path too long\n",38);
# endif
	return -1;
    }

    saddr.sun_family = AF_UNIX;
    strncpy(saddr.sun_path,path,sizeof(saddr.sun_path)-1);

    if ( -1 == check_error(connect(sfd,(struct sockaddr*)&saddr,sizeof saddr)) )
	return -1;

    return sfd;
}

int create_unix_dgram_socket(const char* bind_path, int flags)
{
    int sfd, retval;
    struct sockaddr_un saddr;

    if ( -1 == check_error(sfd = socket(AF_UNIX,SOCK_DGRAM|flags,0)) )
	return -1;

    memset(&saddr,0,sizeof(struct sockaddr_un));

    if ( bind_path != NULL )
    {
	if ( (retval = unlink(bind_path)) == -1 && errno != ENOENT ) // If there's another error than "doesn't exist"
	{
	    check_error(retval);
	    return -1;
	}

	if ( strlen(bind_path) > (sizeof(saddr.sun_path)-1) )
	{
# ifdef VERBOSE
	    debug_write("UNIX socket path too long\n",26);
# endif
	    return -1;
	}

	saddr.sun_family = AF_UNIX;
	strncpy(saddr.sun_path,bind_path,sizeof(saddr.sun_path));

	bind(sfd,(struct sockaddr*)&saddr,sizeof(struct sockaddr));
    }

    return sfd;
}

// Reconnect a datagram UNIX domain socket - works only for DGRAM sockets!
//		      Socket   New path
int connect_unix_dgram_socket(int sfd, const char* path)
{
    struct sockaddr_un new_addr;
    struct sockaddr deconnect;

    if ( sfd < 0 )
	return -1;

    if ( path == NULL )
    {
	memset(&deconnect,0,sizeof(struct sockaddr));

	deconnect.sa_family = AF_UNSPEC;

	if ( check_error(connect(sfd,&deconnect,sizeof(struct sockaddr))) )
	    return -1;

	return 0;
    }

    memset(&new_addr,0,sizeof(struct sockaddr_un));

    new_addr.sun_family = AF_UNIX;

    if ( strlen(path) > sizeof(new_addr.sun_path)-1 )
    {
# ifdef VERBOSE
	debug_write("Path too long\n",14);
# endif
	return -1;
    }

    strncpy(new_addr.sun_path,path,sizeof(new_addr.sun_path)-1);

    if ( -1 == check_error(connect(sfd,(struct sockaddr*)&new_addr,sizeof(struct sockaddr_un))) )
	return -1;

    return 0;
}

// Destroy a socket
//		   Socket file descriptor
int destroy_unix_socket(int sfd)
{
    if ( sfd < 0 )
	return -1;

    if ( -1 == check_error(close(sfd)))
	return -1;

    return 0;
}

int shutdown_unix_stream_socket(int sfd, int method)
{
    if ( sfd < 0 )
	return -1;

    if ( (method != LIBSOCKET_READ) && (method != LIBSOCKET_WRITE) && (method != (LIBSOCKET_READ|LIBSOCKET_WRITE)) )
	return -1;

    if ( method & LIBSOCKET_READ ) // READ is set (0001 && 0001 => 0001)
    {
	if ( -1 == check_error(shutdown(sfd,SHUT_RD)))
	    return -1;

    }

    if ( method & LIBSOCKET_WRITE ) // WRITE is set (0010 && 0010 => 0010)
    {
	if ( -1 == check_error(shutdown(sfd,SHUT_WR)))
	    return -1;
    }

    return 0;
}

// Create new UNIX domain server socket
//			      Bind address DGRAM or STREAM
int create_unix_server_socket(const char* path, int socktype, int flags)
{
    struct sockaddr_un saddr;
    int sfd, type, retval;

    if ( path == NULL )
	return -1;

    if ( strlen(path) > (sizeof(saddr.sun_path)-1) )
    {
# ifdef VERBOSE
	debug_write("Path too long\n",14);
# endif
	return -1;
    }

    switch ( socktype )
    {
	case LIBSOCKET_STREAM:
	    type = SOCK_STREAM;
	    break;
	case LIBSOCKET_DGRAM:
	    type = SOCK_DGRAM;
	    break;
	default:
	    return -1;
    }

    if ( -1 == check_error(sfd = socket(AF_UNIX,type|flags,0)) )
	return -1;

    if ( (retval = unlink(path)) == -1 && errno != ENOENT ) // If there's another error than "doesn't exist"
    {
	check_error(retval);
	return -1;
    }

    memset(&saddr,0,sizeof(struct sockaddr_un));

    saddr.sun_family = AF_UNIX;

    strncpy(saddr.sun_path,path,sizeof(saddr.sun_path) - 1);

    if ( -1 == check_error(bind(sfd,(struct sockaddr*)&saddr,sizeof(struct sockaddr_un))) )
	return -1;

    if ( type == SOCK_STREAM )
    {
	if ( -1 == check_error(listen(sfd,LIBSOCKET_BACKLOG)))
	    return -1;
    }

    return sfd;
}

// Accept connections
int accept_unix_stream_socket(int sfd, int flags)
{
    int cfd;

    if ( sfd < 0 )
	return -1;
# ifdef __linux__
    if ( -1 == check_error(cfd = accept4(sfd,0,0,flags)) )
	return -1;
# else
    if ( -1 == check_error(cfd = accept(sfd,0,0)) )
	return -1;
# endif
    return cfd;
}

// Receives data
ssize_t recvfrom_unix_dgram_socket(int sfd, void* buf, size_t size, char* from, size_t from_size, int recvfrom_flags)
{
    int bytes;
    socklen_t socksize = sizeof(struct sockaddr_un);
    struct sockaddr_un saddr;

    memset(buf,0,size);
    memset(from,0,from_size);

    if ( -1 == check_error(bytes = recvfrom(sfd,buf,size,recvfrom_flags,(struct sockaddr*)&saddr,&socksize)) )
	return -1;

    if ( from != NULL && from_size > 0 )
	strncpy(from,saddr.sun_path,from_size);

    return bytes;
}

// Sends data
ssize_t sendto_unix_dgram_socket(int sfd, const void* buf, size_t size, const char* path, int sendto_flags)
{
    int bytes;
    struct sockaddr_un saddr;

    if ( strlen(path) > sizeof(saddr.sun_path)-1 )
    {
# ifdef VERBOSE
	debug_write("UNIX destination socket path too long\n",14);
# endif
	return -1;
    }

    memset(&saddr,0,sizeof(struct sockaddr_un));

    saddr.sun_family = AF_UNIX;
    strncpy(saddr.sun_path,path,sizeof(saddr.sun_path)-1);

    if ( -1 == check_error(bytes = sendto(sfd,buf,size,sendto_flags,(struct sockaddr*)&saddr,sizeof(struct sockaddr_un))) )
	return -1;

    return bytes;
}

/**
 * @}
 */

#undef debug_write
