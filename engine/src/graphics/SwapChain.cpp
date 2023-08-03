#include <stdexcept>
#include <algorithm>
#include "graphics/SwapChain.h"
#include "graphics/VulkanContext.h"
#include "plog/Log.h"

namespace Engine {
    void SwapChain::destroy() {
        for (auto &swapChainFramebuffer: swapChainFramebuffers) {
            vkDestroyFramebuffer(VulkanContext::device, swapChainFramebuffer, nullptr);
        }

        for (auto &swapChainImageView: swapChainImageViews) {
            vkDestroyImageView(VulkanContext::device, swapChainImageView, nullptr);
        }

        colorImage.destroy();
        depthImage.destroy();

        vkDestroySwapchainKHR(VulkanContext::device, swapChain, nullptr);
    }

    void SwapChain::createImages(GLFWwindow *window) {
        create(window);
        createImageViews();
    }

    void SwapChain::create(GLFWwindow *window) {
        swapChainSupport = getSupportDetails();
        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = chooseSwapExtent(window, swapChainSupport.capabilities);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 &&
            imageCount > swapChainSupport.capabilities.maxImageCount) {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = VulkanContext::surface;

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        uint32_t queueFamilyIndices[] = {VulkanContext::indices.graphicsFamily.value(),
                                         VulkanContext::indices.presentFamily.value()};

        if (VulkanContext::indices.graphicsFamily != VulkanContext::indices.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        } else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(VulkanContext::device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
            throw std::runtime_error("failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(VulkanContext::device, swapChain, &imageCount, nullptr);
        swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(VulkanContext::device, swapChain, &imageCount, swapChainImages.data());

        swapChainImageFormat = surfaceFormat.format;
        VulkanContext::swapChainExtent = extent;
    }

    void SwapChain::createResources() {
        createColorResource();
        createDepthResource();
        createFramebuffers();
    }

    void SwapChain::createImageViews() {
        swapChainImageViews.resize(swapChainImages.size());

        for (size_t i = 0; i < swapChainImages.size(); i++) {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = swapChainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = swapChainImageFormat;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(VulkanContext::device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create image views!");
            }
        }
    }

    void SwapChain::createFramebuffers() {
        swapChainFramebuffers.resize(swapChainImageViews.size());

        for (size_t i = 0; i < swapChainImageViews.size(); i++) {
            VkImageView attachments[] = {
                    colorImage.getView(),
                    depthImage.getView(),
                    swapChainImageViews[i]
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = VulkanContext::renderPass.getRenderPass();
            framebufferInfo.attachmentCount = 3;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = VulkanContext::swapChainExtent.width;
            framebufferInfo.height = VulkanContext::swapChainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(VulkanContext::device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) !=
                VK_SUCCESS) {
                throw std::runtime_error("failed to create framebuffer!");
            }
        }
    }

    VkExtent2D SwapChain::chooseSwapExtent(GLFWwindow *window, const VkSurfaceCapabilitiesKHR &capabilities) {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return capabilities.currentExtent;
        } else {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);

            VkExtent2D actualExtent = {
                    static_cast<uint32_t>(width),
                    static_cast<uint32_t>(height)
            };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width,
                                            capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                                             capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes) {
        for (const auto &availablePresentMode: availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }

        PLOGW << R"(Swap present mode "VK_PRESENT_MODE_MAILBOX_KHR" is not supported, using "VK_PRESENT_MODE_FIFO_KHR")";
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) {
        for (const auto &availableFormat: availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
                availableFormat.colorSpace == VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT) {
                return availableFormat;
            }
        }

        PLOGW << "Swap surface format \"VK_FORMAT_B8G8R8A8_SRGB & VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT\" is not supported, using 1st available";

        return availableFormats[0];
    }

    void SwapChain::recreateSwapChain(GLFWwindow *window) {
        int width = 0, height = 0;
        glfwGetFramebufferSize(window, &width, &height);
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(window, &width, &height);
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(VulkanContext::device);

        VulkanContext::swapChainExtent.width = width;
        VulkanContext::swapChainExtent.height = height;

        destroy();

        create(window);
        createImageViews();
        createColorResource();
        createDepthResource();
        createFramebuffers();
    }

    VkSwapchainKHR SwapChain::getSwapChain() const {
        return swapChain;
    }

    VkFramebuffer SwapChain::getFramebuffer(uint32_t imageIndex) const {
        return swapChainFramebuffers[imageIndex];
    }

    void SwapChain::createColorResource() {
        colorImage.init(VulkanContext::swapChainExtent.width, VulkanContext::swapChainExtent.height,
                        swapChainImageFormat, VK_IMAGE_TILING_OPTIMAL,
                        VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                        VulkanContext::msaaSamples, VMA_MEMORY_USAGE_AUTO);
        colorImage.createImageView(swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
    }

    void SwapChain::createDepthResource() {
        depthImage.init(VulkanContext::swapChainExtent.width, VulkanContext::swapChainExtent.height,
                        VK_FORMAT_D32_SFLOAT,
                        VK_IMAGE_TILING_OPTIMAL,
                        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VulkanContext::msaaSamples,
                        VMA_MEMORY_USAGE_GPU_ONLY);
        depthImage.createImageView(VK_FORMAT_D32_SFLOAT, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
    }

    VkFormat SwapChain::getFormat() const {
        return swapChainImageFormat;
    }

    SwapChainSupportDetails SwapChain::getSupportDetails() {
        SwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VulkanContext::physicalDevice, VulkanContext::surface,
                                                  &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(VulkanContext::physicalDevice, VulkanContext::surface, &formatCount,
                                             nullptr);

        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(VulkanContext::physicalDevice, VulkanContext::surface, &formatCount,
                                                 details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(VulkanContext::physicalDevice, VulkanContext::surface,
                                                  &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(VulkanContext::physicalDevice, VulkanContext::surface,
                                                      &presentModeCount,
                                                      details.presentModes.data());
        }

        return details;
    }
} // Engine