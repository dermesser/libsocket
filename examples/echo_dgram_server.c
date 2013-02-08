# include <stdio.h>
# include <stdlib.h>
# include "../headers/libinetsocket.h"
# include <unistd.h>
# include <string.h>

/*
 * This example is part of libsocket/libinetsocket
 *
 */

/*
 * This example is a little echo server processing infinite requests.
 * If it receives a string, it writes the string back to the client and to stdout.
 * This server uses UDP and listens on 0.0.0.0:1234. It is used with echo_dgram_client.c
 * or echo_dgram_connect_client.c.
 */

int main(void)
{
    int sfd, bytes, ret;
    char src_host[128], src_service[7], buf[16];

    src_host[127] = 0;
    src_service[6] = 0;

    sfd = create_inet_server_socket("0.0.0.0","1234",LIBSOCKET_UDP,LIBSOCKET_IPv4,0);

    if ( -1 == sfd )
    {
	perror("couldn't create server\n");
	exit(1);
    }

    printf("Socket up and running\n");

    while ( 1 )
    {
	memset(buf,0,16);
	ret = bytes = recvfrom_inet_dgram_socket(sfd,buf,15,src_host,127,src_service,6,0,LIBSOCKET_NUMERIC);

	if ( ret < 0 )
	{
	    perror(0);
	    exit(1);
	}

	ret = sendto_inet_dgram_socket(sfd,buf,bytes,src_host,src_service,0);

	if ( ret < 0 )
	{
	    perror(0);
	    exit(1);
	}

	printf("Connection from %s port %s: %s (%i)\n",src_host,src_service,buf,bytes);
	printf("Connection processed\n");
    }

    ret = destroy_inet_socket(sfd);

    if ( ret < 0 )
    {
	perror(0);
	exit(1);
    }

    return 0;
}
