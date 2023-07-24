#pragma once

#include "graphics/VulkanHelper.h"
#include "Window.h"
#include "graphics/VulkanContext.h"
#include "input/InputManager.h"
#include <memory>

namespace Engine {
    class Application {
    protected:
        VulkanContext context;
        std::unique_ptr<Window> window;
    public:
        virtual void create(VulkanContext context);

        virtual void createWindow() = 0;

        virtual void loadAssets() = 0;

        virtual void preInit() = 0;

        virtual void init() = 0;

        virtual void
        render(VkCommandBuffer commandBuffer) = 0;

        virtual void gui() = 0;

        virtual void update() = 0;

        virtual void cleanup() = 0;

        virtual bool shouldClose();

        [[nodiscard]] const std::unique_ptr<Window> &getWindow() const;
    };
}
