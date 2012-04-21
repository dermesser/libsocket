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
 * This example is a little echo server processing only one request. It may be used with telnet.
 * If it receives a string, it writes the string back to the client and to stdout.
 *
*/

int main(void)
{
	int sfd, bytes;
	char src_host[128], src_service[7], buf[16];

	src_host[127] = 0;
	src_service[6] = 0;

	sfd = create_inet_server_socket("0.0.0.0","1234",UDP,IPv4,0);

	if ( -1 == sfd )
	{
		printf("couldn't create server\n");
		exit(1);
	}

	printf("Socket up and running\n");

	while ( 1 )
	{
		memset(buf,0,16);
		bytes = recvfrom_inet_dgram_socket(sfd,buf,15,src_host,127,src_service,6,0,NUMERIC);

		sendto_inet_dgram_socket(sfd,buf,bytes,src_host,src_service,0);

		printf("Connection from %s port %s: %s (%i)\n",src_host,src_service,buf,bytes);
		printf("Connection processed\n");
	}

	destroy_inet_socket(sfd);

	return 0;
}
