#pragma once

#include "VulkanHelper.h"

namespace Engine {
    class BufferHandler {
    private:
        static uint32_t
        findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    public:
        static void
        createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                     VkMemoryPropertyFlags properties,
                     VkBuffer &buffer, VkDeviceMemory &bufferMemory);


        static void copyBuffer(VkBuffer srcBuffer,
                               VkBuffer dstBuffer, VkDeviceSize size);

        static AllocatedBuffer createStagingBuffer(VkDeviceSize size);

        static AllocatedBuffer createStagingBuffer(VkDeviceSize size, void *bufferData);

        static AllocatedBuffer createGpuBuffer(VkDeviceSize size, VkBufferUsageFlags usage);

        static AllocatedBuffer createGpuBuffer(VkDeviceSize size, VkBufferUsageFlags usage, void *bufferData);
    };
}
