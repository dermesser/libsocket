#README for libsocket

## WHAT IS libsocket AND WHY SHOULD I USE IT?

libsocket is a library with separated C and C++ parts making sockets easy (and object-oriented, if you develop in C++).

Using the C part:

- Link against `libsocket.so`
- Functions combining more than one operation on sockets (e.g. create and connect TCP socket)
- Main principle: "One function to connect a socket, one to close it."

Using the C++ part:

- Link against `libsocket++.so`
- Classes representing the different socket types, e.g. TCP client sockets, UNIX DGRAM "server" sockets
- Complex (almost complicated...) class hierarchy (`doc/libsocket++/classes.svg`) providing the possibility of extensive and intensive code sharing.
- C++ish implementation (features include overloaded stream (`<<`, `>>`) operators, functions accepting `std::string` objects and more-or-less STL use), so -> good integration in other applications or libraries.


##FEATURES AND ADVANTAGES

The libsocket library supports following things and protocols:

* IPv4 (client, server)
* IPv6 (client, server; if your machine supports it)
* TCP (client, server)
* UDP (client, server)
* UNIX Domain Sockets (DGRAM&STREAM server/client), only for Linux
* Intelligent algorithms to get the best connection and no errors
* Easy use (one function call to get a socket up and running, one another to close it)
* Proper error processing (using `errno`, `gai\_strerror()` etc.).

One of the main advantages of libsocket is that you don't have to write the complex and error-prone
procedures for connecting a socket, check it for errors etc. yourself. Your network programs
get shorter and better readable.

libsocket supports almost every socket types. During there is no useful documentation, take a look
at the header files in headers/. Most of the functions are self-explaining.

Almost every syscall is wrapped by libsocket, e.g.:
	- sendto
	- recvfrom
	- accept
	- socket/connect - one function
	- socket/bind    - one function

If you're wondering about the UNIX DGRAM functions: Yes, `create_unix_dgram_socket("path")` and `create_unix_server_socket("path",DGRAM,...)`
produces the same result. DGRAM sockets are alway server and client at the same time. The difference is good to distinguish
semantically between the sockets in your code.

If you want to have even more control over the sockets you're using, this library is not the right tool for you. It's intended for the easy
use of sockets.

For detailed documentation read DOCUMENTATION.md resp. DOCUMENTATION.html


##PLATFORMS

libsocket is developed on Linux 3.x with gcc, but every file was successfully tested with clang (from llvm 3.0, 3.1) and works with it.

libsocket works, both statically and dynamically linked, on these platform combinations:

- GCC 4.7, GNU/Linux (tested on Debian and Fedora)
- GCC 4.6, GNU/Linux (tested on Debian)
- clang/clang++ 3.0, GNU/Linux (tested on Debian)
- clang/clang++ 3.1, GNU/Linux (tested on Debian)
- gcc version 4.2.1 20070831 patched, FreeBSD 9.0
- clang/clang++ 3.1 portbld, FreeBSD 9.0

libsocket does not work (AFAIK) on OpenBSD, because there are some source level incompatibilities.

The inet server part does not work properly on FreeBSD; calls to `getnameinfo()` fail with `Non-recoverable failure in name resolution` or do not return a correct name. A workaround or reason for this behavior is not known.
This makes the use of C++ inet server parts impossible, excepted the situation when using a patched version which does not use `getnameinfo()`.

##LICENSE and USE

###Static Linkage

It's possible to compile libsocket statically into your program (by placing the .c[pp] and .h[pp] files in your source tree).
You don't have to mind legal issues because libsocket is licensed by a slightly modified 2-clause BSD license which permits
any use, as long as you include the license text in your product (so it's clear that libsocket is licensed by this License)
and the notice that *we* wrote libsocket (as described in the license)
It's nice to mention libsocket in your product's Readme or advertisements anyway, of course :)

###Dynamic Linkage

The recommended method to use libsocket is to link your program against the libsocket SO.
Using this method is quite easy; you have to compile the dynamic libraries (libsocket and libsocket++)
using the Makefile (see section "BUILDING")

Linking your programs against the library is also easy; if $OBJECTS are your object files, then link them together using one of these
commands:

	$ gcc -o yourprog -lsocket $OBJECTS
	$ g++ -o yourprog -lsocket++ $OBJECTS

You only need to link against one library, even when using C++, because libsocket++ contains all necessary functions.

If you distribute your program in binary form, it's possible to distribute library binaries along your program and install them
along your program.

###Other stuff

You may use other compilers than the GNU Compilers ($(CC), $(CPP)). The dynamic and static linking was tested successfully using
clang/clang++ 3.0 and 3.1 on GNU/Linux and clang 3.1 on FreeBSD (and, of course, gcc on both platforms).

##BUILDING

If you want to install both libsocket and libsocket++, simply use this command:
	# make install

This installs the SOs libsocket.so and libsocket++.so to /usr/lib/ and the header files to
/usr/include/libsocket. If you want to change this path, use this command:

	# LIBPATH=/path/to/lib/ HEADERPATH=/path/to/headers/libsocket make -e install

This command installs the libs to $LIBPATH and the header files to $HEADERPATH.

If you do not like libsocket anymore, remove it using

	# make deinstall
If you specified non-default library or header paths, specify them again and use '-e'.

##EXAMPLES

You may test libsocket and make some experiences by playing with the examples provided in the standard libsocket distribution in examples/ and examples++. More detailed descriptions can be found in the source files.
The collection of examples contain (among other):

(C)

* `http.c`: A simple http client
* `echo_reconnect.c`, `echo_srv.c`: Less echo server than simple transmit of text using INET UDP sockets, but also showing the use of `reconnect_isocket()`
* `unix_stream_client.c`, `unix_stream_server.c`: Demonstrating UNIX STREAM sockets as echo server/client
* `unix_dgram_client.c`, `unix_dgram_server.c`: Demonstrating UNIX DGRAM sockets as simple server/client transmitting text.

(C++)

* `http.cpp`, http_2.cpp: Two simple HTTP clients using slightly different approaches
* `server.cpp`, `client.cpp`: TCP client and server
* `unix_client_dgram.cpp`: Writes a message to the syslog using UNIX DGRAM sockets
* `echo_server.cpp, echo_client_conn.cpp, echo_client_sndto.cpp`: UDP client/server (two clients: One using sendto(), another using connected datagram sockets)
* `unix_client_stream.cpp, unix_server_stream.cpp`: Client/Server using UNIX STREAM sockets.

You should have a look at the length of the code; while `http.c` is complete with 24 sloc (source lines of code) - the quite similar client
simple-http (https://github.com/dermesser/Simple-HTTP-client) uses almost 70 lines of code.
