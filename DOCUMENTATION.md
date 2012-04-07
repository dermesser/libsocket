#Documentation for libinetsocket

##Clients
###`create_isocket()`

	int create_isocket(const char* host, const char* service, char proto_osi4, char proto_osi3, int flags)
	
`create_isocket()` creates and connects a new socket. The parameters have to be filled like this:

* `host` is a string (0-terminated) containing the hostname or IP address of the target host
* `service` is a string (0-terminated) containing the portname or service (e.g. `http`)
* `proto_osi4` is a value representing the protocol on OSI layer 4 which we want to use (defined as macros): `TCP`, `UDP` or `BOTH` when libsocket should choose
* `proto_osi3` is a value representing the protocol on OSI layer 3 which we want to use (defined as macros): `IPv6`, `IPv4` or `BOTH` when libsocket should choose
* `flags` is `SOCK_CLOEXEC` and/or `SOCK_NONBLOCK` (as specified in `socket(2)`). `SOCK_CLOEXEC` will close the socket if you call `exec*()`. `SOCK_NONBLOCK` will return -1 instead of
waiting for data on the socket.

The return value is either a valid file descriptor on which you can execute `write()` or `read()` (from `unistd.h`). If there's an error when creating
that socket, it returns -1.

Important for UDP sockets: The returned socket file descriptor may be used with `read()` and `write()` as well as TCP sockets. If you want to use `sendto()` and
`recvfrom()`, don't use this library. But generally, it is not necessary to use sendto and recvfrom: When you use connected datagram sockets,
each `read()` call will create a new datagram.

###`reconnect_isocket()`

	int reconnect_isocket(int sfd, char* host, char* service);

`reconnect_isocket()` is an easy way to connect an existing socket to a new peer. It is faster and more elegant than destroying the first socket with `destroy_isocket()`
and make a new with `create_isocket()`. ***IMPORTANT: IT MAY ONLY BE USED WITH UDP SOCKETS! IF USED WITH TCP SOCKETS, IT WON'T WORK!!!*** The parameters:

* `sfd` is the existing Socket File Descriptor
* `host` and `service` like in `create_isocket()`
which you chose at `create_isocket()` because it is impossible to get the socktype from the file descriptor.

This function returns either 0 (Success) or -1 (Failure).

###`shutdown_isocket()` 

	int shutdown_isocket(int sfd, int method)

`shutdown_isocket()` shuts a socket down. This means that (READ) you cannot read data anymore respectively (WRITE) you cannot write data anymore
and the other peer gets an EOF signal (`read()` returns 0).

* `sfd` is the Socket File Descriptor
* `method` is either READ, WRITE or READ|WRITE (ORed).

This function returns either 0 (Success) or -1 (Failure)

###`destroy_isocket()`
	
	int destroy_isocket(int sfd)

`destroy_isocket()` `close()`s the specified Socket file descriptor `sfd`. It does not perform a `shutdown()` on it because that would produce
an error if you shut it down before. But that's equal because `close()` also sends EOF.

This function returns either 0 (Success) or -1 (Failure)

##Server

libsocket also supports INET server sockets (also called Passive Sockets).

###`create_issocket()`
'issocket' stands for 'Internet Server Socket', not for 'is socket'. :)

	int create_issocket(const char* bind_addr, const char* bind_port, char proto_osi4, char proto_osi3);

Creates a new server socket:

* `bind_addr` is the address to bind to. It's normally good when you specify "0.0.0.0".
* `bind_port` is the port to bind to.
* proto_osi4 is either TCP or UDP. If it's UDP, the socket doesn't `listen()`.
* proto_osi3 is either IPv4 or IPv6

