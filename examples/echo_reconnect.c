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
 * At the reconnect_isocket() call, 192.168.1.8 is another address on which the same
 * server listens.
*/

int main(void)
{
	int sfd;

	sfd = create_isocket("localhost","1234",UDP,IPv4,0); // Because we want to use reconnect_isocket() - 
	
	write(sfd,"abcde",5); // Each write() generates a new UDP packet
	
	sleep(2);

	if ( -1 == reconnect_isocket(sfd,"192.168.1.8","1234") ) // FIXME: Put your IP here...
		printf("couldn't reconnect\n");
	
	sleep(2);

	write(sfd,"defghi",6);

	destroy_isocket(sfd);

	return 0;
}
