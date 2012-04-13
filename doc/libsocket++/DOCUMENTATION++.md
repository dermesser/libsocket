# DOCUMENTATION for libsocket++

## Introduction and Information for Devs

libsocket++ is a object-oriented wrapper around libsocket. To avoid writing the code twice
and maintain it twice, libsocket++ calls the functions from libsocket.

### Practical Usage

libsocket++ uses the namespace `libsocket`, so you either have to construct the classes as this:

	libsocket::inet_stream sock;

or use some things or the whole namespace:

	using namespace libsocket;

(resp., for the internet TCP socket class)

	using libsocket::inet_stream;

### Internal Class Hierarchy
[ For a good-looking version, take a look to the markdown file! ]

Legend:

- [ ] full-qualified class name
- { } file name of the class
- Provides: - What stuff is provided by this class?
- |/\\\_ - Path to inheriting class

	[ libsocket::socket ]{ C++/socket.cpp }
		|  		Provides: socket file descriptor data element,
		|			  generic destroy() function
		|`------------------------------------------------------------------------------------+
		|                                                                                     |
	[ libsocket::inet_socket ]{ C++/inetsocket.cpp }				[ libsocket::unix_socket ] // Not implemented yet
		|	Provides: some data elements
                |`---------------------------------------------+
	        |                                              |
	[ libsocket::inet_stream ]{ C++/inetsocket.cpp }       |
			Provides: All functions to be used     |
							       |
		+----------------------------------------------+
	        |
	[ libsocket::inet_dgram ]{ C++/inetsocket.cpp }

## Usage in Your Application

As you see in the figure above, the code comes in two main files: `socket.cpp` and `inetsocket.cpp` (resp. `unixsocket.cpp`).

In every file using the classes mentioned above, header files must be included:
- `headers++/socket.hpp` - usually included by `inetsocket.hpp` or `unixsocket.hpp`
- `headers++/inetsocket.hpp` - files using `inet_dgram` or `inet_stream`
- `headers++/unixsocket.hpp` - files using `unix_dgram` or `unix_stream`

*Please note:* `inetsocket.cpp` and `unixsocket.cpp` are using the header file `socket.hpp` so you have to
change the include paths in the first both files eventually (default is "../headers++/socket.hpp").

Then, compile the files (like mentioned in DOCUMENTATION.md) like every other:

	$ gcc -c */*.cpp
	$ gcc *.o
or
	$ gcc */*.cpp

# libinetsocket++

libinetsocket++ is the wrapper around libinetsocket.

## Exception Handling
Exception handling is done with the following class:

	struct inet_exception
	{
		string mesg;

		inet_exception(string,int,string);
	};

An instantiation of this object is thrown in case of error. Almost every function (excepted
the `try_to_...()` functions) is able to throw one or more exceptions. The member `mesg`
contains a string looking like this:

	../C++/inetsocket.cpp:151: inet_stream::destroy() - Socket already closed!
		(1)		(2)		(3)		(4)

It contains information about the file (1), the line (2), the function throwing the exception (3)
and information about the cause for the exception (4) - here, the socket was already closed when calling
the .destroy() routine.

## `inet_stream` Class: Internet TCP Stream Sockets
### Constructors

	inet_stream(void);
	inet_stream(const char* host, const char* port, int proto_osi3, int flags=0);

The first constructor only initializes the most important things. The socket remains unconnected
and must be connected before use using `connect()`.

The second constructors initializes the socket and connects it with the given host:

- `host`: Destination host; if you have the host as `std::string`, use its routine `std::string::c_str()` to get
the C string
- `port`: Destination port (TCP, of course); conversion from string like above.
- `proto_osi3`: `IPv4`, `IPv6` or `BOTH` (`BOTH` lets the library choose; pp macros; defined in header file `inetsocket.hpp`)
- `flags`: Default 0, can be `SOCK_NONBLOCK` or `SOCK_CLOEXEC` (see: `socket(2)`; on other platforms than Linux
has to be 0 to avoid errors)

### `connect()`

	void connect(const char* host, const char* port, int proto_osi3, int flags=0);

Connects the socket. Throws an exception if the socket is already connected.

- `host`: Destination host; if you have the host as `std::string`, use its routine `std::string::c_str()` to get
the C string
- `port`: Destination port (TCP, of course); conversion from string like above.
- `proto_osi3`: `IPv4` or `IPv6` (pp macros; defined in header file `inetsocket.hpp`)
- `flags`: Default 0, can be `SOCK_NONBLOCK` or `SOCK_CLOEXEC` (see: `socket(2)`; on other platforms than Linux
has to be 0 to avoid errors)

