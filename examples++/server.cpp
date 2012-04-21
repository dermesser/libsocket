# include <iostream>
# include <string>
# include "../headers++/inetserversocket.hpp"
# include <unistd.h>
# include <stdio.h>
# include <errno.h>

int main(void)
{
	using std::string;

	using libsocket::inet_stream_server;
	using libsocket::inet_stream;

	string host = "::1";
	string port = "1235";
	
	try {
		inet_stream_server srv(host.c_str(),port.c_str(),IPv6);
		inet_stream* cl1;
		
		cl1 = srv.accept();
		
		*cl1 << "Hello";

		cl1->destroy();

		srv.destroy();

	} catch (libsocket::inet_exception exc)
	{
		std::cout << exc.mesg << std::endl;
	}
	return 0;
}
