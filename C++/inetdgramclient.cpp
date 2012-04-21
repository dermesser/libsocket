# include <iostream>
# include <string>
# include <string.h>

# include "../headers/libinetsocket.h"
# include "../headers++/socket.hpp"
# include "../headers++/inetbase.hpp"
# include "../headers++/inetdgram.hpp"

# include <unistd.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <stdio.h>

// oo wrapper around libinetsocket

namespace libsocket
{
	using std::string;

/************** inet_dgram class (inet UDP sockets) ************/

	class inet_dgram_client : public inet_dgram
	{
		private:
		bool connected;

		public:

		// Only create socket
		inet_dgram_client(int proto_osi3,int flags=0); // Flags: socket()
		// Create socket and connect it
		inet_dgram_client(const char* dsthost, const char* dstport, int proto_osi3, int flags=0); // Flags: socket()

		~inet_dgram_client();

		// actions
		// connect/reconnect
		void connect(const char* dsthost, const char* dstport);
		void deconnect(void);

		// I/O
		// O
		// only if connected
		friend inet_dgram_client& operator<<(inet_dgram_client& sock, const char* str);
		friend inet_dgram_client& operator<<(inet_dgram_client& sock, string& str);

		ssize_t snd(const void* buf, size_t len, int flags=0); // flags: send()

		// I
		friend inet_dgram_client& operator>>(inet_dgram_client& sock, string& dest);

		ssize_t rcv(void* buf, size_t len, int flags=0);

		// Getters

		bool getconn(void);
	};

	// Managing

	inet_dgram_client::inet_dgram_client(int proto_osi3, int flags) : connected(false)
	{
		if ( -1 == (sfd = create_inet_dgram_socket(proto_osi3,flags)) )
			throw inet_exception(__FILE__,__LINE__,"inet_dgram::inet_dgram() - Could not create inet dgram socket!\n");

		proto = proto_osi3;
	}

	inet_dgram_client::inet_dgram_client(const char* dsthost, const char* dstport, int proto_osi3, int flags)
	{
		if ( -1 == (sfd = create_inet_dgram_socket(proto_osi3,flags)) )
			throw inet_exception(__FILE__,__LINE__,"inet_dgram::inet_dgram() - Could not create inet dgram socket!\n");

		try {
			connect(dsthost,dstport);
		} catch (inet_exception exc)
		{
			throw inet_exception(__FILE__,__LINE__,"inet_dgram::inet_dgram() - Could not connect dgram socket\n");
		}
	}

	inet_dgram_client::~inet_dgram_client(void)
	{
		try_to_destroy();
	}

	void inet_dgram_client::connect(const char* dsthost, const char* dstport)
	{
		if ( -1 == (connect_inet_dgram_socket(sfd,dsthost,dstport)) )
			throw inet_exception(__FILE__,__LINE__,"inet_dgram::connect() - Could not connect dgram socket!\n");

		host = dsthost;
		port = dstport;
		connected = true;
	}

	void inet_dgram_client::deconnect(void)
	{
		if ( -1 == sfd )
			throw inet_exception(__FILE__,__LINE__,"inet_dgram::deconnect() - Socket not connected!\n");
		if ( -1 == (connect_inet_dgram_socket(sfd,0,0)) )
			throw inet_exception(__FILE__,__LINE__,"inet_dgram::deconnect() - Could not deconnect!\n");

		connected = false;
		host.resize(0);
		port.resize(0);
	}

	// I/O

	// I

	ssize_t inet_dgram_client::rcv(void* buf, size_t len, int flags)
	{
		ssize_t bytes;

		if ( -1 == sfd )
			throw inet_exception(__FILE__,__LINE__,"inet_dgram::rcv() - Socket already closed!\n");

		if ( connected != true )
			throw inet_exception(__FILE__,__LINE__,"inet_dgram::rcv() - Socket is not connected!\n");

		if ( -1 == (bytes = recv(sfd,buf,len,flags)) )
			throw inet_exception(__FILE__,__LINE__,"inet_dgram::rcv() - recv() failed!\n");

		return bytes;
	}

	inet_dgram_client& operator>>(inet_dgram_client& sock, string& dest)
	{
		ssize_t read_bytes;
		char* buffer;

		buffer = new char[dest.size()];

		if ( sock.sfd == -1 )
			throw inet_exception(__FILE__,__LINE__,">>(std::string) input: Socket not connected!\n");

		if ( -1 == (read_bytes = read(sock.sfd,buffer,dest.size())) )
			throw inet_exception(__FILE__,__LINE__,">>(std::string) input: Error while reading!\n");

		if ( read_bytes < static_cast<ssize_t>(dest.size()) )
			dest.resize(read_bytes); // So the client doesn't print content more than one time
						 // and it can check if the string's length is 0 (end of transmission)

		dest.assign(buffer,read_bytes);

		delete buffer;

		return sock;
	}

	// O

	ssize_t inet_dgram_client::snd(const void* buf, size_t len, int flags)
	{
		ssize_t bytes;

		if ( -1 == sfd )
			throw inet_exception(__FILE__,__LINE__,"inet_dgram::snd() - Socket already closed!\n");

		if ( connected != true )
			throw inet_exception(__FILE__,__LINE__,"inet_dgram::snd() - Socket is not connected!\n");

		if ( -1 == (bytes = send(sfd,buf,len,flags)) )
			throw inet_exception(__FILE__,__LINE__,"inet_dgram::snd() - send() failed!\n");

		return bytes;
	}

	inet_dgram_client& operator<<(inet_dgram_client& sock, const char* str)
	{
		if ( sock.sfd == -1 )
			throw inet_exception(__FILE__,__LINE__,"inet dgram <<(const char*) output: Socket not connected!\n");
		if ( str == NULL )
			throw inet_exception(__FILE__,__LINE__,"inet dgram <<(const char*) output: Null buffer given!\n");
		if ( sock.connected == false )
			throw inet_exception(__FILE__,__LINE__,"inet dgram <<(const char*) output: DGRAM socket not connected!\n");

		size_t len = strlen(str);

		if ( -1 == write(sock.sfd,str,len) )
			throw inet_exception(__FILE__,__LINE__,"inet dgram <<(const char*) output: Write failed!\n");

		return sock;
	}

	inet_dgram_client& operator<<(inet_dgram_client& sock, string& str)
	{
		if ( sock.sfd == -1 )
			throw inet_exception(__FILE__,__LINE__,"inet dgram<<(std::string) output: Socket not connected!\n");
		if ( sock.connected == false )
			throw inet_exception(__FILE__,__LINE__,"inet dgram <<(std::string) output: DGRAM socket not connected!\n");
		if ( -1 == write(sock.sfd,str.c_str(),str.size()) )
			throw inet_exception(__FILE__,__LINE__,"inet dgram <<(std::string) output: Write failed!\n");

		return sock;
	}

	// Getters
	bool inet_dgram_client::getconn(void)
	{
		return connected;
	}
}
