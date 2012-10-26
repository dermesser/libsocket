# ifndef _STREAMCLIENT_H
# define _STREAMCLIENT_H

# include <string>
# include "socket.hpp"

# define LIBSOCKET_READ 1
# define LIBSOCKET_WRITE 2

using std::string;

namespace libsocket
{
	class stream_client_socket : public virtual socket
	{
		protected:

		bool shut_rd;
		bool shut_wr;

		public:

		stream_client_socket();

		ssize_t snd(const void* buf, size_t len, int flags=0); // flags: send()
		ssize_t rcv(void* buf, size_t len, int flags=0); // flags: recv()

		friend stream_client_socket& operator<<(stream_client_socket& sock, const char* str);
		friend stream_client_socket& operator<<(stream_client_socket& sock, string& str);
		friend stream_client_socket& operator>>(stream_client_socket& sock, string& dest);

		void shutdown(int method=LIBSOCKET_WRITE);
	};
}
# endif
