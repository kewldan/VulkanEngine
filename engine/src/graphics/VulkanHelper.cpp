#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

#include <stdexcept>
#include <cassert>
#include <algorithm>
#include <plog/Log.h>

#include "graphics/VulkanHelper.h"
#include "graphics/DeviceHandler.h"
#include "graphics/DebugUtils.h"

#define VMA_IMPLEMENTATION

#include <vk_mem_alloc.h>

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

namespace Engine {
    bool VulkanHelper::checkValidationLayers() {
        uint32_t layerCount = 0;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char *layerName: validationLayers) {
            bool layerFound = false;

            for (const auto &layerProperties: availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }

    VulkanHelper::VulkanHelper(GLFWwindow *window, bool enableValidationLayers) : window(window),
                                                                                  enableValidationLayers(
                                                                                          enableValidationLayers) {
    }

    VkBool32 VulkanHelper::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                         VkDebugUtilsMessageTypeFlagsEXT messageType,
                                         const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *) {
        const char *prefix = "";
        if (messageType & VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) {
            prefix = "[GEN]";
        } else if (messageType &
                   VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT) {
            prefix = "[DEV]";
        } else if (messageType & VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) {
            prefix = "[PER]";
        } else if (messageType & VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) {
            prefix = "[VAL]";
        }
        if (messageSeverity & VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
            PLOGE << prefix << " " << pCallbackData->pMessage;
        } else if (messageSeverity &
                   VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
            PLOGW << prefix << " " << pCallbackData->pMessage;
        } else if (messageSeverity &
                   VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
            PLOGI << prefix << " " << pCallbackData->pMessage;
        } else if (messageSeverity &
                   VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
            PLOGV << prefix << " " << pCallbackData->pMessage;
        }
        return VK_FALSE;
    }

    void VulkanHelper::createInstance(std::vector<const char *> extensions) {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Vulkan game";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        for (int i = 0; i < glfwExtensionCount; i++) {
            extensions.push_back(glfwExtensions[i]);
        }

        createInfo.enabledExtensionCount = extensions.size();
        createInfo.ppEnabledExtensionNames = extensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();

            populateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *) &debugCreateInfo;
        } else {
            createInfo.enabledLayerCount = 0;

            createInfo.pNext = nullptr;
        }
        if (vkCreateInstance(&createInfo, nullptr, &VulkanContext::instance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create graphics instance");
        }
    }

    void VulkanHelper::destroy() {
        cleanupSwapChain();

        vkDestroyDescriptorPool(VulkanContext::device, VulkanContext::descriptorPool, nullptr);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(VulkanContext::device, renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(VulkanContext::device, imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(VulkanContext::device, inFlightFences[i], nullptr);
        }

        vkDestroyCommandPool(VulkanContext::device, VulkanContext::commandPool, nullptr);

        VulkanContext::renderPass.destroy();
        vkDestroyPipelineCache(VulkanContext::device, VulkanContext::pipelineCache, nullptr);

        vmaDestroyAllocator(VulkanContext::allocator);

        vkDestroyDevice(VulkanContext::device, nullptr);

        if (enableValidationLayers) {
            DestroyDebugUtilsMessengerEXT(VulkanContext::instance, debugMessenger, nullptr);
        }

        vkDestroySurfaceKHR(VulkanContext::instance, VulkanContext::surface, nullptr);
        vkDestroyInstance(VulkanContext::instance, nullptr);
    }

    void VulkanHelper::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity =
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType =
                VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
    }

    void VulkanHelper::setupDebugMessenger() {
        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        populateDebugMessengerCreateInfo(createInfo);

        if (CreateDebugUtilsMessengerEXT(VulkanContext::instance, &createInfo, nullptr, &debugMessenger) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to set up debug messenger!");
        }
    }

    void VulkanHelper::init(std::vector<const char *> &extensions) {
        assert(window != nullptr);

        if (enableValidationLayers && !checkValidationLayers()) {
            throw std::runtime_error("validation layers is not available");
        }

        createInstance(extensions);

        if (enableValidationLayers) {
            setupDebugMessenger();
        }

        createSurface();

        DeviceHandler::getDevices(&indices,
                                  std::vector<const char *>() = {VK_KHR_SWAPCHAIN_EXTENSION_NAME},
                                  enableValidationLayers, validationLayers);

        VmaAllocatorCreateInfo allocatorInfo = {};
        allocatorInfo.physicalDevice = VulkanContext::physicalDevice;
        allocatorInfo.device = VulkanContext::device;
        allocatorInfo.instance = VulkanContext::instance;
        vmaCreateAllocator(&allocatorInfo, &VulkanContext::allocator);

        VulkanContext::msaaSamples = getMaxUsableSampleCount();
        PLOGD << "Using MSAAx" << VulkanContext::msaaSamples;

        createSwapChain();
        createImageViews();
        createPipelineCache();
        VulkanContext::renderPass.init(swapChainImageFormat);
        createColorResource();
        createDepthResource();
        createFramebuffers();
        createCommandPool();
        createDescriptorPool();
        VulkanContext::commandBuffers.init();
        createSyncObjects();
    }

    void VulkanHelper::createSurface() const {
        assert(window != nullptr);
        if (glfwCreateWindowSurface(VulkanContext::instance, window, nullptr, &VulkanContext::surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface!");
        }
    }

    void VulkanHelper::createSwapChain() {
        swapChainSupport = querySwapChainSupport();
        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

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

        uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        if (indices.graphicsFamily != indices.presentFamily) {
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

    VkSurfaceFormatKHR VulkanHelper::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) {
        for (const auto &availableFormat: availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
                availableFormat.colorSpace == VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT) {
                return availableFormat;
            }
        }

        PLOGW << "Swap surface format \"VK_FORMAT_B8G8R8A8_SRGB & VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT\" is not supported, using 1st available";

        return availableFormats[0];
    }

    VkPresentModeKHR VulkanHelper::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes) {
        for (const auto &availablePresentMode: availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }

        PLOGW << "Swap present mode \"VK_PRESENT_MODE_MAILBOX_KHR\" is not supported, using \"VK_PRESENT_MODE_FIFO_KHR\"";
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D VulkanHelper::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) const {
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

    std::vector<const char *> &VulkanHelper::getValidationLayers() {
        return validationLayers;
    }

    void VulkanHelper::createImageViews() {
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

    void VulkanHelper::createFramebuffers() {
        swapChainFramebuffers.resize(swapChainImageViews.size());

        for (size_t i = 0; i < swapChainImageViews.size(); i++) {
            VkImageView attachments[] = {
                    colorImageView,
                    depthImageView,
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

    void VulkanHelper::createCommandPool() {
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = indices.graphicsFamily.value();

        if (vkCreateCommandPool(VulkanContext::device, &poolInfo, nullptr, &VulkanContext::commandPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create command pool!");
        }
    }

    void VulkanHelper::createSyncObjects() {
        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(VulkanContext::device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) !=
                VK_SUCCESS ||
                vkCreateSemaphore(VulkanContext::device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) !=
                VK_SUCCESS ||
                vkCreateFence(VulkanContext::device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create synchronization objects for a frame!");
            }
        }
    }

    void VulkanHelper::idle() {
        vkDeviceWaitIdle(VulkanContext::device);
    }

    void VulkanHelper::recreateSwapChain() {
        int width = 0, height = 0;
        glfwGetFramebufferSize(window, &width, &height);
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(window, &width, &height);
            glfwWaitEvents();
        }

        idle();

        VulkanContext::swapChainExtent.width = width;
        VulkanContext::swapChainExtent.height = height;

        cleanupSwapChain();

        createSwapChain();
        createImageViews();
        createColorResource();
        createDepthResource();
        createFramebuffers();
    }

    void VulkanHelper::cleanupSwapChain() {
        for (auto &swapChainFramebuffer: swapChainFramebuffers) {
            vkDestroyFramebuffer(VulkanContext::device, swapChainFramebuffer, nullptr);
        }

        for (auto &swapChainImageView: swapChainImageViews) {
            vkDestroyImageView(VulkanContext::device, swapChainImageView, nullptr);
        }

        vkDestroyImageView(VulkanContext::device, depthImageView, nullptr);
        vmaDestroyImage(VulkanContext::allocator, depthImage.image, depthImage.allocation);

        vkDestroyImageView(VulkanContext::device, colorImageView, nullptr);
        vmaDestroyImage(VulkanContext::allocator, colorImage.image, colorImage.allocation);

        vkDestroySwapchainKHR(VulkanContext::device, swapChain, nullptr);
    }

    void VulkanHelper::createDescriptorPool() {
        VkDescriptorPoolSize poolSize[] = {
                {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 40}
        };

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = sizeof(poolSize) / sizeof(*poolSize);
        poolInfo.pPoolSizes = poolSize;
        poolInfo.maxSets = 40;

        if (vkCreateDescriptorPool(VulkanContext::device, &poolInfo, nullptr, &VulkanContext::descriptorPool) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }
    }

    void VulkanHelper::syncNewFrame() {
        vkWaitForFences(VulkanContext::device, 1, &inFlightFences[VulkanContext::currentFrame], VK_TRUE, UINT64_MAX);

        VkResult result = vkAcquireNextImageKHR(VulkanContext::device, swapChain, UINT64_MAX,
                                                imageAvailableSemaphores[VulkanContext::currentFrame], VK_NULL_HANDLE,
                                                &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
        } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        vkResetFences(VulkanContext::device, 1, &inFlightFences[VulkanContext::currentFrame]);

        vkResetCommandBuffer(
                VulkanContext::commandBuffers.getCommandBuffer(), /*VkCommandBufferResetFlagBits*/ 0);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(VulkanContext::commandBuffers.getCommandBuffer(), &beginInfo) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = VulkanContext::renderPass.getRenderPass();
        renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = VulkanContext::swapChainExtent;

        const static VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
        VkClearValue depthClear;
        depthClear.depthStencil.depth = 1.f;

        VkClearValue clearValues[] = {clearColor, depthClear};

        renderPassInfo.clearValueCount = 2;
        renderPassInfo.pClearValues = clearValues;

        vkCmdBeginRenderPass(VulkanContext::commandBuffers.getCommandBuffer(), &renderPassInfo,
                             VK_SUBPASS_CONTENTS_INLINE);
    }

    void VulkanHelper::endFrame() {
        vkCmdEndRenderPass(VulkanContext::commandBuffers.getCommandBuffer());

        if (vkEndCommandBuffer(VulkanContext::commandBuffers.getCommandBuffer()) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[VulkanContext::currentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        VkCommandBuffer commandBuffers[] = {VulkanContext::commandBuffers.getCommandBuffer()};

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = commandBuffers;

        VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[VulkanContext::currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(VulkanContext::graphicsQueue, 1, &submitInfo, inFlightFences[VulkanContext::currentFrame]) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {swapChain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &imageIndex;

        VkResult result = vkQueuePresentKHR(VulkanContext::presentQueue, &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
            framebufferResized = false;
            recreateSwapChain();
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }

        VulkanContext::currentFrame = (VulkanContext::currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    SwapChainSupportDetails VulkanHelper::querySwapChainSupport() {
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

    AllocatedImage
    VulkanHelper::createImage(unsigned int w, unsigned int h, VkFormat format, VkImageTiling tiling,
                              VkImageUsageFlags usageBits,
                              VkSampleCountFlagBits sampleBits,
                              VmaMemoryUsage memoryUsage) {

        AllocatedImage img{};

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

        if (vmaCreateImage(VulkanContext::allocator, &imageInfo, &allocationInfo, &img.image, &img.allocation,
                           nullptr) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to create image!");
        }

        return img;
    }

    void VulkanHelper::createColorResource() {
        VkFormat colorFormat = swapChainImageFormat;

        colorImage = createImage(VulkanContext::swapChainExtent.width, VulkanContext::swapChainExtent.height,
                                 colorFormat, VK_IMAGE_TILING_OPTIMAL,
                                 VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                                 VulkanContext::msaaSamples, VMA_MEMORY_USAGE_AUTO);
        colorImageView = createImageView(colorImage.image, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
    }

    VkImageView VulkanHelper::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags,
                                              uint32_t mipLevels) {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = mipLevels;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        VkImageView imageView;
        if (vkCreateImageView(VulkanContext::device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture image view!");
        }

        return imageView;
    }

    VkSampleCountFlagBits VulkanHelper::getMaxUsableSampleCount() {
        VkPhysicalDeviceProperties physicalDeviceProperties;
        vkGetPhysicalDeviceProperties(VulkanContext::physicalDevice, &physicalDeviceProperties);

        VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts &
                                    physicalDeviceProperties.limits.framebufferDepthSampleCounts;
        if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
        if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
        if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
        if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
        if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
        if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

        return VK_SAMPLE_COUNT_1_BIT;
    }

    void VulkanHelper::createDepthResource() {
        depthImage = createImage(VulkanContext::swapChainExtent.width, VulkanContext::swapChainExtent.height,
                                 VK_FORMAT_D32_SFLOAT,
                                 VK_IMAGE_TILING_OPTIMAL,
                                 VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VulkanContext::msaaSamples,
                                 VMA_MEMORY_USAGE_GPU_ONLY);
        depthImageView = createImageView(depthImage.image, VK_FORMAT_D32_SFLOAT, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
    }

    void VulkanHelper::createPipelineCache() {
        VkPipelineCacheCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
        vkCreatePipelineCache(VulkanContext::device, &createInfo, nullptr, &VulkanContext::pipelineCache);
    }

    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                          const VkAllocationCallbacks *pAllocator,
                                          VkDebugUtilsMessengerEXT *pDebugMessenger) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance,
                                                                               "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                       const VkAllocationCallbacks *pAllocator) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance,
                                                                                "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(instance, debugMessenger, pAllocator);
        }
    }

    bool QueueFamilyIndices::isComplete() const {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }

}