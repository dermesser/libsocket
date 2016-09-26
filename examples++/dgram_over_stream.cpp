# include <algorithm>
# include <assert.h>
# include <iostream>
# include <stdio.h>
# include <string.h>
# include <string>
# include <vector>

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
void run_string_client(void);
void run_vec_client(void);
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
            run_string_client();
            run_vec_client();
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
    libsocket::dgram_over_stream dgram_cl(std::move(client));

    dgram_cl.sndmsg("Hello", 5);
    std::cout << "Client received " << dgram_cl.rcvmsg(buf, bufsize) << " bytes.\n";
    std::cout << buf << std::endl;

    return;
}

void run_string_client(void) {
    libsocket::inet_stream client(HOST, PORT, LIBSOCKET_IPv4);
    libsocket::dgram_over_stream dgram_cl(std::move(client));

    std::string recvbuf(0, 'a');
    recvbuf.resize(3);

    dgram_cl.sndmsg(std::string("Hello"));
    std::cout << "Client received " << dgram_cl.rcvmsg(&recvbuf) << " bytes into std::string.\n";
    std::cout << recvbuf << std::endl;

    return;
}

void run_vec_client(void) {
    libsocket::inet_stream client(HOST, PORT, LIBSOCKET_IPv4);
    libsocket::dgram_over_stream dgram_cl(std::move(client));

    std::vector<uint8_t> recvbuf;
    recvbuf.resize(15);

    dgram_cl.sndmsg(std::string("Hello"));
    std::cout << "Client received " << dgram_cl.rcvmsg(&recvbuf) << " bytes into std::vec.\n";

    std::for_each(recvbuf.begin(), recvbuf.end(), [](uint8_t b) { std::cout << static_cast<char>(b); });
    std::cout << std::endl;

    return;
}

void run_server(void) {
    static const size_t bufsize = 1024;
    char buf[bufsize];
    memset(buf, 0, bufsize);

    libsocket::inet_stream_server srv(HOST, PORT, LIBSOCKET_IPv4);

    while (true) {
        libsocket::inet_stream* client = srv.accept(1);
        libsocket::dgram_over_stream dgram_cl(std::move(*client));
        ssize_t len = 0;

        std::cout << "Server received " << (len = dgram_cl.rcvmsg(buf, bufsize)) << " bytes.\n";
        std::cout << buf << std::endl;
        dgram_cl.sndmsg("Hello back", 10);

        memset(buf, 0, len);
    }

    return;
}
