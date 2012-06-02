# ifndef UNIXBASE
# define UNIXBASE
# include <string>
# include "../headers++/socket.hpp"

namespace libsocket
{
	using std::string;

	struct unix_exception
	{
		string mesg;

		unix_exception(string,int,string);
	};

	/* ******************** */

	class unix_socket : public socket
	{
		private:
		string bindpath;

		public:
		unix_socket();
	};
}
# endif
