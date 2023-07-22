#pragma once

#include <vulkan/vulkan.hpp>
#include <vector>
#include <memory>
#include <optional>
#include <GLFW/glfw3.h>
#include "Mesh.h"

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

    struct UniformBufferObject {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };

    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                          const VkAllocationCallbacks *pAllocator,
                                          VkDebugUtilsMessengerEXT *pDebugMessenger);

    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                       const VkAllocationCallbacks *pAllocator);

    class Vulkan {
    private:
        std::vector<const char *> validationLayers;

        std::vector<VkImageView> swapChainImageViews;
        std::vector<VkFramebuffer> swapChainFramebuffers;
        int currentFrame = 0;
        bool framebufferResized = false;

        const int MAX_FRAMES_IN_FLIGHT = 2;

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

        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

        void createImageViews();

        void createGraphicsPipeline();

        VkShaderModule createShaderModule(const char *code, size_t length);

        void createRenderPass();

        void createFramebuffers();

        void createCommandPool();

        void createCommandBuffers();

        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

        void createSyncObjects();

        void recreateSwapChain();

        void cleanupSwapChain();

        void createVertexBuffer();

        void createIndexBuffer();

        void createDescriptorSetLayout();

        void createDescriptorSets();

        void createUniformBuffers();

        void updateUniformBuffer(uint32_t currentImage);

        void createDescriptorPool();

        VkInstance vkInstance{};
        VkDebugUtilsMessengerEXT debugMessenger{};
        VkSurfaceKHR surface{};
        VkSwapchainKHR swapChain{};
        VkFormat swapChainImageFormat{};
        VkExtent2D swapChainExtent{};
        VkPipelineLayout pipelineLayout{};
        VkDescriptorSetLayout descriptorSetLayout{};
        VkRenderPass renderPass{};
        VkPipeline graphicsPipeline{};
        VkCommandPool commandPool{};
        std::vector<VkCommandBuffer> commandBuffers;
        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        std::vector<VkDescriptorSet> descriptorSets;
        SwapChainSupportDetails swapChainSupport;

        VkBuffer vertexBuffer{};
        VkDeviceMemory vertexBufferMemory{};
        VkBuffer indexBuffer{};
        VkDeviceMemory indexBufferMemory{};
        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDeviceMemory> uniformBuffersMemory;
        std::vector<void *> uniformBuffersMapped;
        VkDescriptorPool descriptorPool{};

        VkPhysicalDevice vkPhysicalDevice = VK_NULL_HANDLE;
        VkDevice vkLogicalDevice = VK_NULL_HANDLE;
        QueueFamilyIndices indices{};
        VkQueue graphicsQueue{};
        VkQueue presentQueue{};

        GLFWwindow *window;
        Mesh *rect;
    public:
        explicit Vulkan(GLFWwindow *wind, bool validation = true);

        [[nodiscard]] std::vector<const char *> &getValidationLayers();

        VkInstance &getInstance();

        void drawFrame();

        void idle();

        void cleanup();

        void init(std::vector<const char *> &extensions);
    };
}
