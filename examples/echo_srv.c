# include <stdio.h>
# include <stdlib.h>
# include "../headers/libinetsocket.h"
# include <unistd.h>
# include <string.h>

/*
 * This example is a little echo server processing only one request. It may be used with telnet.
 * If it receives a string, it writes the string back to the client and to stdout.
 *
*/

int main(void)
{
	int sfd, cfd, bytes;
	char src_host[128], src_service[10], buf[16];

	src_host[127] = 0;
	src_service[9] = 0;
	buf[15] = 0;

	sfd = create_issocket("0.0.0.0","1234",TCP,IPv4);

	if ( -1 == sfd )
	{
		printf("couldn't create server\n");
		exit(1);
	}

	cfd = accept_issocket(sfd,src_host,127,src_service,9,NUMERIC);

	printf("Connection from %s port %s\n",src_host,src_service);
	
	while ( 0 < ( bytes = read(cfd,buf,15) ) )
	{
		write(cfd,buf,bytes);
		write(1,buf,bytes);
	}

	destroy_isocket(sfd);

	return 0;
}
