#ifndef VULKANENGINE_BASE64_H
#define VULKANENGINE_BASE64_H

namespace Engine {
    class Base64 {
    public:
        static char *encode(const char *binary, size_t size);

        static char *decode(const char *base64, size_t *size);
    };
} // Engine

#endif //VULKANENGINE_BASE64_H
