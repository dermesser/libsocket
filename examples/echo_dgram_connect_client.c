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
	int sfd;
	char buf[16];

	memset(buf,0,16);

	sfd = create_inet_dgram_socket(IPv4,0);

	connect_inet_dgram_socket(sfd,"localhost","1234");

	write(sfd,"abcde",5);

	sleep(2);

	//read(sfd,buf,5);
	// We may use the recvfrom/sendto routines also on connected sockets
	if ( -1 == recvfrom_inet_dgram_socket(sfd,buf,16,0,0,0,0,0,0))
		printf("Error when recvfrom()\n");

	write(1,buf,5);

	destroy_inet_socket(sfd);

	return 0;
}
