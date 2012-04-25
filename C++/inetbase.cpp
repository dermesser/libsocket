# include <string>
# include "../headers++/socket.hpp"
# include <stdio.h>

namespace libsocket
{
	using std::string;

	struct inet_exception
	{
		string mesg;

		inet_exception(string,int,string);
	};

	inet_exception::inet_exception(string f, int l, string m)
	{
		char line[5];

		line[4] = 0;

		sprintf(line,"%i",l);

		m.insert(0,": ");
		m.insert(0,line);
		m.insert(0,":");
		m.insert(0,f);

		mesg = m;
	}

/***************** inet_socket base class **********************/

	class inet_socket : public socket
	{
		protected:
		int proto;
		string host;
		string port;

		public:
		inet_socket();

		string gethost(void) const;
		string getport(void) const;
	};

	inet_socket::inet_socket() : host(""), port("") {}

	string inet_socket::gethost(void) const
	{
		return host;
	}

	string inet_socket::getport(void) const
	{
		return port;
	}
}
