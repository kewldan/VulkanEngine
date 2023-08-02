#pragma once

#include "GLFW/glfw3.h"
#include "misc/Destroyable.h"

namespace Engine {
    class Window : public Destroyable {
    private:
        GLFWwindow *window;
        int width;
        int height;

    public:
        Window(const char *title, int width, int height);

        void destroy() override;

        bool shouldClose();

        GLFWwindow *getHandle();
    };
}
