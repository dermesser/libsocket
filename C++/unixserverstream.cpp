# include "../headers/libunixsocket.h"
# include "../headers++/exception.hpp"
# include "../headers++/unixbase.hpp"
# include "../headers++/unixclientstream.hpp"
# include <string>

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
	}

	unix_stream_server::unix_stream_server(void)
		: listening(false)
	{
	}

	unix_stream_server::unix_stream_server(const char* path, int flags)
	{
		setup(path,flags);
	}

# if __cplusplus == 201103L
	unix_stream_server::unix_stream_server(const string& path, int flags)
		: unix_stream_server(path.c_str(),flags)
	{
	}
# endif

}
