# include <iostream>
# include <string>
# include "../headers/inetclientstream.hpp"
# include "../headers/exception.hpp"

// HTTP client demonstrating the use of snd and rcv on STREAM sockets

int main(void)
{
	using std::string;

	string host = "176.28.21.82";
	string port = "80";
	ssize_t len;
	string test;

	char* buf = new char[10000];

	try {
		libsocket::inet_stream sock(host.c_str(),port.c_str(),IPv4);

		sock.snd("GET / HTTP/1.0\n\n",16);

		sock.shutdown(WRITE);

		while ( 0 != (len=sock.rcv(buf,10000)) )
			std::cout << string(buf,len); // Write only as many characters as we read
	} catch (libsocket::socket_exception exc)
	{
		std::cerr << exc.mesg;
	}

	return 0;
}
