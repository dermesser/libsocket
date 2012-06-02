# include "../headers/libunixsocket.h"
# include "../headers++/exception.hpp"
# include "../headers++/unixbase.hpp"
# include <string>
# include <unistd.h>
# include <string.h>

namespace libsocket
{
	class unix_dgram : public unix_socket
	{
		public:

		ssize_t sndto(const void* buf, size_t length, const char* path, int sendto_flags=0);
		ssize_t sndto(const void* buf, size_t length, const string& path, int sendto_flags=0);

		ssize_t rcvfrom(void* buf, size_t length, char* source, size_t source_len, int recvfrom_flags=0);
		ssize_t rcvfrom(void* buf, size_t length, string& source, int recvfrom_flags=0);

	};

	// Main sendto function
	ssize_t unix_dgram::sndto(const void* buf, size_t length, const char* path, int sendto_flags)
	{
		if ( buf == NULL )
			throw socket_exception(__FILE__,__LINE__,"unix_dgram::sndto: Buffer is NULL!\n");
		
		ssize_t bytes;

		if ( 0 > (bytes = sendto_unix_dgram_socket(sfd,buf,length,path,sendto_flags)) )
			throw socket_exception(__FILE__,__LINE__,"unix_dgram::sndto: Could not send data to peer!\n");
		
		return bytes;
	}

	ssize_t unix_dgram::sndto(const void* buf, size_t length, const string& path, int sendto_flags)
	{
		return sndto(buf,length,path.c_str(),sendto_flags);
	}

	
	ssize_t unix_dgram::rcvfrom(void* buf, size_t length, char* source, size_t source_len, int recvfrom_flags)
	{
		if ( buf == NULL )
			throw socket_exception(__FILE__,__LINE__,"unix_dgram::rcvfrom: Buffer is NULL!\n");

		ssize_t bytes;

		bytes = recvfrom_unix_dgram_socket(sfd,buf,length,source,source_len,recvfrom_flags);
		
		if ( bytes < 0 )
			throw socket_exception(__FILE__,__LINE__,"unix_dgram::rcvfrom: Could not receive data from peer!\n");

		return bytes;
	}

	ssize_t unix_dgram::rcvfrom(void* buf, size_t length, string& source, int recvfrom_flags)
	{
		if ( buf == NULL )
			throw socket_exception(__FILE__,__LINE__,"unix_dgram::rcvfrom: Buffer is NULL!\n");

		ssize_t bytes;

		char* source_cstr = new char[512]; // AFAIK, the address field in struct sockaddr_un is only 108 bytes...
		size_t source_cstr_len;

		memset(source_cstr,0,512);

		bytes = recvfrom_unix_dgram_socket(sfd,buf,length,source_cstr,512,recvfrom_flags);

		if ( bytes < 0 )
			throw socket_exception(__FILE__,__LINE__,"unix_dgram::rcvfrom: Could not receive data from peer!\n");

		source_cstr_len = strlen(source_cstr);

		source.resize(source_cstr_len);

		source = source_cstr;

		return bytes;
	}
}
