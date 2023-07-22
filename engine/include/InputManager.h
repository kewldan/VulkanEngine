#pragma once

#include <vector>
#include "GLFW/glfw3.h"

namespace Engine {
    class InputManager {
    public:
        static std::vector<void (*)(int)> keyPressListeners;
        static std::vector<void (*)(int)> keyReleaseListeners;
        static std::vector<void (*)(int)> mousePressListeners;
        static std::vector<void (*)(int)> mouseReleaseListeners;

        static void registerCallbacks(GLFWwindow *window);

        static void setRawInput(GLFWwindow *window, bool value);
    };
}
