# ifndef INETDGRAM
# define INETDGRAM

# include <iostream>
# include <string>
# include <string.h>

# include "../headers/libinetsocket.h"
# include "../headers++/socket.hpp"
# include "../headers++/inetbase.hpp"

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
		private:
		public:

		~inet_dgram();

		// I/O
		// destroy but don't complain
		void try_to_destroy(void);
		void destroy(void);

		// I/O
		// O
		ssize_t sndto(const void* buf, size_t len, const char* dsthost, const char* dstport, int sndto_flags=0); // flags: sendto()

		// I
		ssize_t rcvfrom(void* buf, size_t len, char* dsthost, size_t hostlen, char* dstport, size_t portlen, int rcvfrom_flags=0, bool numeric=false);

		// Getters

		string gethost(void);
		string getport(void);
	};

}

# endif
