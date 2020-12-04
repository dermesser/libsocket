#include <iostream>
#include <string>
#include "../../headers/exception.hpp"
#include "../../headers/inetclientstream.hpp"

#include <stdlib.h>
#include <unistd.h>

/*
 * This program connects to the server.cpp binary, listens for a message,
 * then sends "Hello back!" to the server, closing the socket afterwards.
 */

int main(void) {
    using std::string;

    using libsocket::inet_stream;

    string host = "::1";
    string port = "1235";
    string answer;

    answer.resize(32);

    try {
        libsocket::inet_stream sock(host, port, LIBSOCKET_IPv6);

        sock >> answer;

        std::cout << answer;

        sock << "Hello back!\n";

        // sock is closed here automatically!
    } catch (const libsocket::socket_exception& exc) {
        std::cerr << exc.mesg;
    }

    return 0;
}
