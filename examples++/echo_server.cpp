# include <iostream>
# include "../headers/inetserverdgram.hpp"
# include "../headers/exception.hpp"
# include <cstring>

// To be used with examples/echo_dgram_server.c

int main(void)
{
	using std::string;

	string host = "localhost";
	string port = "1234";

	string from;
	string fromport;
	string buf;

	buf.resize(32);

	try {
		libsocket::inet_dgram_server srv(host,port,BOTH);
		for (;;)
		{
			srv.rcvfrom(buf,from,fromport);

			std::cout << "Datagram from " << from << ":" << fromport << " " << buf << std::endl;

			srv.sndto("Hello back from the server!",27,from,fromport);
		}

		srv.destroy();
	} catch (libsocket::socket_exception exc)
	{
		std::cout << exc.mesg;
	}


	return 0;
}
