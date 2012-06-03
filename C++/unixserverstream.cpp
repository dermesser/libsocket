# include "../headers/libunixsocket.h"
# include "../headers++/exception.hpp"
# include "../headers++/unixbase.hpp"
# include "../headers++/unixclientstream.hpp"
# include <string>
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
	using std::string;

	class unix_stream_server : public unix_socket
	{
		private:
		bool listening;

		public:

		unix_stream_server(void);
		unix_stream_server(const char* path, int flags=0);
		unix_stream_server(const string& path, int flags=0);

		void setup(const char* path, int flags=0);

		unix_stream_client* accept(int flags=0);
	};

	unix_stream_server::unix_stream_server(void)
		: listening(false)
	{
	}

	unix_stream_server::unix_stream_server(const char* path, int flags)
	{
		setup(path,flags);
	}

	unix_stream_server::unix_stream_server(const string& path, int flags)
	{
		setup(path.c_str(),flags);
	}

	void unix_stream_server::setup(const char* path, int flags)
	{
		if ( path == NULL )
			throw socket_exception(__FILE__,__LINE__,"unix_stream_server::setup: Path is NULL!\n");
		if ( listening == true )
			throw socket_exception(__FILE__,__LINE__,"unix_stream_server::setup: Server socket already created!\n");

		sfd = create_unix_server_socket(path,STREAM,flags);

		if ( sfd < 0 )
			throw socket_exception(__FILE__,__LINE__,"unix_stream_server::setup: Error at creating UNIX stream server socket!\n");

		listening = true;
	}

	unix_stream_client* unix_stream_server::accept(int flags)
	{
		if (listening != true)
			throw socket_exception(__FILE__,__LINE__,"unix_stream_server::accept: Socket not set up yet!\n");

		unix_stream_client* client = new unix_stream_client;

		int cfd = accept_unix_stream_socket(sfd,flags);

		if ( cfd < 0 )
			throw socket_exception(__FILE__,__LINE__,"unix_stream_server::accept: Error at accepting new connection!\n");

		client->sfd = cfd;

		return client;
	}

}
