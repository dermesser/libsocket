# DOCUMENTATION for libsocket++

## Introduction and Information for Devs

libsocket++ is a object-oriented wrapper around libsocket. To avoid writing the code twice
and maintain it twice, libsocket++ calls the functions from libsocket.

### Practical Usage

libsocket++ uses the namespace `libsocket`, so you either have to construct the classes as this:

	libsocket::inet_stream sock;

or use some things or the whole namespace:

	using namespace libsocket;

(for the internet TCP client socket class)

	using libsocket::inet_stream;

## Usage in Your Application

Compile the files (like mentioned in DOCUMENTATION.md) like every other:

	$ g++ -c */*.cpp
	$ g++ *.o
or
	$ g++ */*.cpp

Please note that libsocket++ is only an object-oriented wrapper around libsocket and not independant; you always have to compile
C/libinetsocket.c, too.

To reduce the size of the executables, it is recommended to compile your code only with that library files which are really necessary.
For example, if you have a program which serves as client for a UDP based application:

	$ g++ -o client client.cpp /path/to/libsocket/C/libinetsocket.c /path/to/libsocket/C++/{socket,inetbase,inetdgram,inetclientdgram}.cpp

## Class Hierarchy and names

The class hierarchy is very complex. You may take a look at it by viewing `classes.svg` in this directory. 

To understand the library, you also have to understand the names.

The classes have names which are built like this: `<domain>_<protocol>_<role>`: E.g. `unix_stream_server` or `inet_dgram_client`.
One exception: The TCP internet client class is called `inet_stream` (for historical reasons; in addition, every library needs an
inconsistence ;)

Among this classes, there are many other classes. In the diagram, this classes are the white boxes. It makes no sense to instantiate
objects from this classes although it's possible (they aren't abstract).

## Exception Handling
Defined in `exception.cpp`, has to be included from `exception.hpp`

	struct socket_exception
	{
		std::string mesg;

		socket_exception(std::string,int,string);
	};

An instantiation of this object is thrown in case of error. Almost every function
may raise an exception containing a std::string looking like this:

	../C++/inetclientstream.cpp:167: <<(std::string) output: Socket not connected!
		(1)	  	    (2)		(3)		     (4)

It contains information about the file (1), the line (2), the function throwing the exception (3)
and information about the cause for the exception (4) - here, the socket was already closed when calling
the .destroy() routine.

Example for error handling:

	try {
		sock << "test";
	} catch (libsocket::socket_exception exc)
	{
		std::cerr << exc.mesg;
	}

# libinetsocket++

libinetsocket++ is the wrapper around libinetsocket.


## `inet_stream` Class: Internet TCP Client Stream Sockets
### Constructors
Declared in `inetclientstream.hpp`, defined in `inetclientstream.cpp`

Please note that the name is _not_ `inet_stream_client`!

	1: inet_stream(void);
	2: inet_stream(const char* host, const char* port, int proto_osi3, int flags=0);
	3: inet_stream(const string& dsthost, const string& dstport, int proto_osi3, int flags=0);

1: Only initializes the most important things. The socket remains unconnected
and _must_ be connected before use using `connect()`.

2,3: The constructor initializes the socket and connects it with the given host:

- `host`: Destination host; if you have the host as `std::string`, use its routine `std::string::c_str()` to get
the C string
- `port`: Destination port (TCP, of course); conversion from string like above.
- `proto_osi3`: `IPv4`, `IPv6` or `BOTH` (`BOTH` lets the library choose; pp macros; defined in header file `inetsocket.hpp`)
- `flags`: Default 0, can be `SOCK_NONBLOCK` or `SOCK_CLOEXEC` (see: `socket(2)`; on other platforms than Linux
has to be 0 to avoid errors)

3: This constructor is only available when using a C++11-compliant compiler; it delegates the constructor. Otherwise, you have
to use constructor 2.

### `connect()`
Declared in `inetclientstream.hpp`, defined in `inetclientstream.cpp`

	void connect(const char* host, const char* port, int proto_osi3, int flags=0);

Connects the socket. Throws an exception if the socket is already connected.

- `host`: Destination host; if you have the host as `std::string`, use its routine `std::string::c_str()` to get
the C string
- `port`: Destination port (TCP, of course); conversion from string like above.
- `proto_osi3`: `IPv4` or `IPv6` (pp macros; defined in header file `inetsocket.hpp`)
- `flags`: Default 0, can be `SOCK_NONBLOCK` or `SOCK_CLOEXEC` (see: `socket(2)`; on other platforms than Linux
has to be 0 to avoid errors)

### `shutdown()`
Declared in `inetclientstream.hpp`, defined in `inetclientstream.cpp`

	void shutdown(int method);

Shuts the socket down (`shutdown(2)`).

`method` is `READ`, `WRITE`, or the ORed combination, `READ|WRITE`. `READ` and `WRITE` are defined
in `inetsocket.hpp`

