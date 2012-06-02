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
	};

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
	void unix_stream_server::setup(const char* path, int flags)
	{
		if ( path == NULL )
			throw socket_exception(__FILE__,__LINE__,"unix_stream_server::setup: Path is NULL!\n");

		sfd = create_unix_server_socket(path,STREAM,flags);

		if ( sfd < 0 )
			throw socket_exception(__FILE__,__LINE__,"unix_stream_server::setup: Error at creating UNIX stream server socket!\n");
	}

	unix_stream_client* unix_stream_server::accept(int flags)
	{
		if (listening != true)
			throw socket_exception(__FILE__,__LINE__,"unix_stream_server::accept: Socket not set up yet!\n");

		unix_stream_client* client = new unix_stream_client;

		int cfd = accept_unix_stream_socket(sfd,flags);

		if ( cfd < 0 )
			throw socket_exception(__FILE__,__LINE__,"unix_stream_server::accept: Error at accepting new connection!\n");

		client->sfd = cfd;

		return client;
	}

}
