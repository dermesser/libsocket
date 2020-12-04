#include <string.h>
#include <iostream>
#include <string>
#include "../../headers/exception.hpp"
#include "../../headers/unixclientstream.hpp"

// Builds a connection to /tmp/unixsocket and sends/receives data using it.

int main(void) {
    using libsocket::unix_stream_client;
    using std::string;

    string path = "/tmp/unixsocket";
    char* answer = new char[128];

    memset(answer, 0, 128);

    try {
        unix_stream_client sock(path);

        sock.snd("Hello World!\n", 13);
        sock.rcv(answer, 127);

        std::cout << answer;
    } catch (const libsocket::socket_exception& exc) {
        std::cerr << exc.mesg;
    }

    return 0;
}
