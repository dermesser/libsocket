# include <string>
# include <iostream>
# include "../headers/exception.hpp"
# include "../headers/unixserverdgram.hpp"

// Sends a little log message via DGRAM to the syslogd

int main(void)
{
    std::string req;
    std::string client;

    req.resize(64);
    client.resize(64);

    try {
	libsocket::unix_dgram_server srv("/tmp/srvsock");

	srv.rcvfrom(req,client);

	std::cout << "[server process] " << req << " from " << client << std::endl;

	srv.sndto("Hiho, client!",client);

	srv.destroy();

    } catch (const libsocket::socket_exception& exc)
    {
	std::cerr << exc.mesg;
    }

    return 0;
}
