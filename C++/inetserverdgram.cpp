# include "../headers/libinetsocket.h"
# include "../headers++/socket.hpp"
# include "../headers++/inetbase.hpp"
# include "../headers++/inetdgram.hpp"
# include "../headers++/inetstreamclient.hpp"

# include <string.h>
# include <string>

# define TCP 1
# define UDP 2

# define IPv4 3
# define IPv6 4

# define BOTH 5 // what fits best (TCP/UDP or IPv4/6)

namespace libsocket
{
	using std::string;

/****************** DGRAM *******************/
	class inet_dgram_server : public inet_dgram
	{
		private:

		bool bound;

		public:

		inet_dgram_server(const char* host, const char* port, int proto_osi3, int flags=0);
	};

	inet_dgram_server::inet_dgram_server(const char* bhost, const char* bport, int proto_osi3, int flags)
	{
		bound = false;

		if ( -1 == (sfd = create_inet_server_socket(bhost,bport,UDP,proto_osi3,flags)) )
			throw inet_exception(__FILE__,__LINE__,"inet_dgram_server::inet_dgram_server() - could not create server socket!\n");

		proto = proto_osi3;
		bound = true;

		host = string(bhost);
		port = string(bport);

	}
}
