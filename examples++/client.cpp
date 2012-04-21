# include <iostream>
# include <string>
# include "../headers++/inetstreamclient.hpp"

int main(void)
{
	using std::string;

	using libsocket::inet_stream;

	string host = "::1";
	string port = "1235";
	char answer[7];

	answer[6] = 0;

	try {
		libsocket::inet_stream sock(host.c_str(),port.c_str(),IPv6);

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
