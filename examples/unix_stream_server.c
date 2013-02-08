# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include "../headers/libunixsocket.h"

/*
 * This example is part of libsocket/libunixsocket
 * It may be used with the other part, unix_stream_client.c
 *
 * The server accepts a connection, reads 15 bytes and writes them
 * first back to the client and then to stdout.
 */

int main(void)
{
    int sfd, cfd, bytes, ret;
    char buf[16];

    buf[15] = 0;

    ret = sfd = create_unix_server_socket("/tmp/echosock",LIBSOCKET_STREAM,0);

    if ( ret < 0 )
    {
	perror(0);
	exit(1);
    }

    for ( ;; )
    {
	ret = cfd = accept_unix_stream_socket(sfd,0);

	if ( ret < 0 )
	{
	    perror(0);
	    exit(1);
	}

	while ( 0 < ( bytes = read(cfd,buf,15) ) )
	{
	    write(cfd,buf,bytes);
	    write(1,buf,bytes);
	}

	ret = destroy_unix_socket(cfd);

	if ( ret < 0 )
	{
	    perror(0);
	    exit(1);
	}

    }

    ret = destroy_unix_socket(sfd);

    if ( ret < 0 )
    {
	perror(0);
	exit(1);
    }

    return 0;
}
