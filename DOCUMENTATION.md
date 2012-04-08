# Documentation for libsocket
## Introduction

libsocket is a library written in C and designed to simplify the usage of `AF_INET/AF_INET6` (internet domain) and `AF_UNIX`
(unix domain) sockets. With many specialized functions in the API, it's hard to do things wrong. And with the excellent error
processing with optional verbosity, it's also quite easy to debug your applications.

libsocket is the name for the whole project. libinetsocket is the part for `INET` sockets, libunixsocket for `UNIX` sockets.

### libinetsocket

As the name says, libinetsocket is the part for internet domain sockets. All important features are supported and it's possible
to give flags to the underlying calls, e.g. it's possible to give the flag `MSG_DONTROUTE` to the wrapper function of `sendto()`.

### libunixsocket

libunixsocket is responsible for UNIX domain sockets. The calls are similar to the libinetsocket calls.

## API calls

This section explains the single API calls of libsocket.

### libinetsocket

#### `create_inet_stream_socket()`
`int create_inet_stream_socket(const char* host, const char* service, char proto_osi3, int flags)` (Linux)

`int create_inet_stream_socket(const char* host, const char* service, char proto_osi3)` (Others)

This function creates and returns a inet stream socket (TCP socket) which is connected to `host`:`service`.
`proto_osi3` is either `IPv4` or `IPv6` (defined in `libinetsocket.h`). `flags` is only available on Linux and allows to manipulate the `socket()` call. 
`socket(2)` says the following:

>Since Linux 2.6.27, the type argument serves a second purpose: in addition to specifying a socket type, it may include the bitwise OR of any of the following values, to modify the behavior
>of socket():
>
>	* `SOCK_NONBLOCK`   Set the `O_NONBLOCK` file status flag on the new open file description.  Using this flag saves extra calls to fcntl(2) to achieve the same result.
>
>	* `SOCK_CLOEXEC`    Set the close-on-exec (`FD_CLOEXEC`) flag on the new file descriptor.  See the description of the `O_CLOEXEC` flag in open(2) for reasons why this may be useful.



extern int create_inet_dgram_socket(char proto_osi3, int flags);
extern ssize_t sendto_inet_dgram_socket(int sfd,void* buf, size_t size,char* host, char* service, int sendto_flags);
extern ssize_t recvfrom_inet_dgram_socket(int sfd, void* buffer, size_t size, char* src_host, size_t src_host_len, char* src_service, size_t src_service_len, int recvfrom_flags, int numeric);
extern int connect_inet_dgram_socket(int sfd, char* host, char* service);
extern int destroy_inet_socket(int sfd);
extern int shutdown_inet_stream_socket(int sfd, int method);
extern int create_inet_server_socket(const char* bind_addr, const char* bind_port, char proto_osi4, char proto_osi3);
extern int accept_inet_stream_socket(int sfd, char* src_host, size_t src_host_len, char* src_service, size_t src_service_len, int flags);

# endif
