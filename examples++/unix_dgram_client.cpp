# include <string>
# include <iostream>
# include "../headers/exception.hpp"
# include "../headers/unixclientdgram.hpp"

// Sends a little log message via DGRAM to the syslogd

int main(void)
{
    std::string sock = "/tmp/srvsock";
    std::string logmsg = "Hello, server!";
    std::string answer, peer;

    answer.resize(64);
    peer.resize(64);

    try {
	libsocket::unix_dgram_client cl("/tmp/clsock");

	cl.sndto(logmsg,sock);

	cl.rcvfrom(answer,peer);

	std::cout << "[client process] " << answer << " from " << peer << std::endl;

	cl.destroy();

    } catch (const libsocket::socket_exception& exc)
    {
	std::cerr << exc.mesg;
    }

    return 0;
}
