# include <iostream>
# include "../headers++/inetsocket.hpp"

// Send nonsense datagrams to an ntp server

int main(void)
{
	using std::string;

	string host = "176.28.21.82";
	string port = "123";

	libsocket::inet_dgram sock(IPv4);
	
	for ( int i = 0; i < 20; i++ )
		sock.sndto("abc",3,host.c_str(),port.c_str());

	sock.destroy();

	return 0;
}
