#pragma once

#include "VulkanHelper.h"

namespace Engine {
    class DeviceHandler {
    private:
        static QueueFamilyIndices findQueueFamilies();

    public:
        static void pickPhysicalDevice();

        static void
        createLogicalDevice(std::vector<const char *> &deviceExtensions,
                            bool enableValidationLayers, std::vector<const char *> &validationLayers,
                            QueueFamilyIndices indices);

        static void
        getDevices(QueueFamilyIndices *familyIndices,
                   std::vector<const char *> &deviceExtensions, bool enableValidationLayers,
                   std::vector<const char *> &validationLayers);
    };
}