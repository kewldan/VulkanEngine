#pragma once

#include "GLFW/glfw3.h"

namespace Engine {
    class Window {
    private:
        GLFWwindow *window;
        int width;
        int height;

    public:
        Window(const char *title, int width, int height);

        void cleanup();

        bool shouldClose();

        GLFWwindow *getHandle();
    };
}
