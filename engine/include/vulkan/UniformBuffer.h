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

        VkDescriptorSetLayout getLayout(VkDevice device, VkShaderStageFlagBits shaderStage, int binding);
    };

    template<class T>
    VkDescriptorSetLayout UniformBuffer<T>::getLayout(VkDevice device, VkShaderStageFlagBits shaderStage, int binding) {
        VkDescriptorSetLayoutBinding layoutBinding{};
        layoutBinding.binding = binding;
        layoutBinding.descriptorCount = 1;
        layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        layoutBinding.pImmutableSamplers = nullptr;
        layoutBinding.stageFlags = shaderStage; //TODO: Other shaders

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings = &layoutBinding;

        VkDescriptorSetLayout descriptorSetLayout{};
        if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor set layout!");
        }

        return descriptorSetLayout;
    }

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