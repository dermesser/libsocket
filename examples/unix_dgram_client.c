# include <string.h>
# include <stdio.h>
# include <unistd.h>
# include "../headers/libunixsocket.h"

/*
 * This example is part of libsocket/libunixsocket
 * It may be used with the other part, unix_dgram_server.c
 * It should send the content of string via a DGRAM UNIX socket
 *
*/

int main(void)
{
	int sfd;
	char* string = "abcdefghijklmnopqrstuvwxyz";

	if ( -1 == (sfd = create_unix_dgram_socket()) )
		return -1;

	sendto_unix_dgram_socket(sfd,string,26,"/tmp/echosock");

	destroy_unix_socket(sfd);

	return 0;
}
