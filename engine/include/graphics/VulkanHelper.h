#pragma once


#include <vector>
#include <memory>
#include <optional>
#include "GLFW/glfw3.h"
#include <vulkan/vulkan.h>
#include "vk_mem_alloc.h"
#include "misc/Destroyable.h"

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

    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                          const VkAllocationCallbacks *pAllocator,
                                          VkDebugUtilsMessengerEXT *pDebugMessenger);

    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                       const VkAllocationCallbacks *pAllocator);

    class VulkanHelper : public Destroyable {
    public:
        std::vector<const char *> validationLayers;

        std::vector<VkImageView> swapChainImageViews;
        std::vector<VkFramebuffer> swapChainFramebuffers;
        bool framebufferResized = false;

        std::vector<VkImage> swapChainImages;
        bool enableValidationLayers;

        bool checkValidationLayers();

        void createInstance(std::vector<const char *> extensions);

        void setupDebugMessenger();

        static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);


        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                            VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                            const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                            void *pUserData);

        void createSurface() const;

        void createSwapChain();

        static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);

        static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);

        [[nodiscard]] VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) const;

        [[nodiscard]] static SwapChainSupportDetails querySwapChainSupport();

        void createImageViews();

        void createRenderPass() const;

        void createFramebuffers();

        void createCommandPool();

        static void createCommandBuffers();

        static void createPipelineCache();

        void createSyncObjects();

        void recreateSwapChain();

        void cleanupSwapChain();

        static void createDescriptorPool();

        void createColorResource();

        [[nodiscard]] static VkSampleCountFlagBits getMaxUsableSampleCount();

        static VkImageView
        createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);

        [[nodiscard]] static AllocatedImage
        createImage(unsigned int w, unsigned int h, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usageBits,
                    VkSampleCountFlagBits sampleBits,
                    VmaMemoryUsage memoryUsage);

        VkDebugUtilsMessengerEXT debugMessenger{};
        VkSwapchainKHR swapChain{};
        VkFormat swapChainImageFormat{};
        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        SwapChainSupportDetails swapChainSupport;

        VkImageView depthImageView{};
        AllocatedImage depthImage{};

        AllocatedImage colorImage{};
        VkImageView colorImageView{};

        QueueFamilyIndices indices{};

        uint32_t imageIndex{};

        GLFWwindow *window;
    public:
        explicit VulkanHelper(GLFWwindow *wind, bool validation = true);

        [[nodiscard]] std::vector<const char *> &getValidationLayers();

        int syncNewFrame();

        void endFrame();

        static void idle();

        void destroy() override;

        void init(std::vector<const char *> &extensions);

        void createDepthResource();

        static void updateViewportScissor();
    };
}
