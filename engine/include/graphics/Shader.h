#ifndef VULKANENGINE_SHADER_H
#define VULKANENGINE_SHADER_H

#include "io/Asset.h"
#include "misc/Destroyable.h"
#include <vulkan/vulkan.h>

namespace Engine {
    class Shader : public Destroyable, public Asset {
    private:
        const char *filename{};
        VkShaderModule module = VK_NULL_HANDLE;
    public:
        Shader();

        explicit Shader(const char *filename);

        void load() override;

        void destroy() override;

        [[nodiscard]] VkShaderModule getModule() const;
    };
} // Engine

#endif //VULKANENGINE_SHADER_H
