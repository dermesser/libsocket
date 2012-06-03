# ifndef UNIXSERVERDGRAM
# define UNIXSERVERDGRAM

# include "unixdgram.hpp"
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
}

# endif
