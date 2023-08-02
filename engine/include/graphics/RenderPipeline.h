#ifndef VULKANENGINE_RENDERPIPELINE_H
#define VULKANENGINE_RENDERPIPELINE_H

#include <vulkan/vulkan.h>
#include <functional>
#include "misc/Destroyable.h"
#include "common/GameObject.h"

namespace Engine {
    class RenderPipeline : public Destroyable {
    protected:
        VkPipeline pipeline = VK_NULL_HANDLE;
    public:
        std::function<void(const GameObject &)> renderFunction;

        virtual void init(const std::function<void(const GameObject &)> &func);

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

        virtual void updateViewportScissor();

        virtual void build(VkPipelineLayout layout) = 0;

        [[nodiscard]] virtual VkPipeline getPipeline() const;

        virtual void bind();

        void destroy() override;
    };
}

#endif //VULKANENGINE_RENDERPIPELINE_H
