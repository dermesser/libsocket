# include <iostream>
# include "../headers++/inetserversocket.hpp"

// To be used with examples/echo_dgram_server.c

int main(void)
{
	using std::string;

	string host = "localhost";
	string port = "1234";

	char buf[10];
	char from[128];
	char fromport[16];

	buf[9] = 0;
	from[127] = 0;
	fromport[15] = 0;

	libsocket::inet_dgram_server srv(host.c_str(),port.c_str(),IPv4);

	for (;;)
	{
		srv.rcvfrom(buf,9,from,127,fromport,15);
		
		std::cout << buf << std::endl;

		srv.sndto(buf,9,from,fromport);
	}

	srv.destroy();

	return 0;
}