The call to `listen()` (which is not executed when using UDP sockets uses the biggest linux backlog size 128. You should change it in the source code if you
want another value.

This function returns an int value suitable for calls with `accept()` and `accept_issocket()`.

A with this function created socket may be destroyed with `destroy_isocket()`.

###`accept_issocket()`

 	int accept_issocket(int sfd, char* src_host, size_t src_host_len, char* src_service, size_t src_service_len, int flags);

`accept_issocket()` performs an action similar to `accept()`. **It may not be called on UDP server sockets.** 

* `sfd` is the socket file descriptor
* `src_host` is a pointer to a buffer into which the lib writes the hostname of the client. `src_host_len` is the length of its buffer. More bytes are truncated
* `src_service` and `src_service_len` is the same like `src_host` and `src_host_len`, but for the ports
* `flags` may be `NUMERIC`, which results in numeric host and service names.

`accept_issocket()` returns a file descriptor for a connection to the client which is to be used with `read()` and `write()`. 
In case of failure, it returns -1. `accept_issocket()` blocks until a client connects.

###`recvfrom_issocket()`

	size_t recvfrom_issocket(int sfd, void* buffer, size_t size, char* src_host, size_t src_host_len, char* src_service, size_t src_service_len, int flags)

`recvfrom_issocket()` receives some bytes from a UDP socket. It also may work with STREAM sockets (as TCP) but it is recommended to use `accept_issocket()` with TCP sockets.

The parameters are:

* `sfd` is the socket file descriptor
* `buffer` is a memory block for the received bytes
* `size` is the size of the buffer
* `src_host` is a buffer for the client name and `src_host_len` its length
* `src_service` is a buffer for the client's port and `src_service_len` its length. If you use `NUMERIC` as flag (see next parameter), 
you have to give a buffer of a length of 6 or more - elsewise, the internally used `getnameinfo()` will fail (5 digits for outgoing port plus a \0 byte) 
with "unknown error"! If you use the nonnumerical form, you have to give even more buffer space.
* `flags` may be NUMERIC which results in numeric client and port names.

**Important: Please give buffers which are big enough to hold longer names - if you don't do so, `getnameinfo()` (which is used internally) will return with an unknown error.**

#Documentation for libunixsocket

##Client

###`create_usocket()`

	int create_usocket(const char* path, int socktype);

Creates and connects a new UNIX domain socket file descriptor for a socket located at `path`, type `socktype`.
`socktype` is either `STREAM` or `DGRAM`. 

Important for DGRAM sockets: Please think twice if you want to use DGRAM sockets in UNIX domain. They do not have any advantages
over STREAM sockets!

###`reconnect_usocket()`

	int reconnect_usocket(int sfd, const char* path);

Reconnect a DGRAM UNIX socket `sfd` to the new UNIX socket `path`.

###`shutdown_usocket()` 

	int shutdown_usocket(int sfd, int method)

`shutdown_socket()` shuts a socket down. This means that (READ) you cannot read data anymore respectively (WRITE) you cannot write data anymore
and the other peer gets an EOF signal (`read()` returns 0).

* `sfd` is the Socket File Descriptor
* `method` is either READ, WRITE or READ|WRITE (ORed).

###`destroy_socket()`
	
	int destroy_usocket(int sfd)

`destroy_socket()` shuts the socket down for READ and WRITE operations and `close()`s it.

##Server

###`create_ussocket()`

	int create_ussocket(char* path, int socktype);

Create a new server socket and bind it to `path`. Protocol type (`socktype`) is either `DGRAM` for datagrams (**NOT RECOMMENDED**) or `STREAM`.

###`accept_ussocket()`

	int accept_ussocket(int sfd, int flags);

Accept a new STREAM connection on socket `sfd`. `flags` may be `SOCK_NONBLOCK` which means that the new (returned) client socket won't block or `SOCK_CLOEXEC` which
means that the client socket will be closed if you call a syscall from the `exec()` family.

###`recvfrom_ussocket()`

	size_t recvfrom_ussocket(int sfd, void* buf, size_t size, char* from, size_t from_size)

`recv_ussocket()` gets `size` bytes from `sfd` (usually a DGRAM socket) and writes them to `buf`.
from may be 0 if you don't want to have the origin, or a pointer to a buffer of the size from_size.

#Compile options

If you specify the flag `-DVERBOSE` at compile time, libsocket uses STDERR to print information about occurred errors.
VERBOSE is not activated by default so libsocket is 'quiet'. Using VERBOSE is very useful when searching bugs in your program
or my library.
