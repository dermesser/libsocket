# include "../headers/libunixsocket.h"
# include "../headers++/exception.hpp"
# include "../headers++/unixdgram.hpp"

# include <sys/types.h>
# include <sys/socket.h>

# include <string>

using std::string;

namespace libsocket
{
	class unix_dgram_server : public unix_dgram
	{
		private:
		bool bound;

		public:

		unix_dgram_server(void);
		unix_dgram_server(const char* bindpath, int socket_flags=0);
		unix_dgram_server(const string& bindpath, int socket_flags=0);

		void setup(const char* bindpath, int socket_flags=0);
		void setup(const string& bindpath, int socket_flags=0);
	};

	unix_dgram_server::unix_dgram_server(void)
		: bound(false)
	{ }

	unix_dgram_server::unix_dgram_server(const char* bindpath, int socket_flags)
	{
		setup(bindpath,socket_flags);
	}

	unix_dgram_server::unix_dgram_server(const string& bindpath, int socket_flags)
	{
		setup(bindpath.c_str(),socket_flags);
	}

	void unix_dgram_server::setup(const char* bindpath, int socket_flags)
	{
		sfd = create_unix_server_socket(bindpath, DGRAM, socket_flags);

		if ( sfd < 0 )
			throw socket_exception(__FILE__,__LINE__,"unix_dgram_server::setup: Could not create server!\n");
	}

	void unix_dgram_server::setup(const string& bindpath, int socket_flags)
	{
		setup(bindpath.c_str(),socket_flags);
	}
}
