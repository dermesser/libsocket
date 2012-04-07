# include <stdio.h>
# include <unistd.h>
# include "../headers/libunixsocket.h"

/*
 * This example is part of libsocket/libunixsocket
 * It may be used with the other part, unix_dgram_client.c
 * It receives up to 127 bytes from a client and prints it on stdout
 *
*/

int main(void)
{
	int sfd, bytes;
	char buf[128];

	buf[127] = 0;
	
	if ( -1 == (sfd = create_unix_server_socket("/tmp/echosock",DGRAM)) )
		return -1;

	while ( 0 < ( bytes = recvfrom_unix_dgram_socket(sfd,buf,127,0,0) ) ) // read() is equivalent to recv_ussocket()
	{
		write(1,buf,bytes);
	}

	destroy_unix_socket(sfd);

	return 0;
}
