# include <stdio.h>
# include <stdlib.h>
# include "../headers/libinetsocket.h"
# include <unistd.h>
# include <string.h>

int main(void)
{
	int sfd, bytes;
	char request[128], buf[32];

	buf[31] = 0;

	sfd = create_isocket("lbo.spheniscida.de", "80", TCP, IPv4,0);

	sprintf(request,"GET / HTTP/1.1\nHost: lbo.spheniscida.de\n\n");

	write(sfd,request,strlen(request));

	shutdown_isocket(sfd,WRITE);

	while ( 0 < (bytes = read(sfd,buf,31)) )
		write(1,buf,bytes);

	destroy_isocket(sfd);

	return 0;
}

