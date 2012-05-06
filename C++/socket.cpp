# include <unistd.h>

namespace libsocket
{
	class socket
	{
		protected:
		int sfd;

		public:

		socket();
		virtual ~socket();

	 	virtual int destroy(void);

		int getfd(void) const;
	};

	socket::socket() : sfd(-1) {}

	socket::~socket(void)
	{
		destroy();
	}

	int socket::destroy(void)
	{
		if ( 0 > close(sfd))
			return -1;

		sfd = -1;

		return 0;
	}

	int socket::getfd(void) const
	{
		return sfd;
	}
}
