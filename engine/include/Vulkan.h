#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>
#include <optional>
#include "Vertex.h"

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
        Vertex *vertices;
        uint16_t *indices;
        std::vector<const char *> validationLayers;
        std::vector<const char *> extensions;
        std::vector<const char *> deviceExtensions;
        std::vector<VkImageView> swapChainImageViews;
        std::vector<VkFramebuffer> swapChainFramebuffers;
        int currentFrame = 0;
        bool framebufferResized = false;

        const int MAX_FRAMES_IN_FLIGHT = 2;

        std::vector<VkImage> swapChainImages;
        bool enableValidationLayers;

        bool checkValidationLayers();

        void createInstance();

        void setupDebugMessenger();

        static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);


        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                            VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                            const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                            void *pUserData);

        bool isDeviceSuitable(VkPhysicalDevice device);

        void pickPhysicalDevice();

        void createVertexBuffer();

        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

        void createLogicalDevice();

        void createSurface();

        bool checkDeviceExtensionSupport(VkPhysicalDevice device);

        void createSwapChain();

        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

        void createImageViews();

        void createGraphicsPipeline();

        VkShaderModule createShaderModule(const char *code, size_t length);

        void createRenderPass();

        void createFramebuffers();

        void createCommandPool();

        void createCommandBuffer();

        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

        void createSyncObjects();

        void recreateSwapChain();

        void cleanupSwapChain();

        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

        void
        createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer,
                     VkDeviceMemory &bufferMemory);

        void createIndexBuffer();

        void createDescriptorSetLayout();

        void createUniformBuffers();

        void updateUniformBuffer(uint32_t currentImage);

        void createDescriptorPool();

        void createDescriptorSets();

        VkInstance vkInstance;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkSurfaceKHR surface;
        VkSwapchainKHR swapChain;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        VkDescriptorSetLayout descriptorSetLayout;
        VkPipelineLayout pipelineLayout;
        VkRenderPass renderPass;
        VkPipeline graphicsPipeline;
        VkCommandPool commandPool;
        std::vector<VkCommandBuffer> commandBuffers;
        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;

        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDeviceMemory> uniformBuffersMemory;
        std::vector<void *> uniformBuffersMapped;

        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;

        VkQueue graphicsQueue;
        VkQueue presentQueue;

        VkPhysicalDevice vkPhysicalDevice = VK_NULL_HANDLE;
        VkDevice vkLogicalDevice = VK_NULL_HANDLE;

        GLFWwindow *window;
    public:
        explicit Vulkan(GLFWwindow *, bool = true);

        void addGLFWExtensions();

        VkSurfaceFormatKHR surfaceFormat;
        VkPresentModeKHR presentMode;

        [[nodiscard]] std::vector<const char *> &getValidationLayers();

        [[nodiscard]] std::vector<const char *> &getExtensions();

        [[nodiscard]] std::vector<const char *> &getDeviceExtensions();

        VkInstance &getInstance();

        void drawFrame();

        void idle();

        void cleanup();

        void init();

        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

        static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);

        static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);

        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

        [[nodiscard]] VkRenderPass_T *getRenderPass() const;

        [[nodiscard]] VkQueue_T *getGraphicsQueue() const;

        [[nodiscard]] VkPhysicalDevice_T *getVkPhysicalDevice() const;

        [[nodiscard]] VkDevice_T *getVkLogicalDevice() const;

        [[nodiscard]] VkSurfaceKHR_T *getSurface() const;
    };
}
