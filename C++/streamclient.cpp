# include "../headers++/socket.hpp"
# include <string>
# include "../headers++/exception.hpp"
# include <unistd.h>
# include <string.h>

namespace libsocket
{
	class stream_client_socket : public virtual socket
	{
		public:

		ssize_t snd(const void* buf, size_t len, int flags=0); // flags: send()
		ssize_t rcv(void* buf, size_t len, int flags=0); // flags: recv()
		
		friend stream_client_socket& operator<<(stream_client_socket& sock, const char* str);
		friend stream_client_socket& operator<<(stream_client_socket& sock, string& str);
		friend stream_client_socket& operator>>(stream_client_socket& sock, string& dest);
	};

	ssize_t stream_client_socket::rcv(void* buf, size_t len, int flags)
	{
		ssize_t recvd;

		if ( sfd == -1 )
			throw socket_exception(__FILE__,__LINE__,"stream_client_socket::rcv() - Socket is not connected!\n");

		if ( buf == NULL || len == 0 )
			throw socket_exception(__FILE__,__LINE__,"stream_client_socket::rcv() - Buffer or length is null!\n");

		if ( -1 == (recvd = recv(sfd,buf,len,flags)) )
			throw socket_exception(__FILE__,__LINE__,"stream_client_socket::rcv() - Error while reading!\n");

		return recvd;
	}

	stream_client_socket& operator>>(stream_client_socket& sock, string& dest)
	{
		ssize_t read_bytes;
		char* buffer;

		buffer = new char[dest.size()];

		if ( sock.sfd == -1 )
			throw socket_exception(__FILE__,__LINE__,">>(std::string) input: Socket not connected!\n");

		if ( -1 == (read_bytes = read(sock.sfd,buffer,dest.size())) )
			throw socket_exception(__FILE__,__LINE__,">>(std::string) input: Error while reading!\n");

		if ( read_bytes < static_cast<ssize_t>(dest.size()) )
			dest.resize(read_bytes); // So the client doesn't print content more than one time
						 // and it can check if the string's length is 0 (end of transmission)

		dest.assign(buffer,read_bytes);

		delete buffer;

		return sock;
	}

	// O

	stream_client_socket& operator<<(stream_client_socket& sock, const char* str)
	{
		if ( sock.sfd == -1 )
			throw socket_exception(__FILE__,__LINE__,"<<(const char*) output: Socket not connected!\n");
		if ( str == NULL )
			throw socket_exception(__FILE__,__LINE__,"<<(const char*) output: Null buffer given!\n");

		size_t len = strlen(str);

		if ( -1 == write(sock.sfd,str,len) )
			throw socket_exception(__FILE__,__LINE__,"<<(const char*) output: Write failed!\n");

		return sock;
	}

	stream_client_socket& operator<<(stream_client_socket& sock, string& str)
	{
		if ( sock.sfd == -1 )
			throw socket_exception(__FILE__,__LINE__,"<<(std::string) output: Socket not connected!\n");

		if ( -1 == write(sock.sfd,str.c_str(),str.size()) )
			throw socket_exception(__FILE__,__LINE__,"<<(std::string) output: Write failed!\n");

		return sock;
	}

	ssize_t stream_client_socket::snd(const void* buf, size_t len, int flags)
	{
		ssize_t snd_bytes;

		if ( sfd == -1 )
			throw socket_exception(__FILE__,__LINE__,"stream_client_socket::snd() - Socket not connected!\n");
		if ( buf == NULL || len == 0 )
			throw socket_exception(__FILE__,__LINE__,"stream_client_socket::snd() - Buffer or length is null!\n");

		if ( -1 == (snd_bytes = send(sfd,buf,len,flags)) )
			throw socket_exception(__FILE__,__LINE__,"stream_client_socket::snd() - Error while sending\n");

		return snd_bytes;
	}
}
