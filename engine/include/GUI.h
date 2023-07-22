#pragma once

#include "vulkan/vulkan.h"
#include "GLFW/glfw3.h"

namespace Engine {
    class GUI {
        static VkDescriptorPool imguiPool;
    public:
        static void init(GLFWwindow *window, VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device,
                         VkQueue graphicsQueue,
                         VkRenderPass renderPass, VkCommandPool commandPool, VkCommandBuffer commandBuffer);

        static void draw();

        static void render(VkCommandBuffer buffer);

        static void cleanup(VkDevice device);
    };
}
