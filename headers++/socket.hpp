# ifndef SOCKET
# define SOCKET
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
