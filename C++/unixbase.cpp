# include <iostream>
# include "../headers++/socket.hpp"
# include <string>
# include <stdio.h>

# define STREAM 1
# define DGRAM  2

namespace libsocket
{
	using std::string;

	class unix_socket : public socket
	{
		private:
		string bindpath;

		public:
		unix_socket();
	};

	unix_socket::unix_socket(void) : bindpath("") {}
}
