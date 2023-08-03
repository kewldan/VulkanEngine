#ifndef VULKANENGINE_SWAPCHAIN_H
#define VULKANENGINE_SWAPCHAIN_H

#include <vulkan/vulkan.h>
#include <vector>
#include "vk_mem_alloc.h"
#include "VulkanTypes.h"
#include "misc/Destroyable.h"
#include "Image.h"
#include "GLFW/glfw3.h"

namespace Engine {
    class SwapChain : public Destroyable {
    private:
        std::vector<VkImageView> swapChainImageViews;
        std::vector<VkFramebuffer> swapChainFramebuffers;
        std::vector<VkImage> swapChainImages;
        VkSwapchainKHR swapChain{};
        VkFormat swapChainImageFormat{};
        SwapChainSupportDetails swapChainSupport;

        Image colorImage, depthImage;

        void createImageViews();

        void createFramebuffers();

        void createColorResource();

        void createDepthResource();

        static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);

        static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);

        [[nodiscard]] static VkExtent2D
        chooseSwapExtent(GLFWwindow *window, const VkSurfaceCapabilitiesKHR &capabilities);

        [[nodiscard]] static SwapChainSupportDetails getSupportDetails();

        void create(GLFWwindow *window);

    public:
        void createImages(GLFWwindow *window);

        void createResources();

        void recreateSwapChain(GLFWwindow *window);

        [[nodiscard]] VkSwapchainKHR getSwapChain() const;

        [[nodiscard]] VkFramebuffer getFramebuffer(uint32_t imageIndex) const;

        [[nodiscard]] VkFormat getFormat() const;

        void destroy() override;
    };

} // Engine

#endif //VULKANENGINE_SWAPCHAIN_H
