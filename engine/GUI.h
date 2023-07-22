#pragma once

#include <vulkan/vulkan.h>

namespace Engine {
    class GUI {
        static void init(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device, VkQueue graphicsQueue,
                         VkRenderPass renderPass);

        static void draw();

        static void render(VkCommandBuffer buffer);

        static void uploadFonts(VkCommandBuffer buffer);
    };
}
