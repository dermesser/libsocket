# include <string>
# include <iostream>
# include "../headers++/exception.hpp"
# include "../headers++/unixclientdgram.hpp"

int main(void)
{
	std::string sock = "/dev/log";

	libsocket::unix_dgram_client logcl;

	logcl.sndto("Hello, syslogd!",15,sock);

	logcl.destroy();

	return 0;
}
