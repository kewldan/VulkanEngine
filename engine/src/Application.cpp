#include "Application.h"

namespace Engine {
    bool Application::shouldClose() {
        return window->shouldClose();
    }

    Window *Application::getWindow() {
        return window;
    }

    void Application::create(VulkanContext context) {
        this->context = context;
    }
}