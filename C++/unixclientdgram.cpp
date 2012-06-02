# include "../headers++/exception.hpp"
# include "../headers++/socket.hpp"
# include "../headers++/unixbase.hpp"
# include "../headers++/unixdgram.hpp"
# include "../headers++/dgramclient.hpp"
# include "../headers/libunixsocket.h"
# include <string>

using std::string;

/*
The committers of the libsocket project, all rights reserved
(c) 2012, dermesser <lbo@spheniscida.de>

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
following conditions are met:

	1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
	disclaimer.
	2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
	disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

*/

namespace libsocket
{
	class unix_dgram_client : public unix_dgram, public dgram_client_socket
	{
		private:
		bool connected;

		public:

		unix_dgram_client(int flags=0);
		unix_dgram_client(const char* path, int flags=0);

# if __cplusplus == 201103L
		unix_dgram_client(const string& path, int flags=0);
# endif
		void connect(const char* path);
		void connect(const string& path);

		void deconnect(void);
	};

	unix_dgram_client::unix_dgram_client(int flags)
	{
		sfd = create_unix_dgram_socket(0,flags);

		if ( sfd < 0 )
			throw socket_exception(__FILE__,__LINE__,"unix_dgram_client::unix_dgram_client: Could not create unix dgram client socket!\n");

	}

	unix_dgram_client::unix_dgram_client(const char* path, int flags)
	{
		sfd = create_unix_dgram_socket(path,flags);

		if ( sfd < 0 )
			throw socket_exception(__FILE__,__LINE__,"unix_dgram_client::unix_dgram_client: Could not create unix dgram client socket!\n");
	}

	// Delegating constructor
# if __cplusplus == 201103L
	unix_dgram_client(const string& path, int flags)
		: unix_dgram_client(path.c_str(),flags) {}
# endif

	void unix_dgram_client::connect(const char* path)
	{
		if ( connect_unix_dgram_socket(sfd,path) < 0 )
			throw socket_exception(__FILE__,__LINE__,"unix_dgram_client::connect: Could not connect dgram socket!\n");
	}

	void unix_dgram_client::connect(const string& path)
	{
		connect(path.c_str());
	}

	void unix_dgram_client::deconnect(void)
	{
		if ( connect_unix_dgram_socket(sfd,0) < 0 )
			throw socket_exception(__FILE__,__LINE__,"unix_dgram_client::deconnect: Could not disconnect dgram socket!\n");
	}
}
