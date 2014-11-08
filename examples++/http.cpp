# include <iostream>
# include <string>
# include "../headers/inetclientstream.hpp"
# include "../headers/exception.hpp"

// HTTP client demonstrating the use of stream operators
// on STREAM sockets

int main(void)
{
    using std::string;

    using libsocket::inet_stream;

    try {
	string host = "spheniscida.de";
	string port = "80";
	string answer;
	string request1("GET / HTTP/1.1\n");
	string request2("Host: spheniscida.de\n\n");

	libsocket::inet_stream sock(host,port,LIBSOCKET_IPv4,0);

	sock << request1 << request2;

	sock.shutdown(LIBSOCKET_WRITE);

	answer.resize(2000);

	while ( answer.size() > 0 )
	{
	    sock >> answer;

	    std::cout << answer;
	}

	//sock.destroy(); // socket is also destroyed by the destructor
    } catch (const libsocket::socket_exception& exc)
    {
	std::cerr << exc.mesg;
    }

    return 0;
}
