#include <unistd.h>
#include <iostream>
#include "../../headers/exception.hpp"
#include "../../headers/inetclientdgram.hpp"

/*
 * Sends and receives messages to/from the echo_server via connected UDP sockets
 */

int main(void) {
    using std::string;

    string host = "localhost";
    string port = "1234";

    string answer;

    answer.resize(32);

    libsocket::inet_dgram_client sock(LIBSOCKET_IPv4);

    try {
        std::cout << sock.gethost();
    } catch (const libsocket::socket_exception& exc) {
        std::cerr << exc.mesg;
    }

    try {
        for (int i = 0; i < 20; i++) {
            sock.connect(host, port);

            sock << "Hello, server";

            sock >> answer;

            std::cout << "Answer from server: " << answer << std::endl;

            sock.deconnect();
        }
    } catch (const libsocket::socket_exception& exc) {
        std::cerr << exc.mesg;
    }

    sock.destroy();

    return 0;
}
