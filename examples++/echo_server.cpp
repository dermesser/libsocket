# include <iostream>
# include "../headers++/inetserverdgram.hpp"

// To be used with examples/echo_dgram_server.c

int main(void)
{
	using std::string;

	string host = "localhost";
	string port = "1234";

	char buf[10];

	string from;
	string fromport;

	buf[9] = 0;

	try {
		libsocket::inet_dgram_server srv(host.c_str(),port.c_str(),BOTH);
		for (;;)
		{
			srv.rcvfrom(buf,9,from,fromport);

			std::cout << from << ":" << fromport << " " << buf << std::endl;

			srv.sndto(buf,9,from,fromport);
		}

		srv.destroy();
	} catch (libsocket::inet_exception exc)
	{
		std::cout << exc.mesg;
	}


	return 0;
}
