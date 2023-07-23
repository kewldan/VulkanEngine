#pragma once

#include "VulkanHelper.h"

namespace Engine {
    struct VulkanContext {
        VkInstance instance;
        VkPhysicalDevice physicalDevice;
        VkDevice device;
        VkQueue graphicsQueue, presentQueue;
        VkCommandPool commandPool;
        VkRenderPass renderPass;
        VkExtent2D *swapChainExtent;
        VkDescriptorPool descriptorPool;
        int *currentFrame;
    };
}
