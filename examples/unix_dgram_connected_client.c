# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include "../headers/libunixsocket.h"

/*
 * This example is part of libsocket/libunixsocket
 * It may be used with the other part, unix_dgram_server.c
 * It should send the content of string via a connected DGRAM UNIX socket
 *
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

    ret = connect_unix_dgram_socket(sfd,"/tmp/echosock");

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

    ret = read(sfd,buf,26);

    if ( ret < 0 )
    {
	perror(0);
	exit(1);
    }

    write(1,buf,26);

    ret = destroy_unix_socket(sfd);

    if ( ret < 0 )
    {
	perror(0);
	exit(1);
    }

    return 0;
}
