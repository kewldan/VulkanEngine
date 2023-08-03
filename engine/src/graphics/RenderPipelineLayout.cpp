#include <stdexcept>
#include "graphics/RenderPipelineLayout.h"

#include "graphics/VulkanContext.h"

namespace Engine {
    void RenderPipelineLayout::build() {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = descriptorSetLayouts.size();
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
        pipelineLayoutInfo.pPushConstantRanges = ranges.data();
        pipelineLayoutInfo.pushConstantRangeCount = ranges.size();

        if (vkCreatePipelineLayout(Engine::VulkanContext::device, &pipelineLayoutInfo, nullptr, &layout) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    VkPipelineLayout RenderPipelineLayout::getLayout() const {
        return layout;
    }

    void RenderPipelineLayout::destroy() {

        for (VkDescriptorSetLayout set: descriptorSetLayouts) {
            vkDestroyDescriptorSetLayout(Engine::VulkanContext::device, set, nullptr);
        }

        vkDestroyPipelineLayout(Engine::VulkanContext::device, layout, nullptr);
    }

    const std::vector<VkDescriptorSet> *RenderPipelineLayout::getDescriptorSets() const {
        return descriptorSets;
    }
} // Engine