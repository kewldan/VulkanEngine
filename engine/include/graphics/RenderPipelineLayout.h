#ifndef VULKANENGINE_RENDERPIPELINELAYOUT_H
#define VULKANENGINE_RENDERPIPELINELAYOUT_H

#include <vector>
#include <vulkan/vulkan.h>

#include "graphics/PushBuffer.h"
#include "graphics/UniformBuffer.h"

namespace Engine {
    class RenderPipelineLayout : public Destroyable {
    private:
        std::vector<VkPushConstantRange> ranges;
        std::vector<VkDescriptorSetLayout> sets;
        VkPipelineLayout layout;
    public:

        template<typename T>
        UniformBuffer<T> createUniformBuffer(VkShaderStageFlagBits stageFlags, int binding);

        template<typename T>
        PushBuffer<T> createPushBuffer(VkShaderStageFlagBits shaderStage);

        void build();

        [[nodiscard]] VkPipelineLayout getLayout() const;

        void destroy() override;
    };

    template<typename T>
    PushBuffer<T> RenderPipelineLayout::createPushBuffer(VkShaderStageFlagBits shaderStage) {
        PushBuffer<T> buffer;

        ranges.push_back(buffer.getCreationInfo(shaderStage));

        return buffer;
    }

    template<typename T>
    UniformBuffer<T> RenderPipelineLayout::createUniformBuffer(VkShaderStageFlagBits stageFlags, int binding) {
        UniformBuffer<T> buffer;

        buffer.init(stageFlags, binding);
        sets.push_back(buffer.layout);

        return buffer;
    }
} // Engine

#endif //VULKANENGINE_RENDERPIPELINELAYOUT_H
