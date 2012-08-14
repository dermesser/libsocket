# include <iostream>
# include <string>
# include "../headers/inetclientstream.hpp"
# include "../headers/exception.hpp"

int main(void)
{
	using std::string;

	using libsocket::inet_stream;

	string host = "::1";
	string port = "1235";
	string answer;

	answer.resize(32);

	try {
		libsocket::inet_stream sock(host,port,IPv6);

		sock >> answer;

		std::cout << answer;

		sock << "Hello back!\n";

		sock.destroy();
	} catch (libsocket::socket_exception exc)
	{
		std::cout << exc.mesg;
	}

	return 0;
}
