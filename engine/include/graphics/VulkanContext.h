#pragma once

#include "VulkanHelper.h"
#include "CommandBuffers.h"
#include "RenderPass.h"

namespace Engine {
    class VulkanContext {
    public:
        static VkInstance instance;
        static VkPhysicalDevice physicalDevice;
        static VkDevice device;
        static VkSurfaceKHR surface;
        static RenderPass renderPass;
        static VkExtent2D swapChainExtent;
        static VkDescriptorPool descriptorPool;
        static VkCommandPool commandPool;
        static VkQueue graphicsQueue, presentQueue;
        static VmaAllocator allocator;
        static VkSampleCountFlagBits msaaSamples;
        static int currentFrame;
        static CommandBuffers commandBuffers;
        static VkPipelineCache pipelineCache;
    };
}
