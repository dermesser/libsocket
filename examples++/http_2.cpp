# include <iostream>
# include "../headers++/inetsocket.hpp"

int main(void)
{
	using std::string;

	string host = "176.28.21.82";
	string port = "80";
	ssize_t len;

	char* buf = new char[10000];

	libsocket::inet_stream sock(host.c_str(),port.c_str(),IPv4,0);
	
	sock.snd("GET / HTTP/1.0\n\n",16,0);

	sock.shutdown(WRITE);

	while ( 0 != (len=sock.rcv(buf,10000,0)) )
		std::cout << string(buf,len); // Write only as many characters as we read

	return 0;
}
