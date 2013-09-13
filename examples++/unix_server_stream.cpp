# include <libsocket/unixserverstream.hpp>
# include <libsocket/unixclientstream.hpp>
# include <libsocket/exception.hpp>
# include <string>
# include <iostream>
# include <string.h>

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

	unix_stream_client* client;
	client = srv.accept();

	client->rcv(answer,127);
	// Alternatively:
	//string xyz;
	//xyz.resize(127);
	//*client >> xyz;

	std::cout << answer;

	*client << "Hello back from server!\n";

	delete client;

	srv.destroy();

    } catch (libsocket::socket_exception exc)
    {
	std::cerr << exc.mesg;
    }

    return 0;
}
