# include <iostream>
# include <stdio.h>
# include <string>
# include <string.h>
# include <assert.h>

# include <libsocket/exception.hpp>
# include <libsocket/dgramoverstream.hpp>
# include <libsocket/inetclientstream.hpp>
# include <libsocket/inetserverstream.hpp>

/*
 * This example demonstrates the use of the dgram_over_stream class. As you can read in
 * that classes documentation, it simplifies sending discrete packets of data over a
 * stream connection. This is achieved by framing; concretely, the length of a packet
 * is sent first, so the receiver knows how many bytes to expect.
 *
 * Usage:
 *   ./dgram_over_stream -c # client
 *   ./dgram_over_stream -s # server
 */

using std::string;

static const string HOST = "localhost";
static const string PORT = "4445";

void run_client(void);
void run_server(void);

enum MODE {
    MODE_CLIENT,
    MODE_SERVER,
};

MODE get_mode(char** argv) {
    if (string(argv[1]) == "-c") {
        return MODE_CLIENT;
    } else if (string(argv[1]) == "-s") {
        return MODE_SERVER;
    } else {
        perror("Bad command line; please use either -s or -c");
        exit(1);
    }
}

int main(int argc, char** argv)
{
    assert(argc > 1);

    MODE mode = get_mode(argv);

    try {
        if (mode == MODE_CLIENT) {
            run_client();
        } else if (mode == MODE_SERVER) {
            run_server();
        }
    } catch (libsocket::socket_exception e) {
        std::cerr << e.mesg << std::endl;
    }

    return 0;
}

void run_client(void) {
    static const size_t bufsize = 1024;
    char buf[bufsize];
    memset(buf, 0, bufsize);

    libsocket::inet_stream client(HOST, PORT, LIBSOCKET_IPv4);
    libsocket::dgram_over_stream dgram_cl(client);

    dgram_cl.sndmsg("Hello", 5);
    std::cout << "Client received " << dgram_cl.rcvmsg(buf, bufsize) << " bytes.\n";
    std::cout << buf << std::endl;

    return;
}

void run_server(void) {
    static const size_t bufsize = 1024;
    char buf[bufsize];
    memset(buf, 0, bufsize);

    libsocket::inet_stream_server srv(HOST, PORT, LIBSOCKET_IPv4);

    while (true) {
        libsocket::inet_stream* client = srv.accept(1);
        libsocket::dgram_over_stream dgram_cl(*client);
        ssize_t len = 0;

        std::cout << "Server received " << (len = dgram_cl.rcvmsg(buf, bufsize)) << " bytes.\n";
        std::cout << buf << std::endl;
        dgram_cl.sndmsg("Hello back", 10);

        memset(buf, 0, len);
    }

    return;
}
