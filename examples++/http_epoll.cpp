# include <iostream>
# include <string>
# include "../headers/inetclientstream.hpp"
# include "../headers/exception.hpp"
# include "../headers/epoll.hpp"

// HTTP client demonstrating the use of stream operators
// on STREAM sockets, and additionally how to handle epollsets.

int main(void)
{
    using std::string;

    using libsocket::inet_stream;
    using libsocket::epollset;

    try {
	string host = "spheniscida.de";
	string port = "80";
	string answer;
	string request1("GET / HTTP/1.1\n");
	string request2("Host: spheniscida.de\n\n");

        epollset<inet_stream> eset;
	inet_stream sock(host,port,LIBSOCKET_IPv4,0);

	sock << request1 << request2;

	sock.shutdown(LIBSOCKET_WRITE);

        eset.add_fd(sock,LIBSOCKET_READ);

        epollset<inet_stream>::ready_socks ready = eset.wait(-1);

        if ( ready.first.size() > 0 )
            sock = *(ready.first[0]);
        else
            throw "No socket has been returned!";

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
