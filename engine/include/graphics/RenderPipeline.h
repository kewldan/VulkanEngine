#ifndef VULKANENGINE_RENDERPIPELINE_H
#define VULKANENGINE_RENDERPIPELINE_H

#include <vulkan/vulkan.h>

namespace Engine {
    class RenderPipeline {
    public:
        virtual VkPipelineShaderStageCreateInfo *
        getShaderStages(uint32_t *count, VkShaderModule vertex, VkShaderModule fragment);

        virtual VkPipelineInputAssemblyStateCreateInfo getInputAssembly();

        virtual VkPipelineViewportStateCreateInfo getViewportStateInfo();

        virtual VkPipelineRasterizationStateCreateInfo getRasterizer();

        virtual const VkPipelineColorBlendStateCreateInfo *getColorBlending();

        virtual VkPipelineDynamicStateCreateInfo getDynamicStates();

        virtual VkPipelineMultisampleStateCreateInfo getMultisampling(VkSampleCountFlagBits msaa);

        virtual const VkPipelineVertexInputStateCreateInfo *getVertexInputState();

        virtual VkPipelineDepthStencilStateCreateInfo getDepthState();

        virtual VkPipeline build(VkPipelineLayout layout) = 0;
    };
}

#endif //VULKANENGINE_RENDERPIPELINE_H
