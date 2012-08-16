# include <string>
# include <iostream>
# include "../headers/exception.hpp"
# include "../headers/unixclientdgram.hpp"

// Sends a little log message via DGRAM to the syslogd

int main(void)
{
	std::string sock = "/dev/log";
	std::string logmsg = "Hello, syslogd! :)";

	libsocket::unix_dgram_client logcl;

	logcl.sndto(logmsg,sock);

	logcl.destroy();

	return 0;
}
