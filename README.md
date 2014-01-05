#README for libsocket

![Travis Build Status](https://api.travis-ci.org/dermesser/libsocket.png )

(Pre-built documentation may be found on [my
 server](http://lbo.spheniscida.de/doc/libsocket/doxygen/index.html))

## WHAT IS libsocket AND WHY SHOULD I USE IT?

libsocket is a library with a C part and a C++ part making sockets usage easy and clean.

Using the C part:

- Link against `libsocket.so`
- Functions combining more than one operation on sockets (e.g. create and connect TCP socket)
- Main principle: "One function to connect a socket, one to close it."

Using the C++ part:

- Link against `libsocket++.so`
- Classes representing the different socket types, e.g. TCP client sockets, UNIX DGRAM "server"
sockets
- Complex (almost complicated...) class hierarchy (`doc/libsocket++/classes.svg`) providing the
possibility of extensive and intensive code sharing.
- C++ish implementation (features include overloaded stream (`<<`, `>>`) operators, functions
accepting `std::string` objects and more-or-less STL use), so -> good integration in other
applications or libraries.
- Using C++11 features: The copy constructor of the `socket` base class is deleted; this enables the
destructor to close the socket when it leaves the visible scope. Some functions are internally using
`unique_ptr`s to enable safe deallocation.


##FEATURES AND ADVANTAGES

The libsocket library supports following things and protocols:

* IPv4 (client, server)
* IPv6 (client, server; if your machine supports it)
* TCP (client, server)
* UDP (client, server)
* UNIX Domain Sockets (DGRAM/STREAM server/client)
* Intelligent algorithms to get the best connection and no errors
* Easy use (one function call to get a socket up and running, another one to close it)
* Proper error processing (using `errno`, `gai_strerror()` etc.) and exception system.

One of the main advantages of libsocket is that you don't have to write the complex and error-prone
procedures for connecting a socket, check it for errors etc. yourself. Your networking programs
become shorter and better readable.

libsocket supports the important socket types: INET/INET6 with TCP and UDP; and UNIX DGRAM/STREAM.

Almost every function working with sockets is wrapped by libsocket, e.g.:
* sendto
* recvfrom
* accept
* socket/connect - one function
* socket/bind    - one function

libsocket is designed to not use a "proprietary" socket format (as libc does with its `FILE` type)
giving you the possibility to operate on the raw file descriptor with functions other than those provided by
libsocket.

##PLATFORMS

Libsocket works best on modern linux systems. It needs a C++11 compiler like g++ or clang++. Override the
default compiler using the flag `-DCMAKE_CXX_COMPILER=<compiler` or `-DCMAKE_C_COMPILER=<compiler>`.

Other than on Linux systems libsocket is known to work as well (although not really thoroughly tested) on
FreeBSD systems with working C++11 stack. The library has been tested on a FreeBSD 10.0-RC4 amd64 system
using the shipped compilers (which is clang 3.3).

libsocket does not work on OpenBSD yet because there are some source level incompatibilities.

If you're using libsocket successfully on other platforms, or ported it please let me know.

On both Linux and FreeBSD the usual CMake approach does work:

    cmake CMakeLists.txt
    make
    make install

##How to use the libsocket: static vs. dynamic

###Static Linkage

It's possible to compile libsocket statically into your program (by placing the .c[pp] and .h[pp]
files in your source tree). You don't have to mind legal issues because libsocket is licensed by
a slightly modified 2-clause BSD license which permits any use, as long as you include the license
text in your product (so it's clear that libsocket is licensed by this License) and the notice that
*we* wrote libsocket (as described in the license). It's ok to mention libsocket in your product's
Readme or advertisements anyway.

###Dynamic Linkage

The recommended method to use libsocket is to link your program against the libsocket SO (DLL). Using this
method is quite easy; you have to compile the dynamic libraries (libsocket and libsocket++) using
the Makefile (see section "BUILDING")

Linking your programs against the library is also simple: if $OBJECTS are your object files, then link
them together using one of these commands:

        $ gcc -o yourprog -lsocket $OBJECTS
        # or
        $ g++ -o yourprog -lsocket++ $OBJECTS

You only need to link against one library, even when using C++, because libsocket++ contains all
necessary functions.

If you distribute your program in binary form, it's possible to distribute the library binaries along with
your program and install them along your program.

##BUILDING

If you want to install both libsocket and libsocket++, simply use this command:

    $ cmake CMakeLists.txt
    # make install

This installs the SOs libsocket.so and libsocket++.so to /usr/lib/ and the header files to
/usr/include/libsocket. You may change these paths in the CMakeLists.txt file in the project root.

##EXAMPLES

You may test libsocket and make some experiences by playing with the examples provided in the
standard libsocket distribution in examples/ and examples++. More detailed descriptions can be found
in the source files. The collection of examples contain (among others):

(C)

* `http.c`: A simple http client
* `echo_reconnect.c`, `echo_srv.c`: Less echo server than simple transmit of text using INET UDP
sockets, but also showing the use of `reconnect_isocket()`
* `unix_stream_client.c`, `unix_stream_server.c`: Demonstrating UNIX STREAM sockets as echo
server/client
* `unix_dgram_client.c`, `unix_dgram_server.c`: Demonstrating UNIX DGRAM sockets as simple
server/client transmitting text.

(C++)

* `http.cpp`, `http_2.cpp`: Two simple HTTP clients using slightly different approaches
* `server.cpp`, `client.cpp`: TCP client and server
* `unix_client_dgram.cpp`: Writes a message to the syslog using UNIX DGRAM sockets
* `echo_server.cpp, echo_client_conn.cpp, echo_client_sndto.cpp`: UDP client/server (two clients:
        One using sendto(), another using connected datagram sockets)
* `unix_client_stream.cpp, unix_server_stream.cpp`: Client/Server using UNIX STREAM sockets.

You should have a look at the length of the code; while `http.c` is complete with 24 sloc (source
        lines of code) - the quite similar client simple-http
(https://github.com/dermesser/Simple-HTTP-client) uses almost 70 lines of code.

