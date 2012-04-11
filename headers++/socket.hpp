# ifndef SOCKET_HEAD
# define SOCKET_HEAD
namespace libsocket
{
	class socket
	{
		protected:
		int sfd;

		public:

		virtual ~socket();

		virtual void destroy(void);
	};
}
#endif
