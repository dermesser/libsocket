# include <iostream>
# include <string>
# include <string.h>
# include "../headers/libinetsocket.h"
# include "../headers++/socket.hpp"
# include <unistd.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <stdio.h>

// oo wrapper around libinetsocket

namespace libsocket
{
	using std::string;

	struct inet_exception
	{
		string mesg;

		inet_exception(string,int,string);
	};

	inet_exception::inet_exception(string f, int l, string m)
	{
		char line[5];

		line[4] = 0;

		sprintf(line,"%i",l);

		m.insert(0,": ");
		m.insert(0,line);
		m.insert(0,":");
		m.insert(0,f);

		mesg = m;
	}

/***************** inet_socket base class **********************/

	class inet_socket : public socket
	{
		protected:
		int proto;
		const char* remote_host;
		const char* remote_port;

		public:

		inet_socket();
	};

	inet_socket::inet_socket() {}

/**************** inet_stream class (TCP inet sockets) *********/

	class inet_stream : public inet_socket
	{
		private:

		public:

		inet_stream(void);
		inet_stream(const char* host, const char* port, int proto_osi3, int flags);
		
		~inet_stream();

		// Real actions
		void connect(const char* host, const char* port, int proto_osi3, int flags);
		void shutdown(int method);
		void try_to_destroy(void);
		void destroy(void);

		// I/O
		// O
		friend inet_stream& operator<<(inet_stream& sock, const char* str);
		friend inet_stream& operator<<(inet_stream& sock, string& str);

		ssize_t snd(const void* buf, size_t len, int flags);

		// I
		friend inet_stream& operator>>(inet_stream& sock, string& dest);

		ssize_t rcv(void* buf, size_t len, int flags);

		// Getters
		int getfd(void) const;
		const char* gethost(void) const;
		const char* getport(void) const;
	};

	// Managing

	inet_stream::inet_stream(void)
	{
	}

	inet_stream::inet_stream(const char* host, const char* port, int proto_osi3, int flags)
	{
		try 
		{ 
			connect(host,port,proto_osi3,flags); 
		} catch ( inet_exception(exc) )
		{
			throw inet_exception(__FILE__,__LINE__,"Could not create socket");
		}
	}

	inet_stream::~inet_stream(void)
	{
		try_to_destroy();
	}

	void inet_stream::connect(const char* host, const char* port, int proto_osi3, int flags)
	{
		if ( sfd != -1 ) // Socket is already connected
			throw inet_exception(__FILE__,__LINE__,"Already connected!\n");

		sfd = create_inet_stream_socket(host,port,proto_osi3,flags);

		if ( sfd < 0 )
			throw inet_exception(__FILE__,__LINE__,"Could not create socket\n");

		remote_host = host;
		remote_port = port;
		proto = proto_osi3;
	}


	void inet_stream::shutdown(int method)
	{
		if ( 0 > shutdown_inet_stream_socket(sfd,method))
		{
			throw inet_exception(__FILE__,__LINE__,"Could not shutdown socket\n");
		}
	}

	void inet_stream::try_to_destroy(void)
	{
		close(sfd);
		sfd = -1;
	}

	void inet_stream::destroy(void)
	{
		if ( -1 == sfd )
			throw inet_exception(__FILE__,__LINE__,"Socket was not connected!\n");

		if ( 0 > destroy_inet_socket(sfd) )
			throw inet_exception(__FILE__,__LINE__,"Could not close socket\n");

		sfd = -1;
	}

	// I/O

	// I

	inet_stream& operator<<(inet_stream& sock, const char* str)
	{
		if ( sock.sfd == -1 )
			throw inet_exception(__FILE__,__LINE__,"Socket not connected!\n");
		if ( str == NULL )
			throw inet_exception(__FILE__,__LINE__,"Null buffer given!\n");

		size_t len = strlen(str);

		if ( -1 == write(sock.sfd,str,len) )
			throw inet_exception(__FILE__,__LINE__,"Write failed!\n");

		return sock;
	}

	inet_stream& operator<<(inet_stream& sock, string& str)
	{
		if ( sock.sfd == -1 )
			throw inet_exception(__FILE__,__LINE__,"Socket not connected!\n");

		if ( -1 == write(sock.sfd,str.c_str(),str.size()) )
			throw inet_exception(__FILE__,__LINE__,"Write failed!\n");

		return sock;
	}

	ssize_t inet_stream::rcv(void* buf, size_t len, int flags)
	{
		ssize_t recvd;

		if ( sfd == -1 )
			throw inet_exception(__FILE__,__LINE__,"Socket is not connected!\n");

		if ( buf == NULL || len == 0 )
			throw inet_exception(__FILE__,__LINE__,"Buffer or length is null!\n");

		if ( -1 == (recvd = recv(sfd,buf,len,flags)) )
			throw inet_exception(__FILE__,__LINE__,"Error while reading!\n");

		return recvd;
	}

	// O

	inet_stream& operator>>(inet_stream& sock, string& dest)
	{
		ssize_t read_bytes;
		char* buffer; 

		buffer = new char[dest.size()];

		if ( sock.sfd == -1 )
			throw inet_exception(__FILE__,__LINE__,"Socket not connected!\n");

		if ( -1 == (read_bytes = read(sock.sfd,buffer,dest.size())) )
			throw inet_exception(__FILE__,__LINE__,"Error while reading!\n");

		if ( read_bytes < static_cast<ssize_t>(dest.size()) )
			dest.resize(read_bytes); // So the client doesn't print content more than one time
						 // and it can check if the string's length is 0 (end of transmission)

		dest.assign(buffer,read_bytes);

		delete buffer;

		return sock;
	}

	ssize_t inet_stream::snd(const void* buf, size_t len, int flags)
	{
		ssize_t snd_bytes;

		if ( sfd == -1 )
			throw inet_exception(__FILE__,__LINE__,"Socket not connected!\n");
		if ( buf == NULL || len == 0 )
			throw inet_exception(__FILE__,__LINE__,"Buffer or length is null!\n");

		if ( -1 == (snd_bytes = send(sfd,buf,len,flags)) )
			throw inet_exception(__FILE__,__LINE__,"Error while sending\n");

		return snd_bytes;
	}

	// Getters

	int inet_stream::getfd(void) const
	{
		return sfd;
	}

	const char* inet_stream::gethost(void) const
	{
		return remote_host;
	}

	const char* inet_stream::getport(void) const
	{
		return remote_port;
	}

/************** inet_dgram class (inet UDP sockets) ************/

	class inet_dgram : public inet_socket
	{
		private:
		bool connected;

		public:
		
		inet_dgram(void);
		inet_dgram(int proto_osi3);
		inet_dgram(const char* host, const char* port, int proto_osi3, int flags);
		
		~inet_dgram();

		// actions
		void connect(const char* host, const char* port, int proto_osi3, int flags);
		void deconnect(void);

		// I/O

		void try_to_destroy(void);
		void destroy(void);

		// I/O
		// O
		friend inet_stream& operator<<(inet_stream& sock, const char* str);
		friend inet_stream& operator<<(inet_stream& sock, string& str);

		ssize_t snd(const void* buf, size_t len, int flags);
		ssize_t sndto(const char* host, const char* port, const void* buf, size_t len, int flags);
		// I
		friend inet_stream& operator>>(inet_stream& sock, string& dest);

		ssize_t rcv(void* buf, size_t len, int flags);
		ssize_t rsvfrom(

	};

}