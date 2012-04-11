# include <iostream>
# include "inetsocket.hpp"

int main(void)
{
	using std::string;

	string host = "176.28.21.82";
	string port = "80";
	string answer;
	string request1("GET / HTTP/1.1\n");
	string request2("Host: spheniscida.de\n\n");

	libsocket::inet_stream sock(host.c_str(),port.c_str(),IPv4,0);
	
	sock << request1 << request2;

	sock.shutdown(WRITE);
	
	answer.resize(2000);

	while ( answer.size() > 0 )
	{
		sock >> answer;

		std::cout << answer;
	}

	sock.destroy();

	return 0;
}
