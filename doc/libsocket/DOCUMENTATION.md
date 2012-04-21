# Documentation for libsocket
## Introduction/Explanations

libsocket is a library written in C and designed to simplify the usage of `AF_INET/AF_INET6` (internet domain) and `AF_UNIX`
(unix domain) sockets. With many specialized functions in the API, it's hard to do things wrong. And with the excellent error
processing with optional verbosity, it's also quite easy to debug your applications.

libsocket is targeted at experienced C/Linux programmers which don't like the traditional BSD sockets API.
libsocket is not targeted at programmers which do their first experiences with C or sockets! To understand the
library, you must have experience with the actual syscalls.

libsocket is the name for the whole project. libinetsocket is the part for `INET` sockets, libunixsocket for `UNIX` sockets.

The names of the procedures (in the following also called 'functions' or 'API calls', syscalls are called 'syscall')
follow a simple scheme to simplify the usage:

	<action>_<domain>_<protocol>_socket()

e.g.

	create_inet_stream_socket()

It's important to know this because then you may keep the names better in mind.

If there's something in the text looking like

	getaddrinfo(3)
	socket(2)

This means that I refer to manpages. They can be reached by the following commands:

	$ man 3 getaddrinfo
	$ man 2 socket

**If you want to see some examples of the usage of this library, take a look at the example programs
in examples/. They compile fine on most modern Linux systems and should work out of the box**

Note: The manpage extracts come from the manpages delivered with Debian (Linux manpages), usually created by
the man-pages project, and the OpenBSD manpage extracts come from http://www.openbsd.org/cgi-bin/man.cgi

## Compile time configuration

If the macro `VERBOSE` is defined, libsocket prints error messages on STDERR device. Typically,
you specify it in the compile command:

	gcc -DVERBOSE ...

Usually, you'll use this option during development, but disable it when you finished the application.

### Usage of flags

The usage of the flags for some functions is described for every function. If you don't need any flags (you think),
you may specify for every flag argument 0 without errors.

### libinetsocket

As the name says, libinetsocket is the part for internet domain sockets. All important features are supported and it's possible
to give flags to the underlying calls, e.g. it's possible to give the flag `MSG_DONTROUTE` to the wrapper function of `sendto()`.

libinetsocket is adapted for multi-platform usage. It's mainly tested on Linux, but the goal is also
to get it working on OpenBSD.

### libunixsocket

libunixsocket is responsible for UNIX domain sockets. The calls are similar to the libinetsocket calls.

libunixsocket is very linux-specific; you can't see multi-platform adaptations etc., and it probably won't run on other
platforms.

# Usage in your application
Static linking is recommended. That means, copy a checkout of the library files (C/, headers/) to the
source tree of your application. Then, simply compile the files together with the other files (as they would
be normal part of your application).

*Details about dependancies:* The actual code is in `libinetsocket.c` resp. `libunixsocket.c`. Every
file of your application which wants to use this functions has to include the header files, e.g.:

	# include "../headers/libinetsocket.c"
	# include <stdio.h>
	...
	.
	.
	.

