# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include "../headers/libunixsocket.h"

/*
 * This example is part of libsocket/libunixsocket
 * It may be used with the other part, unix_stream_server.c
 * This example simply sends a string to the echo server
 * and tries to receive the answer and print it.
 */

int main(void)
{
    int sfd, bytes = 1, ret;
    const char* string = "abcdefghijklmnopqrstuvwxyz";
    char buf[16];

    memset(buf,0,sizeof(buf));

    ret = sfd = create_unix_stream_socket("/tmp/echosock",0);

    if ( ret < 0 )
    {
	perror(0);
	exit(1);
    }

    ret = write(sfd,string,26);

    if ( ret < 0 )
    {
	perror(0);
	exit(1);
    }

    ret = shutdown_unix_stream_socket(sfd,LIBSOCKET_WRITE); // Send EOF

    if ( ret < 0 )
    {
	perror(0);
	exit(1);
    }

    while ( bytes > 0 )
    {
	ret = bytes = read(sfd,buf,15);

	if ( ret < 0 )
	{
	    perror(0);
	    exit(1);
	}

	write(1,buf,bytes);
    }

    ret = destroy_unix_socket(sfd);

    if ( ret < 0 )
    {
	perror(0);
	exit(1);
    }

    return 0;
}
