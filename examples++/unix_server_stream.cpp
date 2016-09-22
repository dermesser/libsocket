# include "../headers/unixserverstream.hpp"
# include "../headers/unixclientstream.hpp"
# include "../headers/exception.hpp"
# include <string>
# include <memory>
# include <iostream>
# include <string.h>

using std::unique_ptr;

// Accepts a connection, prints the received message and responds with another message.

int main(void)
{
    using std::string;
    using libsocket::unix_stream_server;
    using libsocket::unix_stream_client;

    string bindpath = "/tmp/unixsocket";
    char* answer = new char[128];

    memset(answer,0,128);

    try {
	unix_stream_server srv(bindpath);

	unique_ptr<unix_stream_client> client;
	client = srv.accept2();

	client->rcv(answer,127);
	// Alternatively:
	//string xyz;
	//xyz.resize(127);
	//*client >> xyz;

	std::cout << answer;

	*client << "Hello back from server!\n";

    } catch (const libsocket::socket_exception& exc)
    {
	std::cerr << exc.mesg;
    }

    return 0;
}
