#include "graphics/VulkanContext.h"

namespace Engine {
    VmaAllocator VulkanContext::allocator = nullptr;
    VkPhysicalDevice VulkanContext::physicalDevice = VK_NULL_HANDLE;
    VkDevice VulkanContext::device = VK_NULL_HANDLE;
    RenderPass VulkanContext::renderPass;
    VkCommandPool VulkanContext::commandPool = VK_NULL_HANDLE;
    VkDescriptorPool VulkanContext::descriptorPool = VK_NULL_HANDLE;
    VkSampleCountFlagBits VulkanContext::msaaSamples = VK_SAMPLE_COUNT_1_BIT;
    int VulkanContext::currentFrame = 0;
    VkSurfaceKHR VulkanContext::surface = VK_NULL_HANDLE;
    VkInstance VulkanContext::instance = VK_NULL_HANDLE;
    VkQueue VulkanContext::graphicsQueue = VK_NULL_HANDLE;
    VkQueue VulkanContext::presentQueue = VK_NULL_HANDLE;
    VkExtent2D VulkanContext::swapChainExtent{};
    CommandBuffers VulkanContext::commandBuffers;
    VkPipelineCache VulkanContext::pipelineCache = VK_NULL_HANDLE;
    SwapChain VulkanContext::swapchain{};
    QueueFamilyIndices VulkanContext::indices{};
}