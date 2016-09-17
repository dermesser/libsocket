# include <iostream>
# include <stdio.h>
# include <unistd.h>
# include <stdint.h>
# include <string.h>

# include <libsocket/framing.hpp>

int main(void)
{
    char dest[4];
    memset(dest, 0, 4);
    uint32_t number = (1 << 30) + 123;
    uint32_t decoded;

    libsocket::encode_uint32(number, dest);

    if (number != (decoded = libsocket::decode_uint32(dest))) {
        std::cout << "Failure: " << number << " " << decoded << std::endl;

        for (int i = 0; i < 4; i++) {
            printf("%02hhx", dest[i]);
        }
        printf("\n");
        printf("%x\n", decoded);
    }

    return 0;
}