Then, compile it (paths are only examples):

	$ gcc -c src/*.c
	$ gcc -c libsocket/*.c
	$ gcc *.o

or...

	$ gcc src/*.c libsocket/*.c
The 2-clause BSD license allows this, also for proprietary applications.

# API calls
## libinetsocket
### `create_inet_stream_socket()`
`int create_inet_stream_socket(const char* host, const char* service, char proto_osi3, int flags)`

This function creates, connects and returns a inet stream socket (TCP socket) which is connected to `host`:`service`.

* `host`: Destination host
* `service`: Destination host's port
* `proto_osi3`: OSI layer 3 protocol: either `IPv4` or `IPv6` (defined in `libinetsocket.h`). 
* `flags` **is only available on Linux >= 2.6.27 - elsewise, specify 0 to avoid errors** 
and allows to manipulate the `socket()` call. 

Returns on success the socket file descriptor number, on error -1.

`socket(2)` says the following:

>Since Linux 2.6.27, the type argument serves a second purpose: in addition to specifying a socket type, it may include the bitwise OR of any of the following values, to modify the behavior
>of socket():
>
>	* `SOCK_NONBLOCK`   Set the `O_NONBLOCK` file status flag on the new open file description.  Using this flag saves extra calls to fcntl(2) to achieve the same result.
>
>	* `SOCK_CLOEXEC`    Set the close-on-exec (`FD_CLOEXEC`) flag on the new file descriptor.  See the description of the `O_CLOEXEC` flag in open(2) for reasons why this may be useful.

`SOCK_NONBLOCK` does not let you wait when read()ing or write()ing; instead of blocking (if there's nothing available) read()/write()
return -1.

`SOCK_CLOEXEC` closes the socket if you call a syscall from the `exec()` family.

The used protocol is TCP.

To send and receive data from the freshly created socket, please use `read(2)` and `write(2)`.

### `create_inet_dgram_socket()`
`int create_inet_dgram_socket(char proto_osi3, int flags)`

Returns an integer describing a DGRAM (UDP) socket.

* `proto_osi3` is `IPv4` (`AF_INET`) or `IPv6` (`AF_INET6`).
* `flags` may be the flags specified in `socket(2)`, i.e. `SOCK_NONBLOCK` and/or `SOCK_CLOEXEC`. 
More than one flags may be ORed. **This argument is only available on Linux >= 2.6.27!**

Like the function before, this function also has different prototypes on different platforms.

Returns the socket file descriptor number, on error -1.

The protocol which is used is UDP.

To send and receive data with this socket, please use the functions explained below, `sendto_inet_dgram_socket()` and
`recvfrom_inet_dgram_socket()`. Of course, it's also possible to use `sendto(2)` and `recvfrom(2)`, but if you use them,
why do you use libsocket?

### `sendto_inet_dgram_socket()`
`ssize_t sendto_inet_dgram_socket(int sfd,void* buf, size_t size,char* host, char* service, int sendto_flags)`

This function is the equivalent to `sendto()`. It returns the number of bytes which was sent; usually, it's the value
of `size`.

* `sfd` is the *Socket File Descriptor* (every socket file descriptor argument in libsocket is called `sfd`) which
you got from `create_inet_dgram_socket()`. *The usage with STREAM sockets is not recommended and the result is undefined!*
* `buf` is a pointer to some data.
* `size` is the length of the buffer to which buf points.
* `host` is the host to which we want to send the data. It's a string so you may specify everything what's resolved by
`getaddrinfo()`, i.e. an IP ("193.21.34.21") or a hostname ("example.net").
* `service` is the port on the remote host. Like in `host`, you may specify the port either as number ("123") or as service string
("ntp", "http", "gopher").
* `sendto_flags` is available on all platforms. The value given here goes directly to the internal `sendto()` call. The
flags which may be specified differ between the platforms. 

Returns the number of bytes sent, on error -1.

On Linux, you can get them from the manpage `sendto(2)`:

       "The flags argument is the bitwise OR of zero or more of the following flags.

       MSG_CONFIRM (Since Linux 2.3.15)
              Tell the link layer that forward progress happened: you got a successful reply from  the  other  side.   If  the  link  layer
              doesn't  get  this  it  will regularly reprobe the neighbor (e.g., via a unicast ARP).  Only valid on SOCK_DGRAM and SOCK_RAW
              sockets and currently only implemented for IPv4 and IPv6.  See arp(7) for details.

       MSG_DONTROUTE
              Don't use a gateway to send out the packet, only send to hosts on directly connected networks.  This is usually used only  by
              diagnostic or routing programs.  This is only defined for protocol families that route; packet sockets don't.

       MSG_DONTWAIT (since Linux 2.2)
              Enables  nonblocking  operation;  if  the  operation would block, EAGAIN or EWOULDBLOCK is returned (this can also be enabled
              using the O_NONBLOCK flag with the F_SETFL fcntl(2)).

       MSG_EOR (since Linux 2.2)
              Terminates a record (when this notion is supported, as for sockets of type SOCK_SEQPACKET).

       MSG_MORE (Since Linux 2.4.4)
              The caller has more data to send.  This flag is used with TCP sockets to obtain the same effect as the TCP_CORK socket option
              (see tcp(7)), with the difference that this flag can be set on a per-call basis.

              Since Linux 2.6, this flag is also supported for UDP sockets, and informs the kernel to package all of the data sent in calls
              with this flag set into a single datagram which is only transmitted when a call is performed that does not specify this flag.
              (See also the UDP_CORK socket option described in udp(7).)

       MSG_NOSIGNAL (since Linux 2.2)
              Requests  not to send SIGPIPE on errors on stream oriented sockets when the other end breaks the connection.  The EPIPE error
              is still returned.

       MSG_OOB
              Sends out-of-band data on sockets that support this notion (e.g., of type SOCK_STREAM); the  underlying  protocol  must  also
              support out-of-band data."

On OpenBSD, `sendto(2)` says this:

		The flags parameter may include one or more of the following:

		MSG_OOB          process out-of-band data
		MSG_DONTROUTE    bypass routing, use direct interface
		MSG_NOSIGNAL     don't send SIGPIPE

If it is not possible to send data at the moment, this call blocks excepted you specified `SOCK_NONBLOCK` when creating the socket.

### `recvfrom_inet_dgram_socket()`
`ssize_t recvfrom_inet_dgram_socket(int sfd, void* buffer, size_t size, char* src_host, size_t src_host_len, char* src_service, size_t src_service_len, int recvfrom_flags, int numeric)`

`recvfrom_inet_dgram_socket()` is the equivalent to `recvfrom()`.

* `sfd` - socket file descriptor
* `buffer`: Memory area to which the received data is written
* `size`: Size (in bytes) of the buffer
* `src_host`: Buffer to which the source host is written. May be 0, then the source is discarded
* `src_host_len`: Length of the `src_host` buffer
* `src_service`: Buffer to which the source port is written. May be 0, then the source port is discarded.
* `src_service_len`: Length of `src_service` buffer
* `recvfrom_flags` is a ORed combination of the flags below (Linux)
* `numeric`: May be `NUMERIC` (defined in header files, then source port and host are given back numeric.

Returns the number of bytes received, or -1 if an error occurred.

	       MSG_CMSG_CLOEXEC (recvmsg() only; since Linux 2.6.23)
		      Set the close-on-exec flag for the file descriptor received via a UNIX domain file descriptor using the SCM_RIGHTS  operation
		      (described in unix(7)).  This flag is useful for the same reasons as the O_CLOEXEC flag of open(2).

	       MSG_DONTWAIT (since Linux 2.2)
		      Enables  nonblocking  operation;  if the operation would block, the call fails with the error EAGAIN or EWOULDBLOCK (this can
		      also be enabled using the O_NONBLOCK flag with the F_SETFL fcntl(2)).

	       MSG_ERRQUEUE (since Linux 2.2)
		      This flag specifies that queued errors should be received from the socket error queue.  The error is passed in  an  ancillary
		      message  with  a  type  dependent on the protocol (for IPv4 IP_RECVERR).  The user should supply a buffer of sufficient size.
		      See cmsg(3) and ip(7) for more information.  The payload of the original packet that caused the error  is  passed  as  normal
		      data via msg_iovec.  The original destination address of the datagram that caused the error is supplied via msg_name.

		      For  local  errors,  no address is passed (this can be checked with the cmsg_len member of the cmsghdr).  For error receives,
		      the MSG_ERRQUEUE is set in the msghdr.  After an error has been passed, the pending socket error is regenerated based on  the
		      next queued error and will be passed on the next socket operation.

		      The error is supplied in a sock_extended_err structure:

			  #define SO_EE_ORIGIN_NONE    0
			  #define SO_EE_ORIGIN_LOCAL   1
			  #define SO_EE_ORIGIN_ICMP    2
			  #define SO_EE_ORIGIN_ICMP6   3

			  struct sock_extended_err
			  {
			      uint32_t ee_errno;   /* error number */
			      uint8_t  ee_origin;  /* where the error originated */
			      uint8_t  ee_type;    /* type */
			      uint8_t  ee_code;    /* code */
			      uint8_t  ee_pad;     /* padding */
			      uint32_t ee_info;    /* additional information */
			      uint32_t ee_data;    /* other data */
			      /* More data may follow */
			  };

			  struct sockaddr *SO_EE_OFFENDER(struct sock_extended_err *);

		      ee_errno  contains  the  errno  number of the queued error.  ee_origin is the origin code of where the error originated.  The
		      other fields are protocol-specific.  The macro SOCK_EE_OFFENDER returns a pointer to the address of the network object  where
		      the  error  originated  from given a pointer to the ancillary message.  If this address is not known, the sa_family member of
		      the sockaddr contains AF_UNSPEC and the other fields of the sockaddr are undefined.  The payload of the  packet  that  caused
		      the error is passed as normal data.

		      For  local  errors,  no address is passed (this can be checked with the cmsg_len member of the cmsghdr).  For error receives,
		      the MSG_ERRQUEUE is set in the msghdr.  After an error has been passed, the pending socket error is regenerated based on  the
		      next queued error and will be passed on the next socket operation.

	       MSG_OOB
		      This  flag  requests  receipt of out-of-band data that would not be received in the normal data stream.  Some protocols place
		      expedited data at the head of the normal data queue, and thus this flag cannot be used with such protocols.

	       MSG_PEEK
		      This flag causes the receive operation to return data from the beginning of the receive queue without removing that data from
		      the queue.  Thus, a subsequent receive call will return the same data.

	       MSG_TRUNC (since Linux 2.2)
		      For  raw (AF_PACKET), Internet datagram (since Linux 2.4.27/2.6.8), and netlink (since Linux 2.6.22) sockets: return the real
		      length of the packet or datagram, even when it was longer than the passed buffer.  Not implemented for UNIX domain  (unix(7))
		      sockets.

		      For use with Internet stream sockets, see tcp(7).

	       MSG_WAITALL (since Linux 2.2)
		      This  flag  requests  that  the operation block until the full request is satisfied.  However, the call may still return less
		      data than requested if a signal is caught, an error or disconnect occurs, or the next data to be received is of  a  different
		      type than that returned.

