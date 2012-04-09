# Documentation for libsocket
## Introduction

libsocket is a library written in C and designed to simplify the usage of `AF_INET/AF_INET6` (internet domain) and `AF_UNIX`
(unix domain) sockets. With many specialized functions in the API, it's hard to do things wrong. And with the excellent error
processing with optional verbosity, it's also quite easy to debug your applications.

libsocket is the name for the whole project. libinetsocket is the part for `INET` sockets, libunixsocket for `UNIX` sockets.

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

libunixsocket is very linux-specific; here, you can't see multi-platform adaptations etc.

## API calls

This section explains the single API calls of libsocket.

### libinetsocket

#### `create_inet_stream_socket()`
`int create_inet_stream_socket(const char* host, const char* service, char proto_osi3, int flags)` (Linux)

`int create_inet_stream_socket(const char* host, const char* service, char proto_osi3)` (Others)

This function creates and returns a inet stream socket (TCP socket) which is connected to `host`:`service`.

`proto_osi3` is either `IPv4` or `IPv6` (defined in `libinetsocket.h`). 

`flags` **is only available on Linux >= 2.6.27** and allows to manipulate the `socket()` call. 
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

### `create_inet_dgram_socket()`

`int create_inet_dgram_socket(char proto_osi3, int flags)` (Linux)

`int create_inet_dgram_socket(char proto_osi3)` (Others)

Returns an integer describing a DGRAM (UDP) socket.

`proto_osi3` is `IPv4` (`AF_INET`) or `IPv6` (`AF_INET6`).

`flags` may be the flags specified in `socket(2)`, i.e. `SOCK_NONBLOCK` and/or `SOCK_CLOEXEC`.
More than one flags may be ORed. **This argument is only available on Linux >= 2.6.27!**

Like the function before, this function also has different prototypes on different platforms.

The protocol which is used is UDP.

### `sendto_inet_dgram_socket()`

`ssize_t sendto_inet_dgram_socket(int sfd,void* buf, size_t size,char* host, char* service, int sendto_flags)`

This function is the equivalent to `sendto()`. It returns the number of bytes which was sent; usually, it's the value
of `size`.

`sfd` is the *Socket File Descriptor* (every socket file descriptor argument in libsocket is called `sfd`) which
you got from `create_inet_dgram_socket()`. *The usage with STREAM sockets is not recommended and the result is undefined!*

`buf` is a pointer to some data.

`size` is the length of the buffer to which buf points.

`host` is the host to which we want to send the data. It's a string so you may specify everything what's resolved by
`getaddrinfo()`, i.e. an IP ("193.21.34.21") or a hostname ("example.net").

`service` is the port on the remote host. Like in `host`, you may specify the port either as number ("123") or as service string
("ntp", "http", "gopher").

`sendto_flags` is available on all platforms. The value given here goes directly to the internal `sendto()` call. The
flags which may be specified differ between the platforms. On Linux, you can get them from the manpage `sendto(2)`:

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

`recvfrom_inet_dgram_socket()` is the equivalent to `recvfrom()`. It receives data from the socket `sfd`,
places the data into the buffer `buffer` (which is `size` bytes long), and places the host from which the
data comes to the `src_host_len` bytes long `src_host` buffer. The source port is placed to `src_service`, with a maximum
length of `src_service_len`. `recvfrom_flags` are given to `recvfrom()` (described below) and `numeric` may be
`NUMERIC` (defined in header files) - this leads to numerical values in `src_host` and `src_service`.

`recvfrom_flags`


	extern int connect_inet_dgram_socket(int sfd, char* host, char* service);
	extern int destroy_inet_socket(int sfd);
	extern int shutdown_inet_stream_socket(int sfd, int method);
	extern int create_inet_server_socket(const char* bind_addr, const char* bind_port, char proto_osi4, char proto_osi3);
	extern int accept_inet_stream_socket(int sfd, char* src_host, size_t src_host_len, char* src_service, size_t src_service_len, int flags);

