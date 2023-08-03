#include <stdexcept>
#include "graphics/Image.h"
#include "graphics/VulkanContext.h"

namespace Engine {
    void Image::init(unsigned int w, unsigned int h, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usageBits,
                     VkSampleCountFlagBits sampleBits, VmaMemoryUsage memoryUsage) {
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = w;
        imageInfo.extent.height = h;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usageBits;
        imageInfo.samples = sampleBits;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VmaAllocationCreateInfo allocationInfo = {};
        allocationInfo.usage = memoryUsage;

        if (vmaCreateImage(VulkanContext::allocator, &imageInfo, &allocationInfo, &image.image, &image.allocation,
                           nullptr) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to create image!");
        }
    }

    void Image::createImageView(VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels) {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image.image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = mipLevels;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(VulkanContext::device, &viewInfo, nullptr, &view) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture image view!");
        }
    }

    AllocatedImage &Image::getImage() {
        return image;
    }

    VkImageView Image::getView() const {
        return view;
    }

    void Image::destroy() {
        if (view != VK_NULL_HANDLE) {
            vkDestroyImageView(VulkanContext::device, view, nullptr);
        }
        vmaDestroyImage(VulkanContext::allocator, image.image, image.allocation);
    }
} // Engine