OpenBSD's `recvfrom(2)` says this about recvfrom()'s flags:

		MSG_OOB         process out-of-band data
		MSG_PEEK        peek at incoming message
		MSG_WAITALL     wait for full request or error
		MSG_DONTWAIT    don't block

### `connect_inet_dgram_socket()`
`int connect_inet_dgram_socket(int sfd, char* host, char* service)`

Calling `connect(2)` on DGRAM sockets is possible - you do it with this function. 
If you do it, subsequent calls to `read(2)` and `write(2)`
will receive and send data only to the host and service you connected to. Sending/receiving to/from other hosts
is still possible with `{recvfrom,sendto}_inet_dgram_socket()`. You may see an example covering this as explained here in 
`examples/echo_dgram_connected_client.c`.

* `sfd` is the file descriptor 
* `host` and `service` are the target address parameters, like in `create_inet_stream_socket()`.

You may call this function multiple times on one socket to reconnect it.

You may 'de-connect' a socket by calling

	connect_inet_dgram_socket(socket,0,0);

where the second 0 also could be "abc" - important is that the host argument is 0. After this call, the connection
is solved and `write(2)/read(2)` is not possible anymore. Of course, you may call this function again to connect the socket.

Returns 0 on success, -1 on error.

### `destroy_inet_socket()`
`int destroy_inet_socket(int sfd)`

