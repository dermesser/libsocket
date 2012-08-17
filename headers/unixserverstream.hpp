# ifndef _UNIXSERVERSTREAM_H
# define _UNIXSERVERSTREAM_H

# include <string>

# include "unixbase.hpp"
# include "unixclientstream.hpp"

namespace libsocket
{
	using std::string;

	class unix_stream_server : public unix_socket
	{
		public:

		unix_stream_server(void);
		unix_stream_server(const char* path, int flags=0);
		unix_stream_server(const string& path, int flags=0);

		void setup(const char* path, int flags=0);
		void setup(const string& path, int flags=0);

		unix_stream_client* accept(int flags=0);
	};
}
# endif
