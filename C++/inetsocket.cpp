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

/**************** inet_stream class (TCP inet sockets) *********/

	class inet_stream : public inet_socket
	{
		private:

		public:

		inet_stream(void);
		inet_stream(const char* dsthost, const char* dstport, int proto_osi3, int flags=0); // flags: socket()

		~inet_stream();

		// Real actions
		void connect(const char* dsthost, const char* dstport, int proto_osi3, int flags=0); // flags: socket()
		void shutdown(int method);
		void try_to_destroy(void);
		void destroy(void);

		// I/O
		// O
		friend inet_stream& operator<<(inet_stream& sock, const char* str);
		friend inet_stream& operator<<(inet_stream& sock, string& str);

		ssize_t snd(const void* buf, size_t len, int flags=0); // flags: send()

		// I
		friend inet_stream& operator>>(inet_stream& sock, string& dest);

		ssize_t rcv(void* buf, size_t len, int flags=0); // flags: recv()

		// Getters
		int getfd(void) const;
		string gethost(void) const;
		string getport(void) const;
	};

	// Managing

	inet_stream::inet_stream(void)
	{
	}

	inet_stream::inet_stream(const char* dsthost, const char* dstport, int proto_osi3, int flags)
	{
		try
		{
			connect(dsthost,dstport,proto_osi3,flags);
		} catch ( inet_exception(exc) )
		{
			throw inet_exception(__FILE__,__LINE__,"inet_stream::inet_stream() - Could not create socket");
		}
	}

	inet_stream::~inet_stream(void)
	{
		try_to_destroy();
	}

	void inet_stream::connect(const char* dsthost, const char* dstport, int proto_osi3, int flags)
	{
		if ( sfd != -1 ) // Socket is already connected
			throw inet_exception(__FILE__,__LINE__,"inet_stream::connect() - Already connected!\n");

		sfd = create_inet_stream_socket(dsthost,dstport,proto_osi3,flags);

		if ( sfd < 0 )
			throw inet_exception(__FILE__,__LINE__,"inet_stream::connect() - Could not create socket\n");

		host = dsthost;
		port = dstport;
		proto = proto_osi3;
	}


	void inet_stream::shutdown(int method)
	{
		if ( 0 > shutdown_inet_stream_socket(sfd,method))
		{
			throw inet_exception(__FILE__,__LINE__,"inet_stream::shutdown() - Could not shutdown socket\n");
		}
	}

	void inet_stream::try_to_destroy(void)
	{
		if ( sfd != -1 )
		{
			close(sfd);
			sfd = -1;
		}
	}

	void inet_stream::destroy(void)
	{
		if ( -1 == sfd )
			throw inet_exception(__FILE__,__LINE__,"inet_stream::destroy() - Socket already closed!\n");

		if ( 0 > destroy_inet_socket(sfd) )
			throw inet_exception(__FILE__,__LINE__,"inet_stream::destroy() - Could not close socket\n");

		sfd = -1;
	}

	// I/O

	// I

	ssize_t inet_stream::rcv(void* buf, size_t len, int flags)
	{
		ssize_t recvd;

		if ( sfd == -1 )
			throw inet_exception(__FILE__,__LINE__,"inet_stream::rcv() - Socket is not connected!\n");

		if ( buf == NULL || len == 0 )
			throw inet_exception(__FILE__,__LINE__,"inet_stream::rcv() - Buffer or length is null!\n");

		if ( -1 == (recvd = recv(sfd,buf,len,flags)) )
			throw inet_exception(__FILE__,__LINE__,"inet_stream::rcv() - Error while reading!\n");

		return recvd;
	}

	inet_stream& operator>>(inet_stream& sock, string& dest)
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

	inet_stream& operator<<(inet_stream& sock, const char* str)
	{
		if ( sock.sfd == -1 )
			throw inet_exception(__FILE__,__LINE__,"<<(const char*) output: Socket not connected!\n");
		if ( str == NULL )
			throw inet_exception(__FILE__,__LINE__,"<<(const char*) output: Null buffer given!\n");

		size_t len = strlen(str);

		if ( -1 == write(sock.sfd,str,len) )
			throw inet_exception(__FILE__,__LINE__,"<<(const char*) output: Write failed!\n");

		return sock;
	}

	inet_stream& operator<<(inet_stream& sock, string& str)
	{
		if ( sock.sfd == -1 )
			throw inet_exception(__FILE__,__LINE__,"<<(std::string) output: Socket not connected!\n");

		if ( -1 == write(sock.sfd,str.c_str(),str.size()) )
			throw inet_exception(__FILE__,__LINE__,"<<(std::string) output: Write failed!\n");

		return sock;
	}

	ssize_t inet_stream::snd(const void* buf, size_t len, int flags)
	{
		ssize_t snd_bytes;

		if ( sfd == -1 )
			throw inet_exception(__FILE__,__LINE__,"inet_stream::snd() - Socket not connected!\n");
		if ( buf == NULL || len == 0 )
			throw inet_exception(__FILE__,__LINE__,"inet_stream::snd() - Buffer or length is null!\n");

		if ( -1 == (snd_bytes = send(sfd,buf,len,flags)) )
			throw inet_exception(__FILE__,__LINE__,"inet_stream::snd() - Error while sending\n");

		return snd_bytes;
	}

	// Getters

	int inet_stream::getfd(void) const
	{
		return sfd;
	}

	string inet_stream::gethost(void) const
	{
		return host;
	}

	string inet_stream::getport(void) const
	{
		return port;
	}

