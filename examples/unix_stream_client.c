# include <string.h>
# include <stdio.h>
# include <unistd.h>
# include "../headers/libunixsocket.h"

/*
 * This example is part of libsocket/libunixsocket
 * It may be used with the other part, unix_stream_client.c
 *
*/

int main(void)
{
	int sfd, bytes = 1;
	const char* string = "abcdefghijklmnopqrstuvwxyz";
	char buf[16];

	memset(buf,0,sizeof(buf));

	if ( -1 == (sfd = create_usocket("/tmp/echosock",STREAM,"/tmp/abc")) )
		return -1;

	write(sfd,string,26);

	shutdown_usocket(sfd,WRITE); // Send EOF

	while ( bytes > 0 )
	{
		bytes = read(sfd,buf,15);
		write(1,buf,bytes);
	}

	destroy_usocket(sfd);

	return 0;
}
