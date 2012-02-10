# include <stdio.h>
# include <stdlib.h>
# include "../headers/libinetsocket.h"
# include <unistd.h>
# include <string.h>

/*
 * This client is to be used with echosrv (https://github.com/dermesser/echosrv)
 * It demonstrates the concept of reconnect
 *
*/

int main(void)
{
	int sfd, bytes;
	char request[128], buf[32];
	
	buf[31] = 0;

	sfd = create_isocket("localhost","55555",TCP,IPv4,0);

	write(sfd,"abcde",5);

	read(sfd,buf,5);

	write(1,buf,5);
	
	reconnect_isocket(sfd,"192.168.1.8","55555",TCP);

	write(sfd,"defghi",5);

	read(sfd,buf,5);

	write(1,buf,5);

	destroy_isocket(sfd);

	return 0;
}
