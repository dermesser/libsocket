# include <iostream>
# include "../headers++/socket.hpp"
# include <string>
# include <stdio.h>

# define STREAM 1
# define DGRAM  2

namespace libsocket
{
	using std::string;

	struct unix_exception
	{
		string mesg;

		unix_exception(string,int,string);
	};

	unix_exception::unix_exception(string f, int l, string m)
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

	/* ******************** */

	class unix_socket : public socket
	{
		private:
		string bindpath;

		public:
		unix_socket();
	};

	unix_socket::unix_socket(void) : bindpath("") {}
}
