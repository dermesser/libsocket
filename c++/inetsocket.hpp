# include <string>
# include "../headers/libinetsocket.h"

# define TCP 1
# define UDP 2

# define IPv4 3
# define IPv6 4

# define BOTH 5 // what fits best (TCP/UDP or IPv4/6)

# define READ 1
# define WRITE 2

# define NUMERIC 1

// Quite simple oo wrapper around libinetsocket

namespace libsocket
{
	using std::string;

	struct inet_exception
	{
		string mesg;

		inet_exception(string);
	};

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
	
	class inet_stream : public inet_socket
	{
		private:

		public:

		inet_stream(string host, string port, int proto_osi3, int flags);
		~inet_stream();

		int shutdown(int method);
	};
}
