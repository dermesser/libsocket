# include <iostream>
# include <string>
# include "../headers++/inetclientstream.hpp"

int main(void)
{
	using std::string;

	using libsocket::inet_stream;

	string host = "::1";
	string port = "1235";
	char answer[7];

	answer[6] = 0;

	try {
# if __cplusplus == 201103L
		libsocket::inet_stream sock(host,port,IPv6);
# else
		libsocket::inet_stream sock(host.c_str(),port.c_str(),IPv6);
# endif
		sock.rcv(answer,6);

		std::cout << answer;

		sock << "Hello back!\n";

		sock.destroy();
	} catch (libsocket::inet_exception exc)
	{
		std::cout << exc.mesg;
	}

	return 0;
}
