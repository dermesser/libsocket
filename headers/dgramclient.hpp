# ifndef _DGRAMCLIENT_H
# define _DGRAMCLIENT_H

# include "socket.hpp"
# include <string>
# include <unistd.h>
# include <string.h>

using std::string;

namespace libsocket
{
	class dgram_client_socket : public virtual socket
	{
		protected:
		bool connected;

		public:

		dgram_client_socket(void);

		friend dgram_client_socket& operator<<(dgram_client_socket& sock, const char* str);
		friend dgram_client_socket& operator<<(dgram_client_socket& sock, string& str);

		ssize_t snd(const void* buf, size_t len, int flags=0); // flags: send()

		// I
		friend dgram_client_socket& operator>>(dgram_client_socket& sock, string& dest);

		ssize_t rcv(void* buf, size_t len, int flags=0);

		// Getters

		bool getconn(void) const;
	};

}
# endif
