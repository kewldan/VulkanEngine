#pragma once

#include <vulkan/vulkan.h>
#include "Vulkan.h"

namespace Engine {
    class DeviceHandler {
    private:
        static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

        static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

    public:
        static VkPhysicalDevice pickPhysicalDevice(VkInstance instance);

        static VkDevice
        createLogicalDevice(VkPhysicalDevice physicalDevice, std::vector<const char *> &deviceExtensions,
                            bool enableValidationLayers, std::vector<const char *> &validationLayers,
                            VkQueue *graphicsQueue, VkQueue *presentQueue, QueueFamilyIndices indices);

        static void
        getDevices(SwapChainSupportDetails *swapChainSupport, VkPhysicalDevice *physicalDevice, VkDevice *device,
                   QueueFamilyIndices *familyIndices,
                   VkInstance instance,
                   std::vector<const char *> &deviceExtensions, bool enableValidationLayers,
                   std::vector<const char *> &validationLayers, VkQueue *graphicsQueue, VkQueue *presentQueue,
                   VkSurfaceKHR surface);
    };
}