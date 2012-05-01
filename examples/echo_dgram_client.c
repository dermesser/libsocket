# include <stdio.h>
# include <stdlib.h>
# include "../headers/libinetsocket.h"
# include <unistd.h>
# include <string.h>

/*
 * This example is part of libsocket/libinetsocket
 * 
*/

int main(void)
{
	int sfd;
	char buf[16];

	sfd = create_inet_dgram_socket(IPv4,0);

	sendto_inet_dgram_socket(sfd,"abcde",5,"localhost","1234",0); 

	//sleep(2);

	recvfrom_inet_dgram_socket(sfd,buf,5,0,0,0,0,0,NUMERIC);

	write(1,buf,5);

	destroy_inet_socket(sfd);

	return 0;
}
