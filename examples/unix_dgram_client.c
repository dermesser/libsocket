# include <string.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include "../headers/libunixsocket.h"

/*
 * This example is part of libsocket/libunixsocket
 * It may be used with the other part, unix_dgram_server.c
 * It should send the content of the string via a DGRAM UNIX socket
 * to a unix_dgram_server listening at /tmp/echosock.
 */

int main(void)
{
    int sfd, ret;
    char* string = "abcdefghijklmnopqrstuvwxyz";
    char buf[26];

    ret = sfd = create_unix_dgram_socket("/tmp/client",0);

    if ( ret < 0 )
    {
	perror(0);
	exit(1);
    }

    ret = sendto_unix_dgram_socket(sfd,string,26,"/tmp/echosock",0);

    if ( ret < 0 )
    {
	perror(0);
	exit(1);
    }

    ret = recvfrom_unix_dgram_socket(sfd,buf,26,0,0,0);

    if ( ret < 0 )
    {
	perror(0);
	exit(1);
    }

    ret = write(1,buf,26);

    if ( ret < 0 )
    {
	perror(0);
	exit(1);
    }

    ret = destroy_unix_socket(sfd);

    if ( ret < 0 )
    {
	perror(0);
	exit(1);
    }

    return 0;
}
