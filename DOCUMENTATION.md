#Documentation for libinetsocket

##`create_isocket()`

	int create_isocket(const char* host, const char* service, char proto_osi4, char proto_osi3)
	
`create_isocket()` creates and connects a new socket. The parameters have to be filled like this:

* `host` is a string (0-terminated) containing the hostname or IP address of the target host
* `service` is a string (0-terminated) containing the portname or service (e.g. `http`)
* `proto_osi4` is a value representing the protocol on OSI layer 4 which we want to use (defined as macros): `TCP`, `UDP` or `BOTH` when libsocket should choose
* `proto_osi3` is a value representing the protocol on OSI layer 3 which we want to use (defined as macros): `IPv6`, `IPv4` or `BOTH` when libsocket should choose

The return value is either a valid file descriptor on which you can execute `write()` or `read()` (from `unistd.h`). If there's an error when creating
that socket, it returns -1.

Important for UDP sockets: The returned socket file descriptor may be used with `read()` and `write()` as well as TCP sockets. If you want to use `sendto()` and
`recvfrom()`, don't use this library. But generally, it is not necessary to use sendto and recvfrom: When you use connected datagram sockets,
each `read()` call will create a new datagram.

##`shutdown_isocket()` 

	int shutdown_isocket(int sfd, int method)

`shutdown_isocket()` shuts a socket down. This means that (READ) you cannot read data anymore respectively (WRITE) you cannot write data anymore
and the other peer gets an EOF signal (`read()` returns 0).

* `sfd` is the Socket File Descriptor
* `method` is either READ, WRITE or READ|WRITE (ORed).

##`destroy_isocket()`
	
	int destroy_isocket(int sfd)

`destroy_isocket()` shuts the socket down for READ and WRITE operations and `close()`s it.

#Documentation for libunixsocket

##`create_socket()`

	int create_usocket(const char* path, int socktype);

Creates and connects a new UNIX domain socket file descriptor for a socket located at `path`, type `socktype`.
`socktype` is either `STREAM` or `DGRAM`. 

Important for DGRAM sockets: Please think twice if you want to use DGRAM sockets in UNIX domain. They do not have any advantages
over STREAM sockets!

##`shutdown_socket()` 

	int shutdown_socket(int sfd, int method)

`shutdown_socket()` shuts a socket down. This means that (READ) you cannot read data anymore respectively (WRITE) you cannot write data anymore
and the other peer gets an EOF signal (`read()` returns 0).

* `sfd` is the Socket File Descriptor
* `method` is either READ, WRITE or READ|WRITE (ORed).

##`destroy_socket()`
	
	int destroy_socket(int sfd)

`destroy_socket()` shuts the socket down for READ and WRITE operations and `close()`s it.


#Compile options

If you specify the flag `-DVERBOSE` at compile time, libsocket uses STDERR to print information about occurred errors.
VERBOSE is not activated by default so libsocket is 'quiet'.
