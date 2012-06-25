# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include "../headers/libinetsocket.h"

int main(void)
{
	int cfd, sfd = create_inet_server_socket("::","55555",TCP,IPv6,0);
	char* buf = calloc(16,1);

	cfd = accept_inet_stream_socket(sfd,0,0,0,0,0,0);

	read(cfd,buf,15);

	printf("%s\n",buf);

	destroy_inet_socket(sfd);

	return 0;
}
