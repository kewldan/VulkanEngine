#ifndef VULKANENGINE_VULKANTYPES_H
#define VULKANENGINE_VULKANTYPES_H

#include <optional>
#include <vulkan/vulkan.h>
#include <vector>
#include <vk_mem_alloc.h>

namespace Engine {
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        [[nodiscard]] bool isComplete() const;
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    struct AllocatedBuffer {
        VkBuffer buffer;
        VmaAllocation allocation;
    };

    struct AllocatedImage {
        VkImage image;
        VmaAllocation allocation;
    };
}

#endif //VULKANENGINE_VULKANTYPES_H
