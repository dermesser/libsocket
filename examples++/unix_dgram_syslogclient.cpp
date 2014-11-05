# include <string>
# include <iostream>
# include "../headers/exception.hpp"
# include "../headers/unixclientdgram.hpp"

// Sends a little log message via DGRAM to the syslogd

int main(void)
{
    std::string sock = "/dev/log";
    std::string logmsg = "Hello, syslogd! :)";

    try {
	libsocket::unix_dgram_client logcl;

	logcl.sndto(logmsg,sock);

	logcl.destroy();
    } catch (const libsocket::socket_exception& exc)
    {
	std::cerr << exc.mesg;
    }

    return 0;
}