/************** inet_dgram class (inet UDP sockets) ************/

	class inet_dgram : public inet_socket
	{
		private:
		bool connected;

		public:

		// Only create socket
		inet_dgram(int proto_osi3,int flags=0); // Flags: socket()
		// Create socket and connect it
		inet_dgram(const char* dsthost, const char* dstport, int proto_osi3, int flags=0); // Flags: socket()

		~inet_dgram();

		// actions
		// connect/reconnect
		void connect(const char* dsthost, const char* dstport);
		void deconnect(void);

		// I/O
		// destroy but don't complain
		void try_to_destroy(void);
		void destroy(void);

		// I/O
		// O
		// only if connected
		friend inet_dgram& operator<<(inet_dgram& sock, const char* str);
		friend inet_dgram& operator<<(inet_dgram& sock, string& str);

		ssize_t snd(const void* buf, size_t len, int flags=0); // flags: send()
		ssize_t sndto(const void* buf, size_t len, const char* dsthost, const char* dstport, int sndto_flags=0); // flags: sendto()

		// I
		friend inet_dgram& operator>>(inet_dgram& sock, string& dest);

		ssize_t rcv(void* buf, size_t len, int flags=0);
		ssize_t rcvfrom(void* buf, size_t len, char* dsthost, size_t hostlen, char* dstport, size_t portlen, int rcvfrom_flags=0, bool numeric=false);

		// Getters

		bool getconn(void);
		string gethost(void);
		string getport(void);
	};

	// Managing

	inet_dgram::inet_dgram(int proto_osi3, int flags) : connected(false)
	{
		if ( -1 == (sfd = create_inet_dgram_socket(proto_osi3,flags)) )
			throw inet_exception(__FILE__,__LINE__,"inet_dgram::inet_dgram() - Could not create inet dgram socket!\n");

		proto = proto_osi3;
	}

	inet_dgram::inet_dgram(const char* dsthost, const char* dstport, int proto_osi3, int flags)
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

	inet_dgram::~inet_dgram(void)
	{
		try_to_destroy();
	}

	void inet_dgram::connect(const char* dsthost, const char* dstport)
	{
		if ( -1 == (connect_inet_dgram_socket(sfd,dsthost,dstport)) )
			throw inet_exception(__FILE__,__LINE__,"inet_dgram::connect() - Could not connect dgram socket!\n");

		host = dsthost;
		port = dstport;
		connected = true;
	}

	void inet_dgram::deconnect(void)
	{
		if ( -1 == sfd )
			throw inet_exception(__FILE__,__LINE__,"inet_dgram::deconnect() - Socket not connected!\n");
		if ( -1 == (connect_inet_dgram_socket(sfd,0,0)) )
			throw inet_exception(__FILE__,__LINE__,"inet_dgram::deconnect() - Could not deconnect!\n");

		connected = false;
		host.resize(0);
		port.resize(0);
	}


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

	ssize_t inet_dgram::rcv(void* buf, size_t len, int flags)
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

	inet_dgram& operator>>(inet_dgram& sock, string& dest)
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

	ssize_t inet_dgram::snd(const void* buf, size_t len, int flags)
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

	ssize_t inet_dgram::sndto(const void* buf, size_t len, const char* dsthost, const char* dstport, int sndto_flags)
	{
		ssize_t bytes;

		if ( -1 == sfd )
			throw inet_exception(__FILE__,__LINE__,"inet_dgram::sendto() - Socket already closed!\n");

		if ( -1 == (bytes = sendto_inet_dgram_socket(sfd,buf,len,dsthost,dstport,sndto_flags)) )
			throw inet_exception(__FILE__,__LINE__,"inet_dgram::sndto() - Error at sendto\n");

		return bytes;
	}

	inet_dgram& operator<<(inet_dgram& sock, const char* str)
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

	inet_dgram& operator<<(inet_dgram& sock, string& str)
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
	bool inet_dgram::getconn(void)
	{
		return connected;
	}

	string inet_dgram::gethost(void)
	{
		if ( connected == false )
			throw inet_exception(__FILE__,__LINE__,"inet_dgram::gethost() - DGRAM socket not connected!\n");

		return host;
	}

	string inet_dgram::getport(void)
	{
		if ( connected == false )
			throw inet_exception(__FILE__,__LINE__,"inet_dgram::getport() - DGRAM socket not connected!\n");

		return port;
	}
}
