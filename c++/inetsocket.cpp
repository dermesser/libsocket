# include <iostream>
# include <string>
# include <string.h>
# include "../headers/libinetsocket.h"
# include "socket.hpp"
# include <unistd.h>
# include <sys/socket.h>
# include <sys/types.h>

// Quite simple oo wrapper around libinetsocket

namespace libsocket
{
	using std::string;

	struct inet_exception
	{
		string mesg;

		inet_exception(string);
	};

	inet_exception::inet_exception(string m) : mesg(m) {}

// // // // // // //

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

	class inet_stream : public inet_socket
	{
		private:

		public:

		inet_stream(void);
		inet_stream(const char* host, const char* port, int proto_osi3, int flags);
		~inet_stream();

		// Real actions
		int connect(const char* host, const char* port, int proto_osi3, int flags);
		int shutdown(int method);
		void try_to_destroy(void);
		int destroy(void);

		// I/O
		friend inet_stream& operator<<(inet_stream& sock, const char* str);
		friend inet_stream& operator>>(inet_stream& sock, string& dest);

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
		if ( 0 > connect(host,port,proto_osi3,flags) )
			throw inet_exception("Could not create socket");
	}

	inet_stream::~inet_stream(void)
	{
		try_to_destroy();
	}

	int inet_stream::connect(const char* host, const char* port, int proto_osi3, int flags)
	{
		if ( sfd != -1 ) // Socket is already connected
			throw inet_exception("Already connected!\n");

		sfd = create_inet_stream_socket(host,port,proto_osi3,flags);

		if ( sfd < 0 )
			throw inet_exception("Could not create socket\n");

		remote_host = host;
		remote_port = port;

		return 0;
	}


	int inet_stream::shutdown(int method)
	{
		if ( 0 > shutdown_inet_stream_socket(sfd,method))
		{
			throw inet_exception("Could not shutdown socket\n");
		}

		return 0;
	}

	void inet_stream::try_to_destroy(void)
	{
		close(sfd);
		sfd = -1;
	}

	int inet_stream::destroy(void)
	{
		if ( 0 > destroy_inet_socket(sfd) )
			throw inet_exception("Could not close socket\n");
		
		sfd = -1;
		
		return 0;
	}

	// I/O
	
	inet_stream& operator<<(inet_stream& sock, const char* str)
	{
		if ( sock.sfd == -1 )
			throw inet_exception("Socket not connected!\n");
		if ( str == NULL )
			throw inet_exception("Null buffer given!\n");

		size_t len = strlen(str);

		if ( -1 == write(sock.sfd,str,len) )
			throw inet_exception("Write failed!\n");

		return sock;
	}

	inet_stream& operator>>(inet_stream& sock, string& dest)
	{
		ssize_t read_bytes;
		char* buffer; 
		
		buffer = new char[dest.size()];

		if ( sock.sfd == -1 )
			throw inet_exception("Socket not connected!\n");

		if ( -1 == (read_bytes = read(sock.sfd,buffer,dest.size())) )
			throw inet_exception("Error while reading!\n");

		if ( read_bytes < static_cast<ssize_t>(dest.size()) )
			dest.resize(read_bytes); // So the client doesn't print content more than one time
						 // and it can check if the string's length is 0 (end of transmission)

		dest.assign(buffer,read_bytes);

		delete buffer;

		return sock;
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
}
