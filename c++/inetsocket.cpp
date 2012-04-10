# include <iostream>
# include <string>
# include "../headers/libinetsocket.h"

namespace libsocket
{
	using std::string;

	struct inet_exception
	{


	class inet_socket
	{
		protected:
		int sfd;
		int proto;
		string remote_host;
		string remote_port;

		public:

		socket();
	}
	
	socket::socket() : sfd(-1), remote_host(""), remote_port("") {}

	class inet_stream : public inet_socket
	{
		private:

		public:

		inet_stream(string host, string port, int proto_osi3, int flags);
	}

	inet_stream::inet_stream(string host, string port, int proto_osi3, int flags)
	{
		sfd = create_inet_stream_socket(host.c_str(),port.c_str(),proto_osi3,flags);


}
