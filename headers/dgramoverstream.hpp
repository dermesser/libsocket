#ifndef LIBSOCKET_DGRAMOVERSTREAM_H_7854202d13e741e98bb3b084eb3d6bc0
#define LIBSOCKET_DGRAMOVERSTREAM_H_7854202d13e741e98bb3b084eb3d6bc0

# include "exception.hpp"
# include "framing.hpp"
# include "socket.hpp"
# include "streamclient.hpp"

# include <string>
# include <vector>
# include <memory>


# include <netinet/ip.h>
# include <netinet/tcp.h>

/**
 * @file dgramoverstream.hpp
 *
 * Contains a class that can be used as wrapper around stream sockets and provides framing. That is,
 * an API allowing applications to send discrete messages.
 */
/*
   The committers of the libsocket project, all rights reserved
   (c) 2016, dermesser <lbo@spheniscida.de>

   Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
   following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
   disclaimer.
   2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
   disclaimer in the documentation and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
   NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
   EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
   OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.

*/

namespace libsocket
{
        /**
         * @brief Wraps a stream socket and provides a message-based API on top of it.
         *
         * Inner has to implement stream and socket methods; those are: snd(), rcv(), setsockopt().
         * It may not be non-blocking, as EWOULDBLOCK is not handled gracefully.
         *
         * This means that if you use sndmsg() to send a frame, then the entire frame will be delivered;
         * and the receiver will (provided it uses a dgram_over_stream socket as well) receive only the entire
         * message (not parts of it).
         *
         * The internally used format is relatively simple; it uses NBO (big-endian) fixed-size 32bit integers as
         * prefix. The prefix encodes how many bytes are coming after it. The maximum supported frame size is 2GiB.
         * Schema: [4* u8, *u8]
         *
         * By default, Nagle's algorithm is disabled on the inner stream. This is necessary so that a message frame
         * is sent as soon as it is written to the socket. If you send a lot of small messages and can accept smaller delays,
         * you can enable it again using enable_nagle().
         *
         * THIS CLASS IS NOT THREADSAFE.
         *
         * AS THE STREAM SOCKET WILL BE CLOSED ON DESTRUCTION, IT IS NOT PERMITTED TO USE A dgram_over_stream OUTSIDE THE SCOPE
         * OF THE ORIGINAL SOCKET.
         *
         * THIS CLASS IS IN BETA STATE: IT HAS NOT BEEN TESTED EXTENSIVELY, BUT IS EXPECTED TO WORK.
         */
        class dgram_over_stream {
                public:
                        dgram_over_stream(void) = delete;
                        dgram_over_stream(const dgram_over_stream&) = delete;
                        dgram_over_stream(stream_client_socket inner);
                        dgram_over_stream(std::unique_ptr<stream_client_socket> inner);

                        void enable_nagle(bool enable) const;

                        ssize_t sndmsg(const void* buf, size_t len);
                        ssize_t rcvmsg(void* dst, size_t len);

                        ssize_t sndmsg(const std::string& msg);
                        ssize_t rcvmsg(std::string* dst);

                        ssize_t sndmsg(const std::vector<uint8_t>& msg);
                        ssize_t rcvmsg(std::vector<uint8_t>* dst);

                private:
                        static const size_t RECV_BUF_SIZE = 256;

                        // The underlying stream.
                        std::unique_ptr<stream_client_socket> inner;
                        char prefix_buffer[FRAMING_PREFIX_LENGTH];
                        char RECV_BUF[RECV_BUF_SIZE];

                        ssize_t receive_bytes(size_t);
                        uint32_t receive_header(void);
        };
}

#endif
