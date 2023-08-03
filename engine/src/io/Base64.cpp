#include <cassert>
#include "io/Base64.h"

#include <libbase64.h>
#include <cstring>

namespace Engine {
    char *Base64::encode(const char *binary, size_t size) {
        assert(binary != nullptr);
        assert(size >= 0);

        char *base64 = new char[size * 2];

        size_t outlen = 0;
        base64_encode(binary, size, base64, &outlen, 0);
        base64[outlen] = 0;

        return base64;
    }

    char *Base64::decode(const char *base64, size_t *size) {
        assert(base64 != nullptr);
        assert(size != nullptr);

        char *binary = new char[strlen(base64) * 2];
        memset(binary, 0, strlen(base64) * 2);
        base64_decode(base64, strlen(base64), binary, size, 0);

        return binary;
    }
} // Engine