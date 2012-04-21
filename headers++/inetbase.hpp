# ifndef INETBASE
# define INETBASE
# include <string>
# include "socket.hpp"

namespace libsocket
{
	using std::string;

	struct inet_exception
	{
		string mesg;

		inet_exception(string,int,string);
	};

/***************** inet_socket base class **********************/

	class inet_socket : public socket
	{
		protected:
		int proto;
		string host;
		string port;

		public:

		inet_socket();
	};
}

# endif
