#include "graphics/Shader.h"
#include "io/Filesystem.h"
#include "graphics/VulkanContext.h"

namespace Engine {
    void Shader::destroy() {
        vkDestroyShaderModule(Engine::VulkanContext::device, module, nullptr);
    }

    void Shader::load() {
        size_t length;
        auto code = Filesystem::readFile(filename, &length);

        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = length;
        createInfo.pCode = reinterpret_cast<const uint32_t *>(code);

        if (vkCreateShaderModule(VulkanContext::device, &createInfo, nullptr, &module) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module!");
        }
    }

    Shader::Shader(const char *filename) : filename(filename) {}

    VkShaderModule Shader::getModule() const {
        return module;
    }

    Shader::Shader() = default;
} // Engine