#pragma once

#include <vector>
#include "GLFW/glfw3.h"
#include "glm/vec2.hpp"
#include <functional>

namespace Engine {
    class InputManager {
    public:
        static bool lockCursor;
        static glm::vec2 mousePosition;
        static std::vector<std::function<void(int)>> keyPressListeners;
        static std::vector<std::function<void(int)>> keyReleaseListeners;
        static std::vector<std::function<void(int)>> mousePressListeners;
        static std::vector<std::function<void(int)>> mouseReleaseListeners;

        static void registerCallbacks(GLFWwindow *window);

        static void setRawInput(GLFWwindow *window, bool value);

        static void setCursorMode(GLFWwindow *window, bool disabled);

        static void setCursorPos(GLFWwindow *window, float x, float y);

        static void setLockCursor(GLFWwindow *window, bool value);

        static glm::vec2 getCursorDelta(GLFWwindow *window);
    };
}
