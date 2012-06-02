# ifndef UNIXDGRAM
# define UNIXDGRAM

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
}

# endif
