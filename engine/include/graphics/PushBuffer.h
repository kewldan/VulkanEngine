#ifndef VULKANENGINE_PUSHBUFFER_H
#define VULKANENGINE_PUSHBUFFER_H

#include "VulkanContext.h"

namespace Engine {
    template<typename T>
    class PushBuffer : public T {
    public:
        PushBuffer() = default;

        VkPushConstantRange getCreationInfo(VkShaderStageFlags stageBits);

        void upload(VkPipelineLayout pipelineLayout);
    };

    template<typename T>
    VkPushConstantRange PushBuffer<T>::getCreationInfo(VkShaderStageFlags stageBits) {
        return {stageBits, 0, sizeof(T)};
    }

    template<typename T>
    void PushBuffer<T>::upload(VkPipelineLayout pipelineLayout) {
        vkCmdPushConstants(VulkanContext::commandBuffers[VulkanContext::currentFrame], pipelineLayout,
                           VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(T),
                           this);
    }
}

#endif //VULKANENGINE_PUSHBUFFER_H
