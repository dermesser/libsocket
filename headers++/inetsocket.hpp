# ifndef INETSOCKET_HEAD
# define INETSOCKET_HEAD

# include <string>
# include "../headers/libinetsocket.h"
# include "socket.hpp"

# define TCP 1
# define UDP 2

# define IPv4 3
# define IPv6 4

# define BOTH 5 // what fits best (TCP/UDP or IPv4/6)

# define READ 1
# define WRITE 2

# define NUMERIC 1

// Quite simple oo wrapper around libinetsocket

namespace libsocket
{
	using std::string;

	struct inet_exception
	{
		string mesg;

		inet_exception(string,int,string);
	};

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
}
# endif
