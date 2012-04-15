# ifndef INETSOCKET_HEAD
# define INETSOCKET_HEAD

# include <string>
# include <sys/types.h>
# include <sys/socket.h>
# include "socket.hpp"
# include "inetbase.hpp"

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

}
# endif
