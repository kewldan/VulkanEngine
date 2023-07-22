#include "io/Assets.h"
#include "io/Filesystem.h"

namespace Engine {

    Mesh *Assets::loadMeshes(const char *filename, const int *count) {
        assert(filename != nullptr);
        assert(count != nullptr);

        // TODO: write code

        return nullptr;
    }

    VkShaderModule Assets::loadShader(VkDevice device, const char *filename) {
        assert(device != VK_NULL_HANDLE);
        assert(filename != nullptr);

        size_t length;
        auto code = Filesystem::readFile(filename, &length);

        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = length;
        createInfo.pCode = reinterpret_cast<const uint32_t *>(code);

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module!");
        }

        return shaderModule;
    }
}