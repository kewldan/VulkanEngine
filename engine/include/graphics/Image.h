#ifndef VULKANENGINE_IMAGE_H
#define VULKANENGINE_IMAGE_H

#include "VulkanTypes.h"
#include "misc/Destroyable.h"

namespace Engine {
    class Image : public Destroyable {
    private:
        AllocatedImage image{};
        VkImageView view = VK_NULL_HANDLE;
    public:
        void init(unsigned int w, unsigned int h, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usageBits,
                  VkSampleCountFlagBits sampleBits,
                  VmaMemoryUsage memoryUsage);

        void createImageView(VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);

        [[nodiscard]] AllocatedImage &getImage();

        [[nodiscard]] VkImageView getView() const;

        void destroy() override;
    };
} // Engine

#endif //VULKANENGINE_IMAGE_H
