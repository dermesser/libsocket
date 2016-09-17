# include <string>
# include <string.h>

# include <framing.hpp>

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
 * @file framing.cpp
 * @brief Utility functions for framing.
 *
 * @addtogroup libsocketplusplus
 * @{
 */

namespace libsocket
{
    void encode_uint32(uint32_t n, char* dst)
    {
        for ( int i = 3; i >= 0; i-- )
        {
            dst[i] = n >> (8 * (3 - i));
        }
    }

    uint32_t decode_uint32(const char* src)
    {
        uint32_t result = 0;
        // We store unsigned numbers in signed chars; convert, otherwise the MSB being
        // set would be interpreted as sign and taken over to uint32_t's MSB.
        const unsigned char* src_ = (const unsigned char*)src;

        for ( int i = 3; i >= 0; i-- )
        {
            result |= uint32_t(src_[i]) << (8 * (3 - i));
        }

        return result;
    }
}

/**
 * @}
 */
