#pragma once

#include "graphics/VulkanHelper.h"
#include "Window.h"
#include "graphics/VulkanContext.h"
#include "input/InputManager.h"
#include <memory>

namespace Engine {
    class Application : public Destroyable {
    protected:
        std::unique_ptr<Window> window;
    public:
        virtual void createWindow() = 0;

        virtual void loadAssets() = 0;

        virtual void preInit() = 0;

        virtual void init() = 0;

        virtual void
        render(VkCommandBuffer commandBuffer) = 0;

        virtual void gui() = 0;

        virtual void update() = 0;

        virtual bool shouldClose();

        [[nodiscard]] const std::unique_ptr<Window> &getWindow() const;
    };
}
