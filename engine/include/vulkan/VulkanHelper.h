#pragma once

#include "vulkan/vulkan.hpp"
#include <vector>
#include <memory>
#include <optional>
#include "GLFW/glfw3.h"
#include "graphics/Mesh.h"

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

    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                          const VkAllocationCallbacks *pAllocator,
                                          VkDebugUtilsMessengerEXT *pDebugMessenger);

    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                       const VkAllocationCallbacks *pAllocator);

    class VulkanHelper {
    public:
        std::vector<const char *> validationLayers;

        std::vector<VkImageView> swapChainImageViews;
        std::vector<VkFramebuffer> swapChainFramebuffers;
        int currentFrame = 0;
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

        void createSurface();

        void createSwapChain();

        static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);

        static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);

        [[nodiscard]] VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) const;

        void createImageViews();

        void createRenderPass();

        void createFramebuffers();

        void createCommandPool();

        void createCommandBuffers();

        void createSyncObjects();

        void recreateSwapChain();

        void cleanupSwapChain();

        void createDescriptorPool();

        VkInstance vkInstance{};
        VkDebugUtilsMessengerEXT debugMessenger{};
        VkSurfaceKHR surface{};
        VkSwapchainKHR swapChain{};
        VkFormat swapChainImageFormat{};
        VkExtent2D swapChainExtent{};
        VkRenderPass renderPass{};
        VkCommandPool commandPool{};
        std::vector<VkCommandBuffer> commandBuffers;
        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        SwapChainSupportDetails swapChainSupport;

        VkDescriptorPool descriptorPool{};

        VkPhysicalDevice vkPhysicalDevice = VK_NULL_HANDLE;
        VkDevice vkLogicalDevice = VK_NULL_HANDLE;
        QueueFamilyIndices indices{};
        VkQueue graphicsQueue{};
        VkQueue presentQueue{};

        uint32_t imageIndex;

        GLFWwindow *window;
    public:
        explicit VulkanHelper(GLFWwindow *wind, bool validation = true);

        [[nodiscard]] std::vector<const char *> &getValidationLayers();

        VkInstance &getInstance();

        int syncNewFrame();

        void endFrame();

        void idle() const;

        void cleanup();

        void init(std::vector<const char *> &extensions);
    };
}
