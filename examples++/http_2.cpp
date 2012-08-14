# include <iostream>
# include <string>
# include "../headers/inetclientstream.hpp"

int main(void)
{
	using std::string;

	string host = "176.28.21.82";
	string port = "80";
	ssize_t len;
	string test;

	char* buf = new char[10000];

	libsocket::inet_stream sock(host.c_str(),port.c_str(),IPv4);

	sock.snd("GET / HTTP/1.0\n\n",16);

	sock.shutdown(WRITE);

	while ( 0 != (len=sock.rcv(buf,10000)) )
		std::cout << string(buf,len); // Write only as many characters as we read

	return 0;
}
