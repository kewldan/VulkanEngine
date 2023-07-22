#include "Application.h"

namespace Engine {
    bool Application::shouldClose() {
        return window->shouldClose();
    }

    void Application::create(VulkanContext context) {
        this->context = context;
    }

    const std::unique_ptr<Window> &Application::getWindow() const {
        return window;
    }
}