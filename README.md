#README for libsocket

##LICENSE

It's recommended to compile libsocket statically into your program (by placing the .c and .h files in your source tree).
You don't have to mind legal issues because libsocket is licensed by a slightly modified 2-clause BSD license which permits
any use, as long as you include the license text in your product (so it's clear that libsocket is licensed by this License)
and the notice that *we* wrote libsocket (as described in the license)
It's friendly to mention libsocket in your product's Readme or advertisements anyway, of course :)

##USE

libsocket is built for little programs whose author(s) are too lazy to build own algorithms and functions for socket (UNIX
and Internet Domain (TCP/UDP)) communication.

As you should know as experienced C programmer, it's quite easy to use this library.
Simply add the line into the source files where the library is used:

	# include "path/to/lib{inet,unix}socket.h"

and call the compiler (eventually in a makefile) with the name of the C file:

	$ gcc lib{inet,unix}socket.c ownfile1.c ownfile2.c
	# or
	$ gcc -c libsocket.c
	$ gcc ownfile1.c ownfile2.c libsocket.o
	# etc. pp.

##FEATURES AND ADVANTAGES

The libsocket library supports following things and protocols:

* IPv4 (client, server)
* IPv6 (client, server; if your machine supports it)
* TCP (client, server)
* UDP (client, server)
* UNIX Domain Sockets (DGRAM&STREAM server/client)
* Intelligent algorithms to get the best connection and no errors
* Easy use (one function call to get a socket up and running, one another to close it)
* Proper error processing (using errno, gai\_strerror() etc.).

One of the main advantages of libsocket is that you don't have to write the complex and error-prone
procedures for connecting a socket, check it on errors etc. yourself.

But libsocket does not support every use of sockets. For example, there is no such function like `sendto()` because the developer
thinks that connected UDP sockets are easier to handle and equal. The combination of connected DGRAM (DGRAM, UDP) sockets and the function
`reconnect_isocket()` resp. `reconnect_usocket()` provides an interface which is also quite easy. On the other side, some functions support
the use of 'raw' flags which are passed to the underlying Sockets API functions (e.g. `create_isocket()` which accepts flags like `SOCK_NONBLOCK`).
I also had the design goal that the library should use the real sockets accepted by read()/write() syscalls etc and not structures (like FILE
pointers used by libc) so you may control sockets nevertheless with `setsockopt()` or receive datagrams with `recvfrom()` although you have
to handle the `struct sockaddr`s yourself. Another example for this design goal is that there is no function equivalent to `read()` or `write()`
(except of `recv_ussocket()` which does no more than `read()`)

If you want to have more control over the sockets you're using, this library is not the right tool for you. It's intended for the easy
use of sockets.

For detailed documentation read DOCUMENTATION.md resp. DOCUMENTATION.html

##EXAMPLES

You may test libsocket and make some experiences by playing with the examples provided in the standard libsocket distribution
in examples/.
The collection of examples contain:

* http.c: A simple http client
* echo\_reconnect.c, echo\_srv.c: Less echo server than simple transmit of text using INET UDP sockets, but also showing the use of `reconnect_isocket()`
* unix\_stream\_client.c, unix\_stream\_server.c: Demonstrating UNIX STREAM sockets as echo server/client
* unix\_dgram\_client.c, unix\_dgram\_server.c: Demonstrating UNIX DGRAM sockets as simple server/client transmitting text.

You should have a look at the length of the code; while `http.c` is complete with 24 sloc (source lines of code) - the quite similar client 
simple-http (https://github.com/dermesser/Simple-HTTP-client) uses almost 70 lines of code.
