# include <iostream>
# include "inetsocket.hpp"

int main(void)
{
	using std::string;

	string host = "176.28.21.82";
	string port = "80";
	string answer;

	libsocket::inet_stream sock(host.c_str(),port.c_str(),IPv4,0);
	
	sock << "GET / HTTP/1.1\nHost: spheniscida.de\n\n";

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
