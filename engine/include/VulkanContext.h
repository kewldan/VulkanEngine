#pragma once

#include <vulkan/vulkan.h>

namespace Engine {
    struct VulkanContext {
        VkInstance instance;
        VkPhysicalDevice physicalDevice;
        VkDevice device;
        VkQueue graphicsQueue, presentQueue;
        VkCommandPool commandPool;
        VkPipelineLayout pipelineLayout;
        VkRenderPass renderPass;
        VkExtent2D *swapChainExtent;
        VkDescriptorSetLayout descriptorSetLayout;
        VkDescriptorPool descriptorPool;
        int *currentFrame;
    };
}
