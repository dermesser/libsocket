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

	// Managing

	void inet_dgram::try_to_destroy(void)
	{
		if ( sfd != -1 )
		{
			close(sfd);
			sfd = -1;
		}
	}

	void inet_dgram::destroy(void)
	{
		if ( -1 == sfd )
			return;

		if ( -1 == destroy_inet_socket(sfd) )
			throw inet_exception(__FILE__,__LINE__,"inet_dgram::destroy() - Could not close socket!\n");
	}

	// I/O

	// I

	ssize_t inet_dgram::rcvfrom(void* buf, size_t len, char* hostbuf, size_t hostbuflen, char* portbuf, size_t portbuflen, int rcvfrom_flags, bool numeric)
	{
		ssize_t bytes;
		int num = ((numeric == true) ? NUMERIC : 0);

		if ( -1 == sfd )
			throw inet_exception(__FILE__,__LINE__,"inet_dgram::rcvfrom() - Socket already closed!\n");

		if ( -1 == (bytes = recvfrom_inet_dgram_socket(sfd,buf,len,hostbuf,hostbuflen,portbuf,portbuflen,rcvfrom_flags,num)) )
			throw inet_exception(__FILE__,__LINE__,"inet_dgram::rcvfrom() - recvfrom() failed!\n");

		return bytes;
	}

	// O

	ssize_t inet_dgram::sndto(const void* buf, size_t len, const char* dsthost, const char* dstport, int sndto_flags)
	{
		ssize_t bytes;

		if ( -1 == sfd )
			throw inet_exception(__FILE__,__LINE__,"inet_dgram::sendto() - Socket already closed!\n");

		if ( -1 == (bytes = sendto_inet_dgram_socket(sfd,buf,len,dsthost,dstport,sndto_flags)) )
			throw inet_exception(__FILE__,__LINE__,"inet_dgram::sndto() - Error at sendto\n");

		return bytes;
	}

	// Getters

	string inet_dgram::gethost(void)
	{
		return host;
	}

	string inet_dgram::getport(void)
	{
		return port;
	}
}
