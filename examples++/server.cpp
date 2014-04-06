# include <iostream>
# include <string>
# include "../headers/inetserverstream.hpp"
# include "../headers/exception.hpp"
# include <unistd.h>
# include <stdio.h>
# include <utility>
# include "../headers/socket.hpp"
# include "../headers/select.hpp"

/*
 * This server is a bit more complicated than
 * what you will need later. It uses the selectset class
 * to demonstrate how to use it.
 * When a client connects it first sends a message and
 * receives the client's answer afterwards.
 */

int main(void)
{
    using std::string;

    using libsocket::inet_stream_server;
    using libsocket::inet_stream;
    using libsocket::selectset;

    string host = "::1";
    string port = "1235";
    string answ;

    try {
	inet_stream_server srv(host,port,LIBSOCKET_IPv6);
	inet_stream* cl1;

	selectset<inet_stream_server> set1;
	set1.add_fd(srv,LIBSOCKET_READ);

	for ( ;; )
	{
	    /********* SELECT PART **********/
	    std::cout << "Called select()\n";

	    libsocket::selectset<inet_stream_server>::ready_socks readypair; // Create pair (libsocket::fd_struct is the return type of selectset::wait()

	    readypair = set1.wait(); // Wait for a connection and save the pair to the var

	    inet_stream_server* ready_srv = dynamic_cast<inet_stream_server*>(readypair.first.back()); // Get the last fd of the LIBSOCKET_READ vector (.first) of the pair and cast the socket* to inet_stream_server*

	    readypair.first.pop_back(); // delete the fd from the pair

	    std::cout << "Ready for accepting\n";

	    /*******************************/

	    cl1 = ready_srv->accept();

	    *cl1 << "Hello\n";

	    answ.resize(100);

	    *cl1 >> answ;

	    std::cout << answ;

	    cl1->destroy();
	}

	srv.destroy();

    } catch (libsocket::socket_exception exc)
    {
	std::cerr << exc.mesg << std::endl;
    }
    return 0;
}
