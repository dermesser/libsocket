# ifndef INETSERVERSOCKET
# define INETSERVERSOCKET

# include "inetbase.hpp"
# include "inetdgram.hpp"

# define IPv4 3
# define IPv6 4

# define BOTH 5 // what fits best (TCP/UDP or IPv4/6)

namespace libsocket
{
	class inet_dgram_server : public inet_dgram
	{
		private:

		bool bound;

		public:

		inet_dgram_server(const char* host, const char* port, int proto_osi3, int flags=0);
	};
}

# endif
