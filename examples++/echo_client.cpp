# include <iostream>
# include "../headers++/inetsocket.hpp"

// To be used with examples/echo_dgram_server.c

int main(void)
{
	using std::string;

	string host = "localhost";
	string port = "1234";

	char buf[10];

	buf[9] = 0;

	libsocket::inet_dgram sock(IPv4);
	
	for ( int i = 0; i < 20; i++ )
	{
		sock.sndto("abcdefghi",9,host.c_str(),port.c_str());
		sock.rcvfrom(buf,9,0,0,0,0);
		std::cout << buf;
	}

	sock.destroy();

	return 0;
}
