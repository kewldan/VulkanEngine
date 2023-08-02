#include <stdexcept>
#include "graphics/CommandBuffers.h"
#include "graphics/VulkanContext.h"

namespace Engine {
    void CommandBuffers::destroy() {
        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {

        }
    }

    VkCommandBuffer CommandBuffers::getCommandBuffer() {
        return commandBuffers[VulkanContext::currentFrame];
    }

    void CommandBuffers::init() {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = VulkanContext::commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = MAX_FRAMES_IN_FLIGHT;

        if (vkAllocateCommandBuffers(VulkanContext::device, &allocInfo, commandBuffers) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }
} // Engine