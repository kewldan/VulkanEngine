#include "Application.h"

namespace Engine {
    void Application::create(VkInstance &instance) {
        vkInstance = instance;
    }

    void Application::init() {

    }

    void Application::loop() {

    }

    void Application::cleanup() {

    }

    bool Application::shouldClose() {
        return false;
    }

    void Application::createWindow() {

    }

    Window *Application::getWindow() {
        return window;
    }
}