#pragma once

#include <vector>
#include "graphics/VulkanHelper.h"
#include "graphics/BufferHandler.h"

namespace Engine {
    template<class T>
    class UniformBuffer : public T {
    private:
        std::vector<VkDeviceMemory> uniformBuffersMemory;
        std::vector<void *> uniformBuffersMapped;

        VkDescriptorSetLayout getLayout(VkDevice device, VkShaderStageFlagBits shaderStage, int binding);

        std::vector<VkDescriptorSet>
        getDescriptorSet(VkDevice device, VkDescriptorPool descriptorPool, VkShaderStageFlagBits shaderStage,
                         int binding);

    public:
        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDescriptorSet> descriptorSets;
        VkDescriptorSetLayout layout{};

        UniformBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkDescriptorPool descriptorPool,
                      VkShaderStageFlagBits shaderStage, int binding);

        void upload(int currentFrame);

        void cleanup(VkDevice device);
    };

    template<class T>
    std::vector<VkDescriptorSet> UniformBuffer<T>::getDescriptorSet(VkDevice device, VkDescriptorPool descriptorPool,
                                                                    VkShaderStageFlagBits shaderStage, int binding) {
        layout = getLayout(device, shaderStage, binding);
        std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, layout);

        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = MAX_FRAMES_IN_FLIGHT;
        allocInfo.pSetLayouts = layouts.data();

        std::vector<VkDescriptorSet> sets;

        sets.resize(MAX_FRAMES_IN_FLIGHT);
        if (vkAllocateDescriptorSets(device, &allocInfo, sets.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

        for (size_t j = 0; j < MAX_FRAMES_IN_FLIGHT; j++) {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = uniformBuffers[j];
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(T);

            VkWriteDescriptorSet descriptorWrite{};
            descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet = sets[j];
            descriptorWrite.dstBinding = binding;
            descriptorWrite.dstArrayElement = 0;
            descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrite.descriptorCount = 1;
            descriptorWrite.pBufferInfo = &bufferInfo;
            vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
        }

        return sets;
    }

    template<class T>
    VkDescriptorSetLayout UniformBuffer<T>::getLayout(VkDevice device, VkShaderStageFlagBits shaderStage, int binding) {
        VkDescriptorSetLayoutBinding layoutBinding{};
        layoutBinding.binding = binding;
        layoutBinding.descriptorCount = 1;
        layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        layoutBinding.pImmutableSamplers = nullptr;
        layoutBinding.stageFlags = shaderStage;

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings = &layoutBinding;
        layoutInfo.pNext = nullptr; // Make sure to initialize the pNext field

        VkDescriptorSetLayout descriptorSetLayout{};
        if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor set layout!");
        }

        return descriptorSetLayout;
    }

    template<class T>
    UniformBuffer<T>::UniformBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkDescriptorPool descriptorPool,
                                    VkShaderStageFlagBits shaderStage, int binding) {
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

        descriptorSets = getDescriptorSet(device, descriptorPool, shaderStage, binding);
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