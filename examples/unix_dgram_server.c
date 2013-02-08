# include <stdio.h>
# include <unistd.h>
# include "../headers/libunixsocket.h"
# include <string.h>
# include <stdlib.h>

/*
 * This example is part of libsocket/libunixsocket
 * It may be used with the other part, unix_dgram_client.c
 * It receives up to 127 bytes from a client and prints it on stdout.
 *
 */

int main(void)
{
    int sfd, bytes, ret;
    char buf[128];
    char from[128];

    memset(buf,0,128);
    memset(from,0,128);

    ret = sfd = create_unix_server_socket("/tmp/echosock",LIBSOCKET_DGRAM,0);

    if ( ret < 0 )
    {
	perror(0);
	exit(1);
    }

    while ( 0 < ( ret = bytes = recvfrom_unix_dgram_socket(sfd,buf,127,from,127,0) ) ) // read() is equivalent to recv_ussocket()
    {
	if ( ret < 0 )
	{
	    perror(0);
	    exit(1);
	}

	write(1,buf,bytes);
	ret = sendto_unix_dgram_socket(sfd,buf,bytes,from,0);

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
