# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include "../headers/libinetsocket.h"

/*
 * Accept one (1) connection from a transmission_client
 * and print the received message.
 */

int main(void)
{
    int cfd, sfd, ret;
    char* buf = calloc(16,1);

    ret = sfd = create_inet_server_socket("::","55555",LIBSOCKET_TCP,LIBSOCKET_IPv6,0);

    if ( ret < 0 )
    {
	perror(0);
	exit(1);
    }

    ret = cfd = accept_inet_stream_socket(sfd,0,0,0,0,0,0);

    if ( ret < 0 )
    {
	perror(0);
	exit(1);
    }

    ret = read(cfd,buf,15);

    if ( ret < 0 )
    {
	perror(0);
	exit(1);
    }

    printf("%s\n",buf);

    ret = destroy_inet_socket(sfd);

    if ( ret < 0 )
    {
	perror(0);
	exit(1);
    }

    return 0;
}