### Destroy Functions
Declared in `socket.hpp`, defined in `socket.cpp`

	int destroy(void);

Close the socket and destroy the connection.

Return value 0 if successfull, otherwise -1.

### Output/Upload Functions
Declared in `inetclientstream.hpp`, defined in `inetclientstream.cpp`

	ssize_t snd(const void* buf, size_t len, int flags=0);

Conventional send function: Send the content of `buf`, which is `len` bytes long, to
the connected host. `flags` *may* be specified using the values allowed on your platform.
The flags available may be found in `send(2)` (the flags beginning with with `MSG_`)
Returns the number of sent bytes or throws an exception if an error occurred.

	friend inet_stream& operator<<(inet_stream& sock, const char* str);
	friend inet_stream& operator<<(inet_stream& sock, string& str);

Now, it gets interesting: The class `inet_stream` imitates the behaviour of
the standard C++ streams (`ostream`, `ofstream` etc.). You may write to the
connected socket using the overloaded bitshift operator. It is overloaded for
C strings (`const char*`, you may also use `char*`) and C++ strings (`std::string`).

As you can see at the return value, you may cascade it (from `examples++/http.c`):

	sock << request1 << request2;

Throws exceptions e.g. if the socket is not connected or `write(2)` returned -1.

### Input/Download Functions
Declared in `inetclientstream.hpp`, defined in `inetclientstream.cpp`

	ssize_t rcv(void* buf, size_t len, int flags=0);

Conventional receive function: Receive `len` bytes from socket and write them to `buf`.
`flags` may be specified and may take the flags specified in `recv(2)` (those beginning with `MSG_`)

	friend inet_stream& operator>>(inet_stream& sock, std::string& dest);

Stream-like read from socket: Reads at most `dest.size()` bytes from socket and puts them
to the string. If less than `dest.size()` characters could be read, the string is resized to
the number of read characters so you can check (`string.size() == 0`) if the server is done
with sending - either closed the socket on his side or shut it down for write access.

### Getters
Declared in `inetbase.hpp`, defined in `inetbase.cpp`

	std::string gethost(void) const;
	std::string getport(void) const;

`gethost()` returns a C++ std::string containing the host to which the socket is connected.

`getport()` returns a C++ std::string containing the port/service to which the socket is connected.

## `inet_stream_server` - TCP Internet server
Declared in `inetserverstream.hpp`, defined in `inetserverstream.cpp`

	inet_stream_server(void);
	inet_stream_server(const char* bindhost, const char* bindport, int proto_osi3, int flags=0);

Create an internet tcp server (passive) socket.

If you use the first constructor, you have to `setup()` the socket before using it. The latter constructor
binds the socket to `bindhost:bindport` using `proto_osi3` (`IPv4` or `IPv6` or `BOTH`).

`flags` are supplied to the internal `socket(2)` call.

### `accept()`
Declared in `inetserverstream.hpp`, defined in `inetserverstream.cpp`

	inet_stream* accept(int numeric=0);

Accept an incoming connection. `numeric` may be `NUMERIC` if you want to have the host and port as numbers.

Returns a pointer to a dynamically allocated `inet_stream` object

### Destroy
Declared in `socket.hpp`, defined in `socket.cpp`

	int destroy(void);

Inherited from `socket` class. Destroy (close) socket.

Return value 0 if successful, otherwise -1.

You don't have to destroy the sockets explicitly. The sockets are also destroyed when
the destructor of `socket` (virtual, of course) is called.

## `inet_dgram_client` Class: Internet UDP Sockets
### Constructors
Declared in `inetclientdgram.hpp`, defined in `inetclientdgram.cpp`

	inet_dgram_client(int proto_osi3, int flags=0); // Flags: socket()
	inet_dgram_client(const char* host, const char* port, int proto_osi3, int flags=0); // Flags: socket()

Because the UDP socket can be connected multiple times and send data to various hosts,
it's mandatory to specify the address family at instantiation time. `proto_osi3` may be
`IPv4` or `IPv6`; this information is used to create a socket with `create_inet_dgram_client_socket()`
and finally `socket(2)`.

The second form allows to specify a host and a port to which the UDP socket is connected.
If an UDP socket is connected, calls to `snd()` and `rcv()` act like on a stream socket:
The data is sent and received only to/from the host to which the socket is connected.

### Connect Functions
Declared in `inetclientdgram.hpp`, defined in `inetclientdgram.cpp`

	void connect(const char* host, const char* port);

(Re)connects the socket to the specified host/port. If you want to change the address family, you have to create
another socket.

	void deconnect(void);

Cut the connection to the host to which the socket was connected to. Now, stream-like
functions like `snd()` or `rcv()` may not be used anymore.

### Destroy Functions
Declared in `socket.hpp`, defined in `socket.cpp`

	int destroy(void);

