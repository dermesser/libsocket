# ifndef UNIXCLIENTSTREAM
# define UNIXCLIENTSTREAM

# define READ  1
# define WRITE 2

namespace libsocket
{
	using std::string;

	class unix_stream_client : public unix_socket
	{
		private:
		string peer_path;

		public:

		unix_stream_client(void);
		unix_stream_client(const char* path, int socket_flags=0);
		unix_stream_client(const string& path, int socket_flags=0);

		void connect(const char* path, int socket_flags=0);
		void connect(const string& path, int socket_flags=0);

		void shutdown(int method=WRITE);

		ssize_t snd(const void* buf, size_t buflen, int send_flags=0);
		ssize_t rcv(void* buf, size_t len, int recv_flags=0);

		friend unix_stream_client& operator<<(unix_stream_client& sock,const char* data);
		friend unix_stream_client& operator<<(unix_stream_client& sock,string& data);
	}
}
# endif
