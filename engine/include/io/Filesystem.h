#pragma once

#include <filesystem>

namespace Engine {
    class Filesystem {
    public:
        static char *readFile(const char *path, size_t *size = nullptr);

        static char *readResourceFile(const char *path, size_t *size = nullptr);

        static bool writeFile(const char *path, const char *data, size_t size);

        static char *readString(const char *path);

        static char *readResourceString(const char *path);

        static bool writeString(const char *path, const char *data);

        static bool exists(const char *path);

        static bool resourceExists(const char *path);

        static std::filesystem::path getWorkingPath();

        static unsigned char *compress(unsigned char *data, size_t length, size_t *compressedLength);

        static unsigned char *decompress(unsigned char *data, size_t length, size_t *decompressedLength);
    };
}