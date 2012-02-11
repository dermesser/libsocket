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
	const char* string = "abcdefghijklmnopqrstuvwxyz";

	if ( -1 == (sfd = create_usocket("/tmp/echosock",DGRAM)) )
		return -1;

	write(sfd,string,26);

	destroy_usocket(sfd);

	return 0;
}
