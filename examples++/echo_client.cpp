# include <iostream>
# include <unistd.h>
# include "../headers++/inetclientdgram.hpp"
# include "../headers++/exception.hpp"

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
	} catch (libsocket::socket_exception exc)
	{
		std::cerr << exc.mesg;
	}

	try {
		for ( int i = 0; i < 20; i++ )
		{
			/*sock.sndto("abcdefghi",9,host.c_str(),port.c_str());

			sock.rcvfrom(buf,9,peer_h,peer_p);
			*/

			/*
			 * Some other possibilities...

			sock.connect(host.c_str(),port.c_str());

			sock.snd("abcdefghi",9);

			sock << "abcdefghi";
			*/
			sock.rcv(buf,9);
			peer_h = host;
			peer_p = port;

			std::cout << peer_h << ":" << peer_p << " " << buf << std::endl;

			std::cout.flush();
		}
	} catch ( libsocket::socket_exception exc )
	{
		std::cerr << exc.mesg;
	}

	sock.destroy();

	return 0;
}
