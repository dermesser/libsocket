# README for libsocket

[![Travis Build Status](https://api.travis-ci.org/dermesser/libsocket.png)](https://travis-ci.org/dermesser/libsocket/)

(Pre-built documentation for libsocket developers can be found on [my
 server](http://lbo.spheniscida.de/doc/libsocket/doxygen/html/))

## BUILDING libsocket

If you want to install both libsocket and libsocket++, simply use this command:

    $ cmake CMakeLists.txt
    $ make
    # make install

This installs the SOs libsocket.so and libsocket++.so to /usr/lib/ and the header files to
/usr/include/libsocket. You may change these paths in the CMakeLists.txt file in the project root.

Note the changed library name on SunOS.

CMake is required to support object libraries, which is the case in versions higher than or equal
to 2.8.

## WHAT IS libsocket AND WHY SHOULD I USE IT?

libsocket is a library with a C part and a C++ part making sockets usage easy and clean.

[Using the C part:](doc/libsocket/DOCUMENTATION.mkd)

- Link against `libsocket.so`
- Functions combining more than one operation on sockets (e.g. create and connect TCP socket)
- Main principle: "One function to connect a socket, one to close it."

[Using the C++ part:](doc/libsocket++/DOCUMENTATION++.mkd)

- Link against `libsocket++.so`
- Classes representing the different socket types, e.g. TCP client sockets, UNIX DGRAM "server"
sockets
- Complex (almost complicated...) class hierarchy (`doc/libsocket++/classes.svg`) providing the
possibility of extensive and intensive code sharing.
- C++ish implementation (features include overloaded stream (`<<`, `>>`) operators, functions
accepting `std::string` objects and more-or-less STL use), so -> good integration in other
applications or libraries.
- Using C++11 features: The copy constructor of the `socket` base class is deleted; this enables the
destructor to safely close the socket when it leaves the visible scope. Some functions are internally using
`unique_ptr`s to enable safe deallocation.

## FEATURES AND ADVANTAGES

The libsocket library supports following things and protocols:

* IPv4 (client, server)
* IPv6 (client, server; if your machine supports it)
* TCP (client, server)
* UDP (client, server -- the difference is that client sockets may be connected to an endpoint)
* UNIX Domain Sockets (DGRAM/STREAM server/client)
* IPv4/IPv6 multicast (only in C)
* Abstraction classes for `select(2)` and `epoll(7)` (C++)
* Easy use (one function call to get a socket up and running, another one to close it)
* RAII, no-copy classes -- resource leaks are hard to do.
* Proper error processing (using `errno`, `gai_strerror()` etc.) and C++ exceptions.

One of the main advantages of libsocket is that you don't have to write the complex and error-prone
procedures for connecting a socket, checking it for errors etc. yourself. Your networking programs
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

## PLATFORMS

### GNU/Linux

Libsocket works best on modern linux systems. It needs a C++11 compiler like g++ or clang++. Override the
default compiler using the flag `-DCMAKE_CXX_COMPILER=<compiler>` or `-DCMAKE_C_COMPILER=<compiler>`.

### FreeBSD

Other than on Linux systems libsocket is known to work as well (although not really thoroughly tested) on
FreeBSD systems with working C++11 stack. The library has been tested on a FreeBSD 10.0-RC4 amd64 system
using the shipped compilers (which is clang 3.3).

### SunOS: OpenIndiana, (Solaris?)

The library part written in C works (partly) also on OpenIndiana; this has been verified using
`SunOS openindiana 5.11 oi_151a8`.

Because a modern C++ compiler was not available at the time of testing, the C++ library part is not
built on SunOS systems.

Another difficulty is the existence of the system library `libsocket`; therefore the C library is renamed
to libsocket\_hl on SunOS. You have to link your programs using the flag `-lsocket_hl`, not `-lsocket`.

#### SunOS limitations

* The UDP server example (`examples/echo_dgram_server.c`) refuses to create a socket. The error is
    "Operation not supported on transport endpoint".
* The TCP server example (`examples/transmission_server.c`) also fails when trying to create the socket.
    Here, the error displayed is "Invalid argument". I'm quite sure that these issues can be fixed with
    a little investigation and knowledge of SunOS.

### OpenBSD
libsocket does not work on OpenBSD yet because there are some more fundamental source level incompatibilities
than those between Linux and FreeBSD/OpenIndiana-SunOS.

### Other OSs
If you're using libsocket successfully on other platforms (or even ported it), please let me know.

## How to use the libsocket: static vs. dynamic

### Static Linkage

It's possible to link libsocket statically into your program (by placing the .c[pp] and .h[pp]
files in your source tree or linking against a `.a` file). You don't have to mind legal issues because libsocket is licensed by
a slightly modified 2-clause BSD license which permits any use, as long as you include the license
text in your product (so it's clear that libsocket is licensed by this License) and the notice that
*we* wrote libsocket (as described in the license). It's ok to mention libsocket in your product's
Readme or advertisements anyway.

### Dynamic Linkage

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

## EXAMPLES

You may test libsocket and make some experiences by playing with the examples provided in the
standard libsocket distribution in examples/ and examples++. More detailed descriptions can be found
in the source files. The collection of examples contain (among others):

(C)

* `http.c`: A simple http client
* `echo_dgram_server.c`, `echo_dgram_client.c`, `echo_dgram_connect_client.c`: Shows how to use
UDP sockets, both in connected and unconnected mode.
* `unix_stream_client.c`, `unix_stream_server.c`: Demonstrating UNIX STREAM sockets as echo
server/client
* `unix_dgram_client.c`, `unix_dgram_server.c`: Demonstrating UNIX DGRAM sockets as simple
server/client transmitting text.
* `multicast-listen.c`: Demonstrating how to use libinetsocket for multicast networking.

Build these with `gcc -o <outfile> -lsocket <example-name>`.

(C++)

* `http.cpp`, `http_2.cpp`: Two simple HTTP clients using slightly different approaches
* `server.cpp`, `client.cpp`: TCP client and server
* `unix_client_dgram.cpp`: Writes a message to the syslog using UNIX DGRAM sockets
* `echo_server.cpp, echo_client_conn.cpp, echo_client_sndto.cpp`: UDP client/server (two clients:
        One using sendto(), another using connected datagram sockets)
* `unix_client_stream.cpp, unix_server_stream.cpp`: Client/Server using UNIX STREAM sockets.

Build these with `[clan]g++ -std=c++11 -lsocket++ -o <outfile> <example-name>`.

You should have a look at the length of the code; while `http.c` is complete with 24 sloc (source
        lines of code) - the quite similar client simple-http
(https://github.com/dermesser/Simple-HTTP-client) uses almost 70 lines of code.

## TODO



