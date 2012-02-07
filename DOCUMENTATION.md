#Documentation for libsocket

##`create_socket()`

	int create_socket(const char* host, const char* service, char proto_osi4, char proto_osi3)
	
`create_socket()` creates and connects a new socket. The parameters have to be filled like this:

* `host` is a string (0-terminated) containing the hostname or IP address of the target host
* `service` is a string (0-terminated) containing the portname or service (e.g. `http`)
* `proto_osi4` is a value representing the protocol on OSI layer 4 which we want to use (defined as macros): `TCP`, `UDP` or `BOTH` when libsocket should choose
* `proto_osi3` is a value representing the protocol on OSI layer 3 which we want to use (defined as macros): `IPv6`, `IPv4` or `BOTH` when libsocket should choose

The return value is either a valid file descriptor on which you can execute `write()` or `read()` (from `unistd.h`). If there's an error when creating
that socket, it returns -1.

##`destroy_socket()`
	
	int destroy_socket(int sfd)

`destroy_socket()` shuts the socket down for READ and WRITE operations and `close()`s it.
