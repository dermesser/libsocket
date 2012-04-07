# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include "../headers/libinetsocket.h"

int main(void)
{
	int sfd = create_inet_stream_socket("2001:554:2323:aabc::22ed","55555",IPv6,0);
	char* buf = "abcde";

	write(sfd,buf,5);

	destroy_inet_socket(sfd);

	return 0;
}
