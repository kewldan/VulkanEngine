#pragma once

#include <vulkan/vulkan.h>
#include "Window.h"
#include "VulkanContext.h"

namespace Engine {
    class Application {
    protected:
        VulkanContext context{};
        Window *window{};
    public:
        virtual void create(VulkanContext context);

        virtual void createWindow() = 0;

        virtual Window *getWindow();

        virtual void init() = 0;

        virtual void render(VkCommandBuffer commandBuffer, VkDescriptorSet *descriptorSet) = 0;

        virtual void gui() = 0;

        virtual void update() = 0;

        virtual void cleanup() = 0;

        virtual bool shouldClose();
    };
}
