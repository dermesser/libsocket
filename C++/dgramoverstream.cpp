# include <string>
# include <unistd.h>
# include <string.h>

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

/**
 * @file dgramoverstream.cpp
 * @brief Simple framing over streams.
 * @addtogroup libsocketplusplus
 * @{
 */

# include <exception.hpp>
# include <dgramoverstream.hpp>

namespace libsocket {

    dgram_over_stream::dgram_over_stream(const stream_client_socket& inner)
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
    void dgram_over_stream::enable_nagle(bool enabled) const {
        int enabled_ = int(!enabled);
        inner.set_sock_opt(IPPROTO_TCP, TCP_NODELAY, (const char*)&enabled_, sizeof(int));
    }

    /**
     * @brief Send the message in buf with length len as one frame.
     * @returns The total number of bytes sent.
     * @throws A socket_exception.
     */
    ssize_t dgram_over_stream::sndmsg(const void* buf, size_t len)
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
    ssize_t dgram_over_stream::rcvmsg(void* dst, size_t len)
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
            size_t rest_len = expected - len;

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

/**
 * @}
 */
