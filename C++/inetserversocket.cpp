# include "../headers/libinetsocket.h"
# include "../headers++/socket.hpp"
# include "../headers++/inetbase.hpp"
# include "../headers++/inetsocket.hpp"
# include <string.h>

# include <string>

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

		inet_stream accept(int numeric=0);
	};

	inet_stream_server::inet_stream_server(void) : listening(false)
	{
	}

	inet_stream_server::inet_stream_server(const char* bindhost, const char* bindport, int proto_osi3, int flags)
	{
		setup(bindhost,bindport,proto_osi3,flags);
	}

	void inet_stream_server::setup(const char* bindhost, const char* bindport, int proto_osi3, int flags)
	{
		if ( listening == true )
			throw inet_exception(__FILE__,__LINE__,"inet_stream_server::inet_stream_server() - already bound and listening!\n");
		if ( bindhost == 0 || bindport == 0 )
			throw inet_exception(__FILE__,__LINE__,"inet_stream_server::inet_stream_server() - at least one bind argument invalid!\n");

		if ( -1 == (sfd = create_inet_server_socket(bindhost,bindport,TCP,proto_osi3,flags)) )
			throw inet_exception(__FILE__,__LINE__,"inet_stream_server::inet_stream_server() - could not create server socket!\n");

		listening = true;
	}

	inet_stream inet_stream_server::accept(int numeric)
	{
		char* src_host = new char[1024];
		char* src_port = new char[32];

		int client_sfd;
		inet_stream client;

		memset(src_host,0,1024);
		memset(src_port,0,32);

		if ( listening != true )
			throw inet_exception(__FILE__,__LINE__,"inet_stream_server::accept() - stream server socket is not in listening state!\n");
		if ( -1 == (client_sfd = accept_inet_stream_socket(sfd,src_host,1024,src_port,32,numeric)) )
			throw inet_exception(__FILE__,__LINE__,"inet_stream_server::accept() - could not accept new connection on stream server socket!\n");

		client.sfd = client_sfd;
		client.host = string(src_host);
		client.port = string(src_port);
		client.proto = proto;

		return client;
	}

}
