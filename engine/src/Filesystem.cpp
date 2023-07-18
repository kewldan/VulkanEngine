#include "Filesystem.h"
#include <Windows.h>
#include "zlib.h"
#include "plog/Log.h"
#include <fstream>

namespace Engine {
    char *Filesystem::readFile(const char *path, size_t *size) {
        assert(path != nullptr);
        std::ifstream stream(path, std::ios::out | std::ios::binary);
        if (!stream) {
            PLOGW << "The requested file [" << path << "] does not exist";
            return nullptr;
        }
        int s = (int) std::filesystem::file_size(path);
        if (!s) {
            PLOGW << "The requested file [" << path << "] does not exist";
            return nullptr;
        }
        char *bin = new char[s + s % sizeof(uint32_t)];
        stream.read(bin, s);
        stream.close();
        if (size != nullptr) {
            *size = s;
        }
        return bin;
    }

    bool Filesystem::writeFile(const char *path, const char *data, size_t size) {
        assert(path != nullptr);
        assert(data != nullptr);
        assert(size > 0);
        std::ofstream stream(path, std::ios::out | std::ios::binary);
        stream.write(data, size);
        stream.close();
        return stream.good();
    }

    bool Filesystem::exists(const char *path) {
        return std::filesystem::exists(path);
    }

    char *Filesystem::readString(const char *path) {
        std::ifstream stream(path, std::ios::out | std::ios::binary);
        if (!stream) {
            PLOGW << "The requested string [" << path << "] does not exist";
            return nullptr;
        }
        uintmax_t size = std::filesystem::file_size(path);
        char *bin = new char[size + 1ULL + size % sizeof(uint32_t)];
        assert(bin != nullptr);
        stream.read(bin, (long long) size);
        stream.close();
        bin[size] = 0;
        return bin;
    }

    bool Filesystem::writeString(const char *path, const char *data) {
        return Filesystem::writeFile(path, data, strlen(data));
    }

    char *Filesystem::readResourceFile(const char *path, size_t *size) {
        assert(path != nullptr);
        auto myResource = ::FindResource(nullptr, path, RT_RCDATA);
        if (!myResource) {
            PLOGW << "The requested resource file [" << path << "] does not exist";
            return nullptr;
        }
        auto myResourceData = ::LoadResource(nullptr, myResource);
        if (size != nullptr)
            *size = (int) ::SizeofResource(nullptr, myResource);
        return static_cast<char *>(::LockResource(myResourceData));
    }

    char *Filesystem::readResourceString(const char *path) {
        assert(path != nullptr);
        auto myResource = ::FindResource(nullptr, path, RT_RCDATA);
        if (!myResource) {
            PLOGW << "The requested resource string [" << path << "] does not exist";
            return nullptr;
        }
        auto myResourceData = ::LoadResource(nullptr, myResource);
        auto pMyBinaryData = ::LockResource(myResourceData);
        DWORD size = ::SizeofResource(nullptr, myResource);
        char *str = new char[size + 1];
        assert(str != nullptr);
        str[size] = 0;
        memcpy(str, pMyBinaryData, size);
        return str;
    }

    bool Filesystem::resourceExists(const char *path) {
        return ::FindResourceA(nullptr, path, RT_RCDATA);
    }

    unsigned char *Filesystem::compress(unsigned char *data, size_t length, size_t *compressedLength) {
        assert(data != nullptr);
        assert(length > 0);
        auto *deflated = new unsigned char[length];
        z_stream defstream;
        defstream.zalloc = Z_NULL;
        defstream.zfree = Z_NULL;
        defstream.opaque = Z_NULL;

        defstream.avail_in = length;
        defstream.next_in = data;
        defstream.avail_out = length;
        defstream.next_out = deflated;

        deflateInit(&defstream, Z_BEST_COMPRESSION);
        deflate(&defstream, Z_FINISH);
        deflateEnd(&defstream);

        if (compressedLength != nullptr)
            *compressedLength = defstream.total_out;

        return deflated;
    }

    unsigned char *Filesystem::decompress(unsigned char *data, size_t length, size_t *decompressedLength) {

        z_stream infstream;
        infstream.zalloc = Z_NULL;
        infstream.zfree = Z_NULL;
        infstream.opaque = Z_NULL;

        infstream.next_in = data;
        infstream.avail_in = length;

        auto *inflated = (unsigned char *) malloc(128);
        infstream.next_out = inflated;

        inflateInit(&infstream);
        for (int i = 0;; i++) {
            infstream.avail_out = 64;
            infstream.next_out = inflated + 64 * i;
            int err = inflate(&infstream, Z_SYNC_FLUSH);
            if (err == Z_STREAM_END) break;
            inflated = static_cast<unsigned char *>(realloc(inflated, 128 + 64 * i));
        }
        inflateEnd(&infstream);


        if (decompressedLength != nullptr)
            *decompressedLength = infstream.total_out;

        return inflated;
    }

    std::filesystem::path Filesystem::getWorkingPath() {
        return std::filesystem::current_path();
    }
}