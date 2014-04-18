# include <stdio.h>
# include <stdlib.h>
# include "../headers/libinetsocket.h"
# include <unistd.h>
# include <string.h>

/*
 * This program shows which address family is available
 * for a certain host
 *
 * Usage: $ get_address_family hostname.domain
 *
 */

int main(int argc, char** argv)
{

    int af = get_address_family(argv[1]);

    switch ( af )
    {
	case LIBSOCKET_IPv4:
	    printf("Supports IPv4\n");
	    break;
	case LIBSOCKET_IPv6:
	    printf("Supports IPv6\n");
	    break;
	default:
	    printf("No valid result\n");
    }

    return 0;
}

