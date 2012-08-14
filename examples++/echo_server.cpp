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

	char buf[32];

	string from;
	string fromport;

	try {
		libsocket::inet_dgram_server srv(host.c_str(),port.c_str(),BOTH);
		for (;;)
		{
			memset(buf,0,32);

			srv.rcvfrom(buf,31,from,fromport);

			std::cout << from << ":" << fromport << " " << buf << std::endl;

			srv.sndto(buf,strlen(buf),from,fromport);
		}

		srv.destroy();
	} catch (libsocket::socket_exception exc)
	{
		std::cout << exc.mesg;
	}


	return 0;
}
