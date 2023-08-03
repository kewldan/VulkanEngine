#pragma once

#include <vector>
#include <memory>
#include <optional>
#include "GLFW/glfw3.h"
#include <vulkan/vulkan.h>
#include "vk_mem_alloc.h"
#include "misc/Destroyable.h"
#include "VulkanTypes.h"

namespace Engine {
    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                          const VkAllocationCallbacks *pAllocator,
                                          VkDebugUtilsMessengerEXT *pDebugMessenger);

    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                       const VkAllocationCallbacks *pAllocator);

    class VulkanHelper : public Destroyable {
    public:
        std::vector<const char *> validationLayers;

        bool framebufferResized = false;

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

        static void createCommandPool();

        static void createPipelineCache();

        void createSyncObjects();

        static void createDescriptorPool();

        [[nodiscard]] static VkSampleCountFlagBits getMaxUsableSampleCount();

        VkDebugUtilsMessengerEXT debugMessenger{};

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;

        uint32_t imageIndex{};

        GLFWwindow *window;
    public:
        explicit VulkanHelper(GLFWwindow *wind, bool validation = true);

        [[nodiscard]] std::vector<const char *> &getValidationLayers();

        void syncNewFrame();

        void endFrame();

        void destroy() override;

        void init(std::vector<const char *> &extensions);
    };
}
