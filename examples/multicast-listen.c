# include <stdlib.h>
# include <stdio.h>
# include "../headers/libinetsocket.h"
# include <netinet/in.h>
# include <sys/socket.h>
# include <unistd.h>
# include <string.h>

int main(void)
{

    int c = 0;
    char* buffer = malloc(8192);

    //int sfd = create_multicast_socket("225.1.2.3","1230","eth0");
    int sfd = create_multicast_socket("239.255.255.250","1900","eth0"); // This group/port is used by UPnP devices. Maybe you see something?
    //int sfd = create_multicast_socket("ff00::abcd","1900","eth0");

    ssize_t received = 0;

    if ( 0 > setsockopt(sfd,IPPROTO_IP,IP_MULTICAST_LOOP,&c,4) )
    {
        fprintf(stderr,"setsockopt failed.");
        free(buffer);
        exit(1);
    }

    while ( 0 <= (received = read(sfd,buffer,8191)) )
    {
        write(0,buffer,received);
        sendto_inet_dgram_socket(sfd,"Hi back",7,"239.255.255.250","1900",0);
    }

    free(buffer);

    return 0;
}
