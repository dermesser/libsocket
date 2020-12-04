#include <cstring>
#include <iostream>
#include "../../headers/exception.hpp"
#include "../../headers/inetserverdgram.hpp"

// Server for echo_client_*.cpp
// Simply receives a datagram message and sends back an answer.

int main(void) {
    using std::string;

    string host = "localhost";
    string port = "1234";

    string answer("Hello back from the server!");
    string from;
    string fromport;
    string buf;

    buf.resize(32);

    try {
        libsocket::inet_dgram_server srv(host, port, LIBSOCKET_IPv4);

        for (;;) {
            srv.rcvfrom(buf, from, fromport);

            std::cout << "Datagram from " << from << ":" << fromport << " "
                      << buf << std::endl;

            srv.sndto(answer, from, fromport);
        }

        // libsocket::inet_dgram_server also has a destructor doing this for
        // us, so we are doing explicitly and can reuse the socket.
        srv.destroy();
    } catch (const libsocket::socket_exception& exc) {
        std::cerr << exc.mesg;
    }

    return 0;
}
