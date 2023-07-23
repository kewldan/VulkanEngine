#pragma once

#include "VulkanHelper.h"

namespace Engine {
    class BufferHandler {
    private:
        static uint32_t
        findMemoryType(VkPhysicalDevice vkPhysicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

    public:
        static void
        createBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage,
                     VkMemoryPropertyFlags properties,
                     VkBuffer &buffer, VkDeviceMemory &bufferMemory);


        static void copyBuffer(VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, VkBuffer srcBuffer,
                               VkBuffer dstBuffer, VkDeviceSize size);
    };
}
