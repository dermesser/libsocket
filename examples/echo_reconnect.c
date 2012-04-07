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
 * This example is to be used with echo_srv.c as server
*/

int main(void)
{
	int sfd;
	char buf[16];

	sfd = create_inet_dgram_socket(IPv4,0);
	
	sendto_inet_dgram_socket(sfd,"abcde",5,"localhost","1234"); 
	
	sleep(2);
/*
	if ( -1 == reconnect_isocket(sfd,"192.168.1.8","1234") ) // FIXME: Put your IP here...
		printf("couldn't reconnect\n");
	
	sleep(2);

	write(sfd,"defghi",6);
*/
	recvfrom_inet_dgram_socket(sfd,buf,5,0,0,0,0,NUMERIC);
	
	write(1,buf,5);

	destroy_inet_socket(sfd);

	return 0;
}
