#pragma once

#include "graphics/VulkanHelper.h"
#include "GLFW/glfw3.h"

namespace Engine {
    class GUI {
        static VkDescriptorPool imguiPool;
    public:
        static void init(GLFWwindow *window, VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device,
                         VkQueue graphicsQueue,
                         VkRenderPass renderPass, VkCommandPool commandPool, VkCommandBuffer commandBuffer,
                         VkSampleCountFlagBits sampleCount);

        static void begin();

        static void end();

        static void render(VkCommandBuffer buffer);

        static void cleanup(VkDevice device);
    };
}
