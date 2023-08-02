#ifndef VULKANENGINE_RENDERPASS_H
#define VULKANENGINE_RENDERPASS_H

#include <vulkan/vulkan.h>
#include "misc/Destroyable.h"

namespace Engine {
    class RenderPass : public Destroyable {
    private:
        VkRenderPass renderPass = VK_NULL_HANDLE;
    public:
        void init(VkFormat swapChainImageFormat);

        VkRenderPass getRenderPass();

        void destroy() override;
    };
} // Engine

#endif //VULKANENGINE_RENDERPASS_H
