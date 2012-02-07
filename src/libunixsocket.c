# include <stdlib.h>
# include <sys/socket.h> 
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

# define VERBATIM

// Macro definitions

# define STREAM 1
# define DGRAM  2


static inline signed int check_error(int return_value)
{
	const char* errbuf;

	if ( return_value < 0 )
	{
# ifdef VERBATIM
		errbuf = strerror(errno);
		write(2,errbuf,strlen(errbuf));
#endif
		return -1;
	}

	return 0;
}

// Create new unix socket
int create_usocket(const char* path, int socktype)
{
	struct sockaddr_un saddr;
	int sfd;

	switch ( socktype )
	{
		case STREAM:
			sfd = socket(AF_UNIX,SOCK_STREAM,0);
			break;
		case DGRAM:
			sfd = socket(AF_UNIX,SOCK_DGRAM,0);
	}

	if ( -1 == check_error(sfd) )
		return -1;

	memset(&saddr,0,sizeof(struct sockaddr_un));

	saddr.sun_family = AF_UNIX;
	strncpy(saddr.sun_path,path,sizeof(saddr.sun_path-1));
	
	
	if ( -1 == check_error(connect(sfd,(struct sockaddr*)&saddr,sizeof saddr)))
		return -1;

	return sfd;
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
	if ( (method & 1) == 1 ) // READ is set (0001 && 0001 => 0001)
	{
		if ( -1 == check_error(shutdown(sfd,SHUT_RD)))
			return -1;

	} else if ( (method & 2) == 2 ) // WRITE is set (0010 && 0010 => 0010)
	{
		if ( -1 == check_error(shutdown(sfd,SHUT_WR)))
			return -1;

	} else if ( (method & 3) == 3 ) // READ | WRITE is set (0011 && 0011 => 0011)
	{
		if ( -1 == check_error(shutdown(sfd,SHUT_RDWR)))
			return -1;
	}
	return 0;
}
