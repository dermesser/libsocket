# ifndef STREAMCLIENT
# define STREAMCLIENT

# include <string>
# include "socket.hpp"

using std::string;

namespace libsocket
{
	class stream_client_socket : public virtual socket
	{
		public:

		ssize_t snd(const void* buf, size_t len, int flags=0); // flags: send()
		ssize_t rcv(void* buf, size_t len, int flags=0); // flags: recv()
		
		friend stream_client_socket& operator<<(stream_client_socket& sock, const char* str);
		friend stream_client_socket& operator<<(stream_client_socket& sock, string& str);
		friend stream_client_socket& operator>>(stream_client_socket& sock, string& dest);
	};
}
# endif
