# include <string>
# include <stdio.h>

namespace libsocket
{
	using std::string;

	struct socket_exception
	{
		string mesg;

		socket_exception(string,int,string);
	};

	socket_exception::socket_exception(string f, int l, string m)
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
}
