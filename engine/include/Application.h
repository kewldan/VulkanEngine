#pragma once

#include <vulkan/vulkan.h>
#include "Window.h"

namespace Engine {
    class Application {
    protected:
        VkInstance vkInstance{};
        Window *window{};
    public:
        virtual void create(VkInstance &);

        virtual void createWindow();

        virtual Window *getWindow();

        virtual void init();

        virtual void loop();

        virtual void cleanup();

        virtual bool shouldClose();
    };
}
