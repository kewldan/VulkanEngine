#include <vector>
#include <set>
#include <stdexcept>
#include "graphics/DeviceHandler.h"
#include "graphics/VulkanContext.h"
#include "plog/Log.h"

namespace Engine {
    void
    DeviceHandler::createLogicalDevice(std::vector<const char *> &deviceExtensions,
                                       bool enableValidationLayers, std::vector<const char *> &validationLayers) {
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {VulkanContext::indices.graphicsFamily.value(),
                                                  VulkanContext::indices.presentFamily.value()};

        float queuePriority = 1.0f;
        for (uint32_t queueFamily: uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(VulkanContext::physicalDevice, &createInfo, nullptr, &VulkanContext::device) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }

        vkGetDeviceQueue(VulkanContext::device, VulkanContext::indices.graphicsFamily.value(), 0,
                         &VulkanContext::graphicsQueue);
        vkGetDeviceQueue(VulkanContext::device, VulkanContext::indices.presentFamily.value(), 0,
                         &VulkanContext::presentQueue);
    }

    void
    DeviceHandler::pickPhysicalDevice() {
        uint32_t deviceCount = 0;
        if (vkEnumeratePhysicalDevices(VulkanContext::instance, &deviceCount, nullptr) != VK_SUCCESS) {
            throw std::runtime_error("failed to enumerate physical devices!");
        }

        if (deviceCount == 0) {
            throw std::runtime_error("failed to find GPUs with VulkanHelper support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        if (vkEnumeratePhysicalDevices(VulkanContext::instance, &deviceCount, devices.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to enumerate physical devices!");
        }

        VulkanContext::physicalDevice = devices[0];

        if (VulkanContext::physicalDevice == VK_NULL_HANDLE) {
            throw std::runtime_error("failed to find a suitable GPU!");
        }
    }

    QueueFamilyIndices DeviceHandler::findQueueFamilies() {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(VulkanContext::physicalDevice, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(VulkanContext::physicalDevice, &queueFamilyCount,
                                                 queueFamilies.data());

        int i = 0;
        for (const auto &queueFamily: queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(VulkanContext::physicalDevice, i, VulkanContext::surface,
                                                 &presentSupport);

            if (presentSupport) {
                indices.presentFamily = i;
            }

            if (indices.isComplete()) {
                break;
            }

            i++;
        }

        return indices;
    }

    void
    DeviceHandler::getDevices(std::vector<const char *> &deviceExtensions, bool enableValidationLayers,
                              std::vector<const char *> &validationLayers) {

        pickPhysicalDevice();

        VulkanContext::indices = findQueueFamilies();

        createLogicalDevice(deviceExtensions, enableValidationLayers, validationLayers);
    }
}
