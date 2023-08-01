#pragma once

#include "graphics/VulkanHelper.h"
#include "GLFW/glfw3.h"

namespace Engine {
    class GUI {
        static VkDescriptorPool imguiPool;
    public:
        static void init(GLFWwindow *window);

        static void begin();

        static void end();

        static void render(VkCommandBuffer buffer);

        static void cleanup(VkDevice device);
    };
}
