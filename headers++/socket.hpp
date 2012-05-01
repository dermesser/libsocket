# ifndef SOCKET
# define SOCKET

# include <sys/socket.h>
# include <sys/types.h>

namespace libsocket
{
	class socket
	{
		protected:
		int sfd;

		public:

		virtual ~socket();

		virtual void destroy(void);

		int getfd(void) const;
	};
}
#endif
