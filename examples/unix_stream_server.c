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
	int sfd, cfd, bytes;
	char buf[16];

	buf[15] = 0;

	if ( -1 == (sfd = create_ussocket("/tmp/echosock",STREAM)) )
		return -1;

	for ( ;; )
	{
		cfd = accept_ussocket(sfd,0);

		if ( cfd == -1 )
			return -1;

		while ( 0 < ( bytes = read(cfd,buf,15) ) )
		{
			write(cfd,buf,bytes);
			write(1,buf,bytes);
		}

		//shutdown_usocket(cfd,WRITE);
		destroy_usocket(cfd);
	}

	destroy_usocket(sfd);

	return 0;
}
