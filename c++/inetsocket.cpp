# include <iostream>
# include <string>
# include "../headers/libinetsocket.h"

// Quite simple oo wrapper around libinetsocket

namespace libsocket
{
	using std::string;

	struct inet_exception
	{
		string mesg;

		inet_exception(string);
	};

	inet_exception::inet_exception(string m) : mesg(m) {}

// // // // // // //

	class inet_socket
	{
		protected:
		int sfd;
		int proto;
		string remote_host;
		string remote_port;

		public:

		inet_socket();
	};
	
	inet_socket::inet_socket() : sfd(-1), remote_host(""), remote_port("") {}

	class inet_stream : public inet_socket
	{
		private:

		public:

		inet_stream(string host, string port, int proto_osi3, int flags);
		~inet_stream();

		int shutdown(int method);
	};

	inet_stream::inet_stream(string host, string port, int proto_osi3, int flags)
	{
		sfd = create_inet_stream_socket(host.c_str(),port.c_str(),proto_osi3,flags);

		if ( sfd < 0 )
			throw inet_exception("Could not create socket\n");
	}

	inet_stream::~inet_stream(void)
	{
		if ( 0 > destroy_inet_socket(sfd) )
			throw inet_exception("Could not close socket\n");
	}

	int inet_stream::shutdown(int method)
	{
		if ( 0 > shutdown_inet_stream_socket(sfd,method))
		{
			throw inet_exception("Could not shutdown socket\n");
		}

		return 0;
	}
}
