# include <stdio.h>
# include <stdlib.h>
# include "../headers/libinetsocket.h"
# include <unistd.h>
# include <string.h>

/*
 * This example is part of libsocket/libinetsocket
 * It works similar to echo_dgram_client.c, but uses
 * connected datagram sockets.
 *
 */

int main(void)
{
    int sfd, ret;
    char buf[16];

    memset(buf,0,16);

    ret = sfd = create_inet_dgram_socket(LIBSOCKET_IPv4,0);

    if ( ret < 0 )
    {
	perror(0);
	exit(1);
    }

    ret = connect_inet_dgram_socket(sfd,"localhost","1234");

    if ( ret < 0 )
    {
	perror(0);
	exit(1);
    }

    ret = write(sfd,"abcde",5);

    if ( ret < 0 )
    {
	perror(0);
	exit(1);
    }

    sleep(2);

    //read(sfd,buf,5);
    // We may use the recvfrom/sendto routines also on connected sockets
    ret = recvfrom_inet_dgram_socket(sfd,buf,16,0,0,0,0,0,0);

    if ( ret < 0 )
    {
	perror(0);
	exit(1);
    }

    ret = write(1,buf,5);

    if ( ret < 0 )
    {
	perror(0);
	exit(1);
    }

    ret = destroy_inet_socket(sfd);

    if ( ret < 0 )
    {
	perror(0);
	exit(1);
    }

    return 0;
}
