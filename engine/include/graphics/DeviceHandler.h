#pragma once

#include "VulkanHelper.h"
#include "VulkanHelper.h"

namespace Engine {
    class DeviceHandler {
    private:
        static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

    public:
        static VkPhysicalDevice pickPhysicalDevice(VkInstance instance);

        static VkDevice
        createLogicalDevice(VkPhysicalDevice physicalDevice, std::vector<const char *> &deviceExtensions,
                            bool enableValidationLayers, std::vector<const char *> &validationLayers,
                            VkQueue *graphicsQueue, VkQueue *presentQueue, QueueFamilyIndices indices);

        static void
        getDevices(VkPhysicalDevice *physicalDevice, VkDevice *device,
                   QueueFamilyIndices *familyIndices,
                   VkInstance instance,
                   std::vector<const char *> &deviceExtensions, bool enableValidationLayers,
                   std::vector<const char *> &validationLayers, VkQueue *graphicsQueue, VkQueue *presentQueue,
                   VkSurfaceKHR surface);
    };
}