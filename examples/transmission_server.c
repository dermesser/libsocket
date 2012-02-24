# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include "headers/libinetsocket.h"

int main(void)
{
	int cfd, sfd = create_issocket("::","55555",TCP,IPv6);
	char* buf = calloc(16,1);

	cfd = accept_issocket(sfd,0,0,0,0,0);

	read(cfd,buf,15);

	printf("%s\n",buf);

	destroy_isocket(sfd);

	return 0;
}
