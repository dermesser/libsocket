# ifndef INETSERVERSOCKET
# define INETSERVERSOCKET

namespace libsocket
{
	class inet_stream_server : public inet_socket
	{
		private:

		public:

		inet_stream_server(void);
		inet_stream_server(const char* bindhost, const char* bindport, int proto_osi3, int flags=0);

		void setup(const char* bindhost, const char* bindport, int proto_osi3, int flags=0);

		inet_stream accept(char* src_host, char* src_host_len, char* src_port, char* src_port_len, int numeric=0);
	};
}

# endif
