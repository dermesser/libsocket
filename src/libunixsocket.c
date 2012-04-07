# define _GNU_SOURCE // accept4()
# include <sys/socket.h> 
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h> // read()/write()
# include <sys/un.h> // UNIX domain sockets
# include <stdint.h>
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

/*
 * Structure of the functions defined here:
 *
 * <Declarations>
 * <Checks on passed arguments>
 * <actual code>
 *
*/

# define VERBOSE

// Macro definitions

# define BACKLOG 128

# define STREAM 1
# define DGRAM  2

# define READ  1
# define WRITE 2

static inline signed int check_error(int return_value)
{
# ifdef VERBOSE
	const char* errbuf;
# endif
	if ( return_value < 0 )
	{
# ifdef VERBOSE
		errbuf = strerror(errno);
		write(2,errbuf,strlen(errbuf));
# endif
		return -1;
	}

	return 0;
}

// Create new unix socket
int create_usocket(const char* path, int socktype, const char* bind_path)
{
	struct sockaddr_un saddr, baddr;
	int sfd;

	switch ( socktype )
	{
		case STREAM:
			sfd = socket(AF_UNIX,SOCK_STREAM,0);
			break;
		case DGRAM:
			sfd = socket(AF_UNIX,SOCK_DGRAM,0);
			break;
		default:
			return -1;
	}

	if ( -1 == check_error(sfd) )
		return -1;

	memset(&saddr,0,sizeof(struct sockaddr_un));
	memset(&baddr,0,sizeof(struct sockaddr_un));

	if ( strlen(path) > sizeof(saddr.sun_path)-1 )
	{
# ifdef VERBOSE
		write(2,"UNIX destination socket path too long\n",14);
# endif
		return -1;
	}

	if ( bind_path != 0 )
	{
		if ( strlen(bind_path) > sizeof(baddr.sun_path) )
		{
# ifdef VERBOSE
			write(2,"Bind path too long\n",14);
# endif
			return -1;
		}
		
		baddr.sun_family = AF_UNIX;
		strncpy(baddr.sun_path,bind_path,sizeof(saddr.sun_path)-1);
		
		if ( -1 == check_error(bind(sfd,(struct sockaddr*)&baddr,sizeof(struct sockaddr_un))) )
			return -1;
	}

	saddr.sun_family = AF_UNIX;
	strncpy(saddr.sun_path,path,sizeof(saddr.sun_path)-1);

	if ( -1 == check_error(connect(sfd,(struct sockaddr*)&saddr,sizeof saddr)) )
		return -1;

	return sfd;
}

// Reconnect a datagram UNIX domain socket - works only for DGRAM sockets!
//		      Socket   New path
int reconnect_usocket(int sfd, const char* path)
{
	struct sockaddr_un new_addr;

	memset(&new_addr,0,sizeof(struct sockaddr_un));

	new_addr.sun_family = AF_UNIX;
	
	if ( strlen(path) > sizeof(new_addr.sun_path)-1 )
	{
# ifdef VERBOSE
		write(2,"Path too long\n",14);
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
int destroy_usocket(int sfd)
{
	if ( -1 == check_error(close(sfd)))
		return -1;
	return 0;
}

int shutdown_usocket(int sfd, int method)
{
	if ( method & READ ) // READ is set (0001 && 0001 => 0001)
	{
		if ( -1 == check_error(shutdown(sfd,SHUT_RD)))
			return -1;

	} else if ( method & WRITE ) // WRITE is set (0010 && 0010 => 0010)
	{
		if ( -1 == check_error(shutdown(sfd,SHUT_WR)))
			return -1;
	}

	return 0;
}

// Create new UNIX domain server socket
//		    Bind address DGRAM or STREAM
int create_ussocket(char* path, int socktype)
{
	struct sockaddr_un saddr;
	int sfd, type, retval;

	if ( strlen(path) > sizeof(saddr.sun_path) )
	{
# ifdef VERBOSE
		write(2,"Path too long\n",14);
# endif
		return -1;
	}

	switch ( socktype )
	{
		case STREAM:
			type = SOCK_STREAM;
			break;
		case DGRAM:
			type = SOCK_DGRAM;
			break;
		default:
			return -1;
	}

	if ( -1 == check_error(sfd = socket(AF_UNIX,type,0)) )
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
		if ( -1 == check_error(listen(sfd,BACKLOG)))
			return -1;
	}

	// Our socket is up and running and ready for accepting connections

	return sfd;
}

// Accept connections
//		    Socket   Flags (SOCK_NONBLOCK, SOCK_CLOEXEC)
int accept_ussocket(int sfd, int flags)
{
	int cfd;

	if ( flags != SOCK_NONBLOCK && flags != SOCK_CLOEXEC && flags != (SOCK_CLOEXEC|SOCK_NONBLOCK) && flags != 0 )
		return -1;

	if ( -1 == check_error(cfd = accept4(sfd,0,0,flags)) )
		return -1;

	return cfd;
}

// Receives data
// 		      Socket   Buffer     its size
ssize_t recvfrom_ussocket(int sfd, void* buf, size_t size, char* from, size_t from_size)
{
	int bytes, socksize = sizeof(struct sockaddr_un);
	struct sockaddr_un saddr;

	if ( -1 == check_error(bytes = recvfrom(sfd,buf,size,0,(struct sockaddr*)&saddr,&socksize)) )
		return -1;
	
	if ( from != 0 && from_size > 0 )
		strncpy(from,saddr.sun_path,from_size);

	return bytes;
}
