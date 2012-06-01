# ifndef SOCKET_EXCEPTION
# define SOCKET_EXCEPTION

# include <string>

using std::string;

struct socket_exception
{       
	string mesg;

	socket_exception(string,int,string);
};

# endif
