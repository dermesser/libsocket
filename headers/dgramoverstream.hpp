#ifndef LIBSOCKET_DGRAMOVERSTREAM_H_7854202d13e741e98bb3b084eb3d6bc0
#define LIBSOCKET_DGRAMOVERSTREAM_H_7854202d13e741e98bb3b084eb3d6bc0

# include "exception.hpp"
# include "framing.hpp"
# include "socket.hpp"
# include "streamclient.hpp"

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
     * Wraps a stream socket and provides a message API on top of it.
     *
     * Inner has to implement stream and socket methods; those are: snd(), rcv(), setsockopt().
     * It may not be non-blocking, as EWOULDBLOCK is not handled gracefully.
     *
     * This means that if you use sndmsg() to send a frame, then the entire frame will be delivered;
     * and the receiver will (provided it uses a dgram_over_stream socket as well) receive only the entire
     * message (not parts of it).
     *
     * The internally used format is relatively simple; it uses NBO (big-endian) fixed-size 32bit integers as
     * prefix. The prefix encodes how many bytes are coming after it. This means that the maximum message length
     * is 4GiB.
     *
     * Schema: [4* u8, *u8]
     *
     * By default, Nagle's algorithm is disabled on the inner stream. This is necessary so that a message frame
     * is sent as soon as it is written to the socket. If you send a lot of small messages and can accept smaller delays,
     * you can enable it again using enable_nagle().
     *
     * THIS CLASS IS NOT THREADSAFE.
     */
    template<typename Inner>
    class dgram_over_stream {
        public:
            dgram_over_stream(void) = delete;
            dgram_over_stream(Inner inner);


            void enable_nagle(bool enable) const;

            ssize_t sndmsg(const void* buf, size_t len);
            ssize_t rcvmsg(void* dst, size_t len);

        private:
            static const size_t devnull_size = 256;

            // The underlying stream.
            Inner inner;
            char prefix_buffer[FRAMING_PREFIX_LENGTH];
            char devnull[devnull_size];
    };

    template<typename Inner>
    dgram_over_stream<Inner>::dgram_over_stream(Inner inner)
        : inner(inner)
    {
        enable_nagle(false);
    }
    
    /**
     * @brief Set TCP_NODELAY to `!enabled` on the underlying socket.
     *
     * TCP_NODELAY causes writes to the socket to be pushed to the network immediately. This
     * emulates the behavior of datagram sockets, and is very useful for datagram-like use of
     * streams, like this class implements. However, it creates slight overhead as data are not
     * batched.
     *
     * (clarification: If Nagle's algorithm is *enabled*, that means that `TCP_NODELAY` is *disabled*,
     * and vice versa)
     */
    template<typename Inner>
    void dgram_over_stream<Inner>::enable_nagle(bool enabled) const {
        int enabled_ = int(!enabled);
        inner.set_sock_opt(IPPROTO_TCP, TCP_NODELAY, &enabled_, sizeof(int));
    }

    /**
     * @brief Send the message in buf with length len as one frame.
     * @returns The total number of bytes sent.
     * @throws A socket_exception.
     */
    template<typename Inner>
    ssize_t dgram_over_stream<Inner>::sndmsg(const void* buf, size_t len)
    {
        encode_uint32(uint32_t(len), prefix_buffer);
        ssize_t result = inner.snd(prefix_buffer, FRAMING_PREFIX_LENGTH, 0);

        if (result < 0)
            return result;

        result = inner.snd(buf, len, 0);

        if (result < 0)
            return result;

        return result;
    }

    /**
     * @brief Receive a message and store the first `len` bytes into `buf`.
     * @returns The number of bytes received.
     * @throws A socket_exception.
     *
     * Bytes in the message beyond `len` are discarded.
     */
    template<typename Inner>
    ssize_t dgram_over_stream<Inner>::rcvmsg(void* dst, size_t len)
    {
        ssize_t result = inner.rcv(prefix_buffer, FRAMING_PREFIX_LENGTH, 0);

        if (result < 0)
            throw socket_exception(__FILE__, __LINE__, "dgram_over_stream::rcvmsg(): Could not receive length prefix!", false);

        uint32_t expected = decode_uint32(prefix_buffer);

        if (len >= expected)
        {
            result = inner.rcv(dst, expected, 0);

            if (result < 0)
                throw socket_exception(__FILE__, __LINE__, "dgram_over_stream::rcvmsg(): Could not receive message!", false);
        } else
        {
            result = inner.rcv(dst, expected, 0);

            if (result < 0)
                throw socket_exception(__FILE__, __LINE__, "dgram_over_stream::rcvmsg(): Could not receive message!", false);

            // Ignore rest of message.
            ssize_t rest_len = expected - len;

            do {
                size_t to_receive;

                if (rest_len > devnull_size)
                    to_receive = devnull_size;

                ssize_t recvd = inner.rcv(devnull, to_receive, 0);

                if (recvd < 0)
                    return result;

                rest_len -= recvd;
            } while (rest_len > 0);
        }
        return result;
    }
}

#endif
