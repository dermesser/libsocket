# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include "../headers/libinetsocket.h"

/*
 * Connect to a transmission_server.c instance
 * and send a message
 */

int main(void)
{
    int ret;
    int sfd;
    char* buf = "abcde";

    ret = sfd = create_inet_stream_socket("::1","55555",LIBSOCKET_IPv6,0);

    if ( ret < 0 )
    {
	perror(0);
	exit(1);
    }

    ret = write(sfd,buf,5);

    if ( ret < 0 )
    {
	perror(0);
	exit(1);
    }

    ret = destroy_inet_socket(sfd);

    if ( ret < 0 )
    {
	perror(0);
	exit(1);
    }

    return 0;
}