Simply calls `close(2)` on `sfd`.

Returns 0 on success, -1 on error.

### `shutdown_inet_stream_socket()`
`int shutdown_inet_stream_socket(int sfd, int method)`

Calls `shutdown(2)` on `sfd`. 

* `sfd`: Socket file descriptor
* `method` is either `READ`, `WRITE` or the ORed combination `READ|WRITE`.

Calling `shutdown_inet_stream_socket()` on a socket has the following effects:

* if `method` is `READ`: Disallow subsequent calls to `read()`
* if `method` is `WRITE`: Disallow subsequent calls to `write()` and send an EOF-like signal to the remote host
* if `method` is `READ|WRITE`: Combination of the two actions before.

Returns 0 upon success, -1 on error.

### `create_inet_server_socket()`
`int create_inet_server_socket(const char* bind_addr, const char* bind_port, char proto_osi4, char proto_osi3, int flags)`

Creates a server socket, also known as *Passive Socket*. With this socket, you may accept connections (STREAM) or
simply bind a UDP socket - if you use it as UDP "server" socket, it's almost the same like a socket created with
`create_inet_dgram_socket()`, but it's bound (`bind(2)`) to a fix address.

The arguments:

* `bind_addr`: The address to which the socket is bound to. E.g.: "127.0.0.1" for localhost, "0.0.0.0" for all available addresses
* `bind_port`: The port to bind to.
* `proto_osi4`: `UDP` or `TCP` (defined in headers)
* `proto_osi3`: `IPv4` or `IPv6`.
* `flags`: `SOCK_NONBLOCK` or `SOCK_CLOEXEC` (like in `create_inet_stream_socket()` etc, on non-linux systems, specify it as 0)

Internals: The `backlog` argument of `listen(2)` when using TCP sockets is set to `BACKLOG`, defined at the beginning
of src/libinetsocket.c. Default is 128, the maximum value on Linux. This may differ on other platforms.

Returns a socket file descriptor on success, on error, -1 is returned.

### `accept_inet_stream_socket()`

`int accept_inet_stream_socket(int sfd, char* src_host, size_t src_host_len, char* src_service, size_t src_service_len, int flags)`

Accept incoming connections on server sockets created with `create_inet_server_socket()` (above).