### `shutdown()`

	void shutdown(int method);

Shuts the socket down (`shutdown(2)`).

`method` is `READ`, `WRITE`, or the ORed combination, `READ|WRITE`. `READ` and `WRITE` are defined
in `inetsocket.hpp`

### Destroy Functions

	void try_to_destroy(void);
	void destroy(void);

These two functions do the same thing: Close the socket and destroy the connection.

The difference is that `try_to_destroy()` fails silently and `destroy()` would fail
with an exception if the socket is already closed.

### Output/Upload Functions

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

	ssize_t rcv(void* buf, size_t len, int flags=0);

Conventional receive function: Receive `len` bytes from socket and write them to `buf`.
`flags` may be specified and may take the flags specified in `recv(2)` (those beginning with `MSG_`)

	friend inet_stream& operator>>(inet_stream& sock, string& dest);

Stream-like read from socket: Reads at most `dest.size()` bytes from socket and puts them
to the string. If less than `dest.size()` characters could be read, the string is resized to
the number of read characters so you can check (`string.size() == 0`) if the server is done
with sending - either closed the socket on his side or shut it down for write access.

### Getters

	int getfd(void) const;
	string gethost(void) const;
	string getport(void) const;

`getfd()` returns the socket file descriptor.

`gethost()` returns a C++ string containing the host to which the socket is connected.

`getport()` returns a C++ string containing the port/service to which the socket is connected.

## `inet_dgram` Class: Internet UDP Sockets
### Constructors
	inet_dgram(int proto_osi3, int flags=0); // Flags: socket()
	inet_dgram(const char* host, const char* port, int proto_osi3, int flags=0); // Flags: socket()

Because the UDP socket can be connected multiple times and send data to various hosts,
it's mandatory to specify the address family at instantiation time. `proto_osi3` may be
`IPv4` or `IPv6`; this information is used to create a socket with `create_inet_dgram_socket()`
and finally `socket(2)`.

The second form allows to specify a host and a port to which the UDP socket is connected.
If an UDP socket is connected, calls to `snd()` and `rcv()` act like it is a stream socket:
The data is sent and received only to/from the host to which the socket is connected.

### Connect Functions

	void connect(const char* host, const char* port);

(Re)connects the socket to the specified host/port. If you want to change the address family, you have to create
another socket.

	void deconnect(void);

Cut the connection to the host to which the socket was connected to. Now, stream-like
functions like `snd()` or `rcv()` may not be used anymore.

### Destroy Functions

	void try_to_destroy(void);

Try to destroy the socket, but do not throw an exception if it failed.

	void destroy(void);

Try to destroy the socket and throw an exception if it failed.

### Send/Upload Functions

	ssize_t snd(const void* buf, size_t len, int flags=0); // flags: send()

Conventional send, *only available if socket is connected*.

Send `len` bytes from `buf` (does not need to be `const`; in C++ an implicit conversion to const is allowed)
to the connected peer. `flags` may be specified and take the flags described in `send(2)` (`MSG_...`).

	ssize_t sndto(const void* buf, size_t len, const char* host, const char* port, int sndto_flags=0); // flags: sendto()

Send `len` bytes from `buf` to `host`:`port`. `sndto_flags` may be specified and take the flags described
in `sendto(2)` (`MSG_...`).

### Receive/Download Functions

	ssize_t rcv(void* buf, size_t len, int flags=0);

Conventional receive function: Receive `len` bytes from the socket and write them to `buf`. `flags` may take the
flags described in `recv(2)` (`MSG_...`).

	ssize_t rcvfrom(void* buf, size_t len, char* host, size_t hostlen, char* port, size_t portlen, int rcvfrom_flags=0, bool numeric=false);

Receive `len` bytes from the socket and place them in `buf`. The source host is placed in `host`, which is at least
`hostlen` bytes long, the source port gets written to `port`, which is at least `portlen` bytes long. `recvfrom_flags`
can take the flags described in `recvfrom(2)`, `numeric` is considered as `false`, but if you specify it as
`true`, source host and source port are expressed in numerical form. This is recommended because it's faster
than an additional (internal) rDNS query.

# libunixsocket++

Not implemented yet.
