#include <stdexcept>
#include <cassert>
#include "graphics/BufferHandler.h"
#include "graphics/VulkanContext.h"

namespace Engine {
    void
    BufferHandler::copyBuffer(VkBuffer srcBuffer,
                              VkBuffer dstBuffer, VkDeviceSize size) {
        assert(srcBuffer != nullptr);
        assert(dstBuffer != nullptr);
        assert(size > 0);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = VulkanContext::commandPool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(VulkanContext::device, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        VkBufferCopy copyRegion{};
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(VulkanContext::graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(VulkanContext::graphicsQueue);

        vkFreeCommandBuffers(VulkanContext::device, VulkanContext::commandPool, 1, &commandBuffer);
    }

    AllocatedBuffer BufferHandler::createStagingBuffer(VkDeviceSize size) {
        assert(size > 0);

        AllocatedBuffer stagingBuffer{};

        VkBufferCreateInfo stagingBufferInfo = {};
        stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        stagingBufferInfo.pNext = nullptr;
        stagingBufferInfo.size = size;
        stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        VmaAllocationCreateInfo vmaallocInfo = {};
        vmaallocInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_HOST;
        vmaallocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;

        vmaCreateBuffer(VulkanContext::allocator, &stagingBufferInfo, &vmaallocInfo,
                        &stagingBuffer.buffer,
                        &stagingBuffer.allocation,
                        nullptr);

        return stagingBuffer;
    }

    AllocatedBuffer BufferHandler::createStagingBuffer(VkDeviceSize size, void *bufferData) {
        assert(size > 0);
        assert(bufferData != nullptr);

        AllocatedBuffer stagingBuffer = createStagingBuffer(size);

        void *data;
        vmaMapMemory(VulkanContext::allocator, stagingBuffer.allocation, &data);
        memcpy(data, bufferData, size);
        vmaUnmapMemory(VulkanContext::allocator, stagingBuffer.allocation);

        return stagingBuffer;
    }

    AllocatedBuffer BufferHandler::createGpuBuffer(VkDeviceSize size, VkBufferUsageFlags usage) {
        assert(size > 0);

        AllocatedBuffer buffer{};

        VkBufferCreateInfo vertexBufferInfo = {};
        vertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        vertexBufferInfo.pNext = nullptr;
        vertexBufferInfo.size = size;
        vertexBufferInfo.usage = usage | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

        VmaAllocationCreateInfo vmaallocInfo = {};
        vmaallocInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;

        vmaCreateBuffer(VulkanContext::allocator, &vertexBufferInfo, &vmaallocInfo,
                        &buffer.buffer,
                        &buffer.allocation,
                        nullptr);

        return buffer;
    }

    AllocatedBuffer BufferHandler::createGpuBuffer(VkDeviceSize size, VkBufferUsageFlags usage, void *bufferData) {
        assert(size > 0);
        assert(bufferData != nullptr);

        AllocatedBuffer gpuBuffer = createGpuBuffer(size, usage);
        AllocatedBuffer stagingBuffer = createStagingBuffer(size, bufferData);

        copyBuffer(stagingBuffer.buffer, gpuBuffer.buffer, size);

        vmaDestroyBuffer(VulkanContext::allocator, stagingBuffer.buffer, stagingBuffer.allocation);

        return gpuBuffer;
    }

    AllocatedBuffer BufferHandler::createUniformBuffer(VkDeviceSize size, void **mapped) {
        assert(size > 0);
        assert(mapped != nullptr);

        AllocatedBuffer uniformBuffer{};

        VkBufferCreateInfo uniformBufferInfo = {};
        uniformBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        uniformBufferInfo.pNext = nullptr;
        uniformBufferInfo.size = size;
        uniformBufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        VmaAllocationCreateInfo vmaallocInfo = {};
        vmaallocInfo.usage = VMA_MEMORY_USAGE_AUTO;
        vmaallocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
        vmaCreateBuffer(VulkanContext::allocator, &uniformBufferInfo, &vmaallocInfo,
                        &uniformBuffer.buffer,
                        &uniformBuffer.allocation,
                        nullptr);

        vmaMapMemory(VulkanContext::allocator, uniformBuffer.allocation, mapped);

        return uniformBuffer;
    }
}