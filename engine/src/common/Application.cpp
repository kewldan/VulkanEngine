#include "common/Application.h"

namespace Engine {
    bool Application::shouldClose() {
        return window->shouldClose();
    }

    const std::unique_ptr<Window> &Application::getWindow() const {
        return window;
    }
}