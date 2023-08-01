#include <stdexcept>
#include "graphics/BufferHandler.h"
#include "graphics/VulkanContext.h"

namespace Engine {
    uint32_t BufferHandler::findMemoryType(uint32_t typeFilter,
                                           VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(VulkanContext::physicalDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        throw std::runtime_error("failed to find suitable memory type!");
    }

    void
    BufferHandler::copyBuffer(VkBuffer srcBuffer,
                              VkBuffer dstBuffer, VkDeviceSize size) {
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

    void BufferHandler::createBuffer(VkDeviceSize size,
                                     VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                                     VkBuffer &buffer, VkDeviceMemory &bufferMemory) {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(VulkanContext::device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(VulkanContext::device, buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits,
                                                   properties);

        if (vkAllocateMemory(VulkanContext::device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate buffer memory!");
        }

        vkBindBufferMemory(VulkanContext::device, buffer, bufferMemory, 0);
    }

    AllocatedBuffer BufferHandler::createStagingBuffer(VkDeviceSize size) {
        AllocatedBuffer stagingBuffer{};

        VkBufferCreateInfo stagingBufferInfo = {};
        stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        stagingBufferInfo.pNext = nullptr;
        stagingBufferInfo.size = size;
        stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        VmaAllocationCreateInfo vmaallocInfo = {};
        vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;
        vmaCreateBuffer(VulkanContext::allocator, &stagingBufferInfo, &vmaallocInfo,
                        &stagingBuffer.buffer,
                        &stagingBuffer.allocation,
                        nullptr);

        return stagingBuffer;
    }

    AllocatedBuffer BufferHandler::createStagingBuffer(VkDeviceSize size, void *bufferData) {
        AllocatedBuffer stagingBuffer = createStagingBuffer(size);

        void *data;
        vmaMapMemory(VulkanContext::allocator, stagingBuffer.allocation, &data);
        memcpy(data, bufferData, size);
        vmaUnmapMemory(VulkanContext::allocator, stagingBuffer.allocation);

        return stagingBuffer;
    }

    AllocatedBuffer BufferHandler::createGpuBuffer(VkDeviceSize size, VkBufferUsageFlags usage) {
        AllocatedBuffer buffer{};

        VkBufferCreateInfo vertexBufferInfo = {};
        vertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        vertexBufferInfo.pNext = nullptr;
        vertexBufferInfo.size = size;
        vertexBufferInfo.usage = usage | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

        VmaAllocationCreateInfo vmaallocInfo = {};
        vmaallocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

        vmaCreateBuffer(VulkanContext::allocator, &vertexBufferInfo, &vmaallocInfo,
                        &buffer.buffer,
                        &buffer.allocation,
                        nullptr);

        return buffer;
    }

    AllocatedBuffer BufferHandler::createGpuBuffer(VkDeviceSize size, VkBufferUsageFlags usage, void *bufferData) {
        AllocatedBuffer gpuBuffer = createGpuBuffer(size, usage);
        AllocatedBuffer stagingBuffer = createStagingBuffer(size, bufferData);

        copyBuffer(stagingBuffer.buffer, gpuBuffer.buffer, size);

        vmaDestroyBuffer(VulkanContext::allocator, stagingBuffer.buffer, stagingBuffer.allocation);

        return gpuBuffer;
    }
}