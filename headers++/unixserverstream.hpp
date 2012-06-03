# ifndef UNIXSERVERSTREAM
# define UNIXSERVERSTREAM

# include <string>
# include "unixbase.hpp"
# include "unixclientstream.hpp"

namespace libsocket
{
	using std::string;

	class unix_stream_server : public unix_socket
	{
		private:
		// string bindpath; -> from unix_socket
		bool listening;

		public:

		unix_stream_server(void);
		unix_stream_server(const char* path, int flags=0);
# if __cplusplus == 201103L
		unix_stream_server(const string& path, int flags=0);
# endif
		void setup(const char* path, int flags=0);

		unix_stream_client* accept(int flags=0);
	};
}
# endif
