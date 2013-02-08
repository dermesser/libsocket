# include <stdio.h>
# include <stdlib.h>
# include "../headers/libinetsocket.h"
# include <unistd.h>
# include <string.h>

/*
 * This example is part of libsocket/libinetsocket
 * It may be used with the most HTTP servers.
 *
 * This is a almost complete HTTP client, simply
 * writing the data to stdout. There's no more code
 * with libsocket!
 */

int main(void)
{
    int sfd, bytes, ret;
    char request[128], buf[32];

    buf[31] = 0;

    ret = sfd = create_inet_stream_socket("lbo.spheniscida.de", "80", LIBSOCKET_IPv4,0);

    if ( ret < 0 )
    {
	perror(0);
	exit(1);
    }

    sprintf(request,"GET / HTTP/1.1\nHost: lbo.spheniscida.de\n\n");

    ret = write(sfd,request,strlen(request));

    if ( ret < 0 )
    {
	perror(0);
	exit(1);
    }

    ret = shutdown_inet_stream_socket(sfd,LIBSOCKET_WRITE);

    if ( ret < 0 )
    {
	perror(0);
	exit(1);
    }

    while ( 0 < (bytes = read(sfd,buf,31)) )
	write(1,buf,bytes);

    ret = destroy_inet_socket(sfd);

    if ( ret < 0 )
    {
	perror(0);
	exit(1);
    }

    return 0;
}

