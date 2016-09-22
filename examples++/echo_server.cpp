# include <iostream>
# include "../headers/inetserverdgram.hpp"
# include "../headers/exception.hpp"
# include <cstring>

// Server for echo_client_*.cpp
// simply receives a message and sends an answer.

int main(void)
{
    using std::string;

    string host = "localhost";
    string port = "1234";

    string answer("Hello back from the server!");
    string from;
    string fromport;
    string buf;

    buf.resize(32);

    try {
	libsocket::inet_dgram_server srv(host,port,LIBSOCKET_BOTH);

	for (;;)
	{
	    srv.rcvfrom(buf,from,fromport);

	    std::cout << "Datagram from " << from << ":" << fromport << " " << buf << std::endl;

	    srv.sndto(answer,from,fromport);
	}

	srv.destroy();
    } catch (const libsocket::socket_exception& exc)
    {
	std::cerr << exc.mesg;
    }


    return 0;
}
