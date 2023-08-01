#pragma once

#include "VulkanHelper.h"

namespace Engine {
    class VulkanContext {
    public:
        static VkInstance instance;
        static VkPhysicalDevice physicalDevice;
        static VkDevice device;
        static VkSurfaceKHR surface;
        static VkRenderPass renderPass;
        static VkExtent2D swapChainExtent;
        static VkDescriptorPool descriptorPool;
        static VkCommandPool commandPool;
        static VkQueue graphicsQueue, presentQueue;
        static VmaAllocator allocator;
        static VkSampleCountFlagBits msaaSamples;
        static int currentFrame;
        static std::vector<VkCommandBuffer> commandBuffers;
    };
}