* `sfd`: Socket
* `src_host`: Buffer to which the source host is written
* `src_host_len`: Length of the previous buffer
* `src_service`: Buffer to which the source port is written
* `src_service_len`: Length of the previous buffer
* `flags`: may be `NUMERIC`.

This function will block until there's an incoming connection.

Returns a stream file descriptor connected to the connecting client. On error, -1 is returned.

## libunixsocket
As mentioned in the introduction, libunixsocket is Linux-specific. Until now, it did not run on other Unixes than Linux.

### `create_unix_stream_socket()`
`int create_unix_stream_socket(const char* path, int flags)`

Creates a UNIX STREAM socket and connects it to `path`.

* `path`: The socket is connected to the path given with this argument.
* `flags`: Optional flags for `socket(2)`: `SOCK_CLOEXEC` and `SOCK_NONBLOCK`

Returns a valid socket descriptor on success or -1 on error.

### `create_unix_dgram_socket()`
`int create_unix_dgram_socket(const char* bind_path, int flags)`

Creates a UNIX DGRAM socket and binds it optionally to `bind_path`.

* `bind_path`: If this is not 0, the socket is bound to this path, i.e. a socket file is created with this name.
* `flags`: Optional flags for `socket(2)`: `SOCK_CLOEXEC` and `SOCK_NONBLOCK`

Returns a valid socket descriptor on success or -1 on error.

### `connect_unix_dgram_socket()`
`int connect_unix_dgram_socket(int sfd, const char* path)`

Connect a UNIX DGRAM socket.

* `sfd`: The socket
* `path`: The path of the socket to which we want to connect the socket.

Returns 0 on success and -1 on error.

### `destroy_unix_socket()`
`int destroy_unix_socket(int sfd)`

Destroy (close) a socket.

* `sfd`: The socket file descriptor.

Returns 0 on success and -1 on error.

### `shutdown_unix_stream_socket()`
`int shutdown_unix_stream_socket(int sfd, int method)`

* `sfd`: Socket file descriptor
* `method`: `READ`/`WRITE`/`READ|WRITE`

Calling `shutdown_inet_stream_socket()` on a socket has the following effects:

* if `method` is `READ`: Disallow subsequent calls to `read()`
* if `method` is `WRITE`: Disallow subsequent calls to `write()` and send an EOF-like signal to the remote host
* if `method` is `READ|WRITE`: Combination of the two actions before.

Returns 0 upon success, -1 on error.

### `create_unix_server_socket()`
`int create_unix_server_socket(const char* path, int socktype, int flags)`

Creates a new UNIX server (passive) socket. 

* `path`: Path to which the new socket is bound to.
* `socktype`: `SOCK` or `DGRAM` (symbolic constants from header files)
* `flags`: Flags for `socket(2)`: `SOCK_NONBLOCK` or `SOCK_CLOEXEC`

Returns a valid passive socket descriptor or -1 on error.

**Note: If you specify `DGRAM` as socktype, you get the same result like with `create_unix_dgram_socket("path",0)` because there's
no difference between server and client when using DGRAM sockets (of course, you have to bind server sockets)**

### `accept_unix_stream_socket()`
`int accept_unix_stream_socket(int sfd, int flags)`

Accepts a connection on UNIX stream sockets.

* `sfd` is the passive socket, generated by `create_unix_server_socket()`.
* `flags` is a combination of `SOCK_CLOEXEC` and/or `SOCK_STREAM` for the new (client) socket.

Returns a socket connected to the client, or -1 on error.

### `recvfrom_unix_dgram_socket()`
`ssize_t recvfrom_unix_dgram_socket(int sfd, void* buf, size_t size, char* from, size_t from_size, int recvfrom_flags)`

Receives a datagram which is sent to the socket `sfd` which should be bound to somewhere.

* `sfd` is the socket
* `buf` is a buffer to which the data is written
* `size` is its size
* `from` is a buffer to which the source address is written
* `from_size` is its size
* `recvfrom_flags` is a combination of flags for `recvfrom()` (see section `recvfrom_inet_dgram_socket()`)

Returns the number of received bytes, or -1 on error.

### `sendto_unix_dgram_socket()`
`ssize_t sendto_unix_dgram_socket(int sfd, void* buf, size_t size, const char* path, int sendto_flags)`

Sends a message to another socket.

* `sfd`: Socket
* `buf`: Buffer
* `size`: Its length
* `path`: The path of the other socket
* `sendto_flags`: Combination of flags, as described at `sendto_inet_dgram_socket()`.

Returns number of sent bytes or -1.
