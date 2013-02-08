# include <stdio.h>
# include <stdlib.h>
# include "../headers/libinetsocket.h"
# include <unistd.h>
# include <string.h>

/*
 * This example is part of libsocket/libinetsocket
 *
 * It sends a UDP packet containing the message 'abcde' to
 * localhost:1234 (usually an echo_dgram_server.c instance)
 *
 */

int main(void)
{
    int sfd;
    char buf[16];
    int ret;

    sfd = create_inet_dgram_socket(LIBSOCKET_IPv4,0);

    if ( sfd < 0 )
    {
	perror(0);
	exit(1);
    }

    ret = sendto_inet_dgram_socket(sfd,"abcde",5,"localhost","1234",0);

    if ( ret < 0 )
    {
	perror(0);
	exit(1);
    }

    //sleep(2);

    ret = recvfrom_inet_dgram_socket(sfd,buf,5,0,0,0,0,0,LIBSOCKET_NUMERIC);

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
