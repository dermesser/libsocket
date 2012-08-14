# include <iostream>
# include <unistd.h>
# include "../headers/inetclientdgram.hpp"
# include "../headers/exception.hpp"
# include <cstring>

int main(void)
{
	using std::string;

	string host = "localhost";
	string port = "1234";

	string from1;
	string from2;

	from1.resize(64);
	from2.resize(64);

	string text = "Hello, Server!";
	string buf;

	buf.resize(32);

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
			sock.sndto(text,host,port);

			sock.rcvfrom(buf,from1,from2);

			std::cout << "Answer from " << from1 << ":" << from2 << " - " << buf << std::endl;

			std::cout.flush();

			sock.deconnect();
		}
	} catch ( libsocket::socket_exception exc )
	{
		std::cerr << exc.mesg;
	}

	sock.destroy();

	return 0;
}
