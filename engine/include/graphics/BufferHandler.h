#pragma once

#include "VulkanHelper.h"

namespace Engine {
    class BufferHandler {
    private:
        static void copyBuffer(VkBuffer srcBuffer,
                               VkBuffer dstBuffer, VkDeviceSize size);

    public:
        static AllocatedBuffer createUniformBuffer(VkDeviceSize size, void **mapped);

        static AllocatedBuffer createStagingBuffer(VkDeviceSize size);

        static AllocatedBuffer createStagingBuffer(VkDeviceSize size, void *bufferData);

        static AllocatedBuffer createGpuBuffer(VkDeviceSize size, VkBufferUsageFlags usage);

        static AllocatedBuffer createGpuBuffer(VkDeviceSize size, VkBufferUsageFlags usage, void *bufferData);
    };
}
