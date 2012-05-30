# ifndef INETDGRAM
# define INETDGRAM

# include <iostream>
# include <string>
# include <string.h>

# include "../headers/libinetsocket.h"
# include "socket.hpp"
# include "inetbase.hpp"

# include <unistd.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <stdio.h>

// oo wrapper around libinetsocket

namespace libsocket
{
	using std::string;

/************** inet_dgram class (inet UDP sockets) ************/

	class inet_dgram : public inet_socket
	{
		public:

		// I/O
		// O
		ssize_t sndto(const void* buf, size_t len, const char* dsthost, const char* dstport, int sndto_flags=0); // flags: sendto()

		// I
		ssize_t rcvfrom(void* buf, size_t len, char* srchost, size_t hostlen, char* srcport, size_t portlen, int rcvfrom_flags=0, bool numeric=false);
		ssize_t rcvfrom(void* buf, size_t len, string& srchost, string& srcport, int rcvfrom_flags=0, bool numeric=false);
	};
}

# endif
