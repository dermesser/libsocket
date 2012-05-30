# include <iostream>
# include <unistd.h>
# include "../headers++/inetclientdgram.hpp"

int main(void)
{
	using std::string;

	string host = "localhost";
	string port = "1234";

	string peer_h;
	string peer_p;

	char buf[10];

	buf[9] = 0;

	libsocket::inet_dgram_client sock(IPv4);

	try {
		std::cout << sock.gethost();
	} catch (libsocket::inet_exception exc)
	{
		std::cerr << exc.mesg;
	}

	for ( int i = 0; i < 20; i++ )
	{
		sock.sndto("abcdefghi",9,host.c_str(),port.c_str());

		sock.rcvfrom(buf,9,peer_h,peer_p);

		std::cout << peer_h << ":" << peer_p << " " << buf << std::endl;

		std::cout.flush();
	}

	sock.destroy();

	return 0;
}
