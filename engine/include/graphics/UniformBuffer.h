#pragma once

#include <vector>
#include "graphics/VulkanHelper.h"
#include "graphics/BufferHandler.h"
#include <stdexcept>
#include "misc/Destroyable.h"

namespace Engine {
    template<typename T>
    class UniformBuffer : public T {
    private:
        std::vector<AllocatedBuffer> uniformAllocatedBuffers;
        std::vector<void *> uniformBuffersMapped;

        VkDescriptorSetLayout getLayout(VkShaderStageFlagBits shaderStage, int binding) {
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
            if (vkCreateDescriptorSetLayout(VulkanContext::device, &layoutInfo, nullptr, &descriptorSetLayout) !=
                VK_SUCCESS) {
                throw std::runtime_error("failed to create descriptor set layout!");
            }

            return descriptorSetLayout;
        };

        std::vector<VkDescriptorSet>
        getDescriptorSet(VkShaderStageFlagBits shaderStage,
                         int binding);

    public:
        std::vector<VkDescriptorSet> descriptorSets;
        VkDescriptorSetLayout layout{};

        UniformBuffer() = default;

        void init(VkShaderStageFlagBits shaderStage, int binding);

        void upload();

        void destroy() {
            for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
                vmaUnmapMemory(VulkanContext::allocator, uniformAllocatedBuffers[i].allocation);
                vmaDestroyBuffer(VulkanContext::allocator, uniformAllocatedBuffers[i].buffer,
                                 uniformAllocatedBuffers[i].allocation);
            }
        }
    };

    template<class T>
    void UniformBuffer<T>::init(VkShaderStageFlagBits shaderStage, int binding) {
        VkDeviceSize bufferSize = sizeof(T);

        uniformAllocatedBuffers.resize(MAX_FRAMES_IN_FLIGHT);
        uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            uniformAllocatedBuffers[i] = BufferHandler::createUniformBuffer(bufferSize, &uniformBuffersMapped[i]);
        }

        descriptorSets = getDescriptorSet(shaderStage, binding);
    }

    template<class T>
    std::vector<VkDescriptorSet> UniformBuffer<T>::getDescriptorSet(VkShaderStageFlagBits shaderStage, int binding) {
        layout = getLayout(shaderStage, binding);
        std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, layout);

        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = VulkanContext::descriptorPool;
        allocInfo.descriptorSetCount = MAX_FRAMES_IN_FLIGHT;
        allocInfo.pSetLayouts = layouts.data();

        std::vector<VkDescriptorSet> sets;

        sets.resize(MAX_FRAMES_IN_FLIGHT);
        if (vkAllocateDescriptorSets(VulkanContext::device, &allocInfo, sets.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

        for (size_t j = 0; j < MAX_FRAMES_IN_FLIGHT; j++) {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = uniformAllocatedBuffers[j].buffer;
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
            vkUpdateDescriptorSets(VulkanContext::device, 1, &descriptorWrite, 0, nullptr);
        }

        return sets;
    }

    template<class T>
    void UniformBuffer<T>::upload() {
        memcpy(uniformBuffersMapped[VulkanContext::currentFrame], this, sizeof(T));
    }
}