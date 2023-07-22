#pragma once

#include <vector>
#include "vulkan/vulkan.h"
#include "BufferHandler.h"

namespace Engine {
    template<class T>
    class UniformBuffer : public T {
    private:
        std::vector<VkDeviceMemory> uniformBuffersMemory;
        std::vector<void *> uniformBuffersMapped;
    public:
        std::vector<VkBuffer> uniformBuffers;

        UniformBuffer(VkPhysicalDevice physicalDevice, VkDevice device);

        void upload(int currentFrame);

        void cleanup(VkDevice device);
    };

    template<class T>
    UniformBuffer<T>::UniformBuffer(VkPhysicalDevice physicalDevice, VkDevice device) {
        VkDeviceSize bufferSize = sizeof(T);

        uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
        uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
        uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

        for (size_t i = 0; i < 2; i++) {
            BufferHandler::createBuffer(physicalDevice, device, bufferSize,
                                        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                        uniformBuffers[i],
                                        uniformBuffersMemory[i]);

            vkMapMemory(device, uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
        }
    }

    template<class T>
    void UniformBuffer<T>::cleanup(VkDevice device) {
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroyBuffer(device, uniformBuffers[i], nullptr);
            vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
        }
    }

    template<class T>
    void UniformBuffer<T>::upload(int currentFrame) {
        memcpy(uniformBuffersMapped[currentFrame], this, sizeof(T));
    }
}