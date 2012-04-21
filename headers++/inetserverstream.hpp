# ifndef INETSERVERSOCKET
# define INETSERVERSOCKET

# include "inetbase.hpp"
# include "inetstreamclient.hpp"

# define IPv4 3
# define IPv6 4

# define BOTH 5 // what fits best (TCP/UDP or IPv4/6)

namespace libsocket
{
	class inet_stream_server : public inet_socket
	{
		private:
		bool listening;

		public:

		inet_stream_server(void);
		inet_stream_server(const char* bindhost, const char* bindport, int proto_osi3, int flags=0);

		void setup(const char* bindhost, const char* bindport, int proto_osi3, int flags=0);

		inet_stream* accept(int numeric=0);

		string getbindhost(void);
		string getbindport(void);
	};
}

# endif
