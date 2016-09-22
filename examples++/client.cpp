# include <iostream>
# include <string>
# include "../headers/inetclientstream.hpp"
# include "../headers/exception.hpp"

# include <unistd.h>
# include <stdlib.h>

/*
 * This program connects to host:port (usually localhost),
 * closes the socket, reopens it with the same parameters
 * (to show what you may do with sockets... ;), receives
 * a message from the server and sends a message back.
 */

int main(void)
{
    using std::string;

    using libsocket::inet_stream;

    string host = "::1";
    string port = "1235";
    string answer;

    answer.resize(32);

    try {
	libsocket::inet_stream sock(host,port,LIBSOCKET_IPv6);

	sock >> answer;

	std::cout << answer;

	sock << "Hello back!\n";

        // sock is closed here automatically!
    } catch (const libsocket::socket_exception& exc)
    {
	std::cerr << exc.mesg;
    }

    return 0;
}