0: Success, -1: Error (the only errors at `close()` come if the file descriptor has been closed already)

### Send/Upload Functions
Defined in `dgramclient.cpp`

	ssize_t snd(const void* buf, size_t len, int flags=0); // flags: send()

Conventional send, *only available if socket is connected*.

Send `len` bytes from `buf` (does not need to be `const`; in C++ an implicit conversion to const is allowed)
to the connected peer. `flags` may be specified and take the flags described in `send(2)` (`MSG_...`).

Defined in `inetdgram.cpp`

	1: ssize_t sndto(const void* buf, size_t len, const char* host, const char* port, int sndto_flags=0);
	
	2: ssize_t sndto(const void* buf, size_t len, const std::string& host, const std::string& port, int sndto_flags=0)

1, 2: Send `len` bytes from `buf` to `host`:`port`. `sndto_flags` may be specified and take the flags described
in `sendto(2)` (`MSG_...`).

	friend inet_dgram_client& operator<<(inet_dgram_client& sock, const char* str);
	friend inet_dgram_client& operator<<(inet_dgram_client& sock, std::string& str);

Only for connected sockets. Send either a string or a C string to the connected peer.
Usage like streams:

	sock << "abc" << std::string("def");

### Receive/Download Functions
Defined in `dgramclient.cpp`

	ssize_t rcv(void* buf, size_t len, int flags=0);

Conventional receive function: Receive `len` bytes from the socket and write them to `buf`. `flags` may take the
flags described in `recv(2)` (`MSG_...`). Only available if socket is connected!

Defined in `inetdgram.cpp`

	1: ssize_t rcvfrom(void* buf, size_t len, char* host, size_t hostlen, char* port, size_t portlen, int rcvfrom_flags=0, bool numeric=false);

	2: ssize_t rcvfrom(void* buf, size_t len, std::string& srchost, std::string& srcport, int rcvfrom_flags=0, bool numeric=false);

1: Receive `len` bytes from the socket and place them in `buf`. The source host is placed in `host`, which is at least
`hostlen` bytes long, the source port gets written to `port`, which is at least `portlen` bytes long. `recvfrom_flags`
can take the flags described in `recvfrom(2)`, `numeric` is considered as `false`, but if you specify it as
`true`, source host and source port are expressed in numerical form. This is recommended because it's faster
than an additional (internal) rDNS query.

2: Same as form 1, but use strings. The strings are resized to the appropriate length of host and port.

	friend inet_dgram_client& operator>>(inet_dgram_client& sock, std::string& dest);

Stream-like read from (connected!) socket: Reads at most `dest.size()` bytes from socket and puts them
to the string. If less than `dest.size()` characters could be read, the string is resized to
the number of read characters. One read call normally returns one datagram

## `inet_dgram_server` - INET DGRAM server sockets
Declared in `inetserverdgram.hpp`, defined in `inetserverdgram.hpp`.

	inet_dgram_server(const char* host, const char* port, int proto_osi3, int flags=0);

Create and bind a DGRAM socket. The only difference to `inet_dgram_client` is that this socket is explicitly bound to somewhere.

* `host` to bind to
* `port` to bind to
* `proto_osi3` - `IPv4 IPv6 BOTH`
* `flags` is passed to `socket(2)`

It is not possible to call `connect(), rcv(), snd()` on such sockets; the `rcvfrom(), sndto()` functions may be called, of course.

# libunixsocket++

libunixsocket++ is the UNIX domain socket part of libsocket++. The class tree is described above.

##`unix_stream_client`
Defined in `unixclientstream.cpp`

	unix_stream_client(void);
	unix_stream_client(const char* path, int socket_flags=0);
	unix_stream_client(const string& path, int socket_flags=0);

Create a new unix domain stream client socket. The second and the third form connect the socket immediately to `path`.
If you use the first form, you have to `connect()` your socket before using it. `socket_flags` are flags for the 
`socket(2)` syscall.

### `connect()`
Defined in `unixclientstream.cpp`

	void connect(const char* path, int socket_flags=0);
	void connect(const string& path, int socket_flags=0);

Connect the socket to `path`. `socket_flags` are given to `socket(2)`.

### `shutdown()`
Defined in `unixclientstream.cpp`

	void shutdown(int method=WRITE);

Shut the socket down. `method` is `READ`, `WRITE` or `READ|WRITE` and specifies how the socket should be shut down.

### `snd() rcv()`
Defined in `streamclient.cpp`

	1: ssize_t snd(const void* buf, size_t buflen, int send_flags=0);
	2: ssize_t rcv(void* buf, size_t len, int recv_flags=0);

1: Send the data in `buf` which is `buflen` bytes to the connected peer. `send_flags` is passed to `send(2)`.
2: Receive `buflen` bytes from the connected peer and store them in buf. `recv_flags` is passed to `recv(2)`.

