#include <cassert>
#include <stdexcept>
#include "common/Window.h"

namespace Engine {
    Window::Window(const char *title, int width, int height) : width(width), height(height) {
        assert(title != nullptr);
        assert(width > 0);
        assert(height > 0);

        window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (window == nullptr) {
            throw std::runtime_error("failed to create window");
        }

        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    }

    void Window::cleanup() {
        glfwDestroyWindow(window);
    }

    bool Window::shouldClose() {
        return glfwWindowShouldClose(window);
    }

    GLFWwindow *Window::getHandle() {
        return window;
    }
}