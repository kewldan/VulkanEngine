#include "input/InputManager.h"
#include "imgui_impl_glfw.h"

namespace Engine {
    void InputManager::registerCallbacks(GLFWwindow *window) {
        glfwSetKeyCallback(window, [](GLFWwindow *win, int key, int scancode, int action, int mods) {
            ImGui_ImplGlfw_KeyCallback(win, key, scancode, action, mods);

            if (action == GLFW_PRESS) {
                for (const std::function<void(int)> &func: keyPressListeners) {
                    func(key);
                }
            } else if (action == GLFW_RELEASE) {
                for (const std::function<void(int)> &func: keyReleaseListeners) {
                    func(key);
                }
            }
        });

        glfwSetMouseButtonCallback(window, [](GLFWwindow *win, int button, int action, int mods) {
            ImGui_ImplGlfw_MouseButtonCallback(win, button, action, mods);

            if (action == GLFW_PRESS) {
                for (const std::function<void(int)> &func: mousePressListeners) {
                    func(button);
                }
            } else if (action == GLFW_RELEASE) {
                for (const std::function<void(int)> &func: mouseReleaseListeners) {
                    func(button);
                }
            }
        });

        glfwSetCursorPosCallback(window, [](GLFWwindow *win, double x, double y) {
            ImGui_ImplGlfw_CursorPosCallback(win, x, y);

            mousePosition.x = (float) x;
            mousePosition.y = (float) y;
        });
    }

    std::vector<std::function<void(int)>> InputManager::keyPressListeners = std::vector<std::function<void(int)>>();
    std::vector<std::function<void(int)>> InputManager::keyReleaseListeners = std::vector<std::function<void(int)>>();
    std::vector<std::function<void(int)>> InputManager::mousePressListeners = std::vector<std::function<void(int)>>();
    std::vector<std::function<void(int)>> InputManager::mouseReleaseListeners = std::vector<std::function<void(int)>>();
    glm::vec2 InputManager::mousePosition = glm::vec2(0.f);
    bool InputManager::lockCursor = false;

    void InputManager::setRawInput(GLFWwindow *window, bool value) {
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, value);
    }

    void InputManager::setCursorMode(GLFWwindow *window, bool disabled) {
        glfwSetInputMode(window, GLFW_CURSOR, disabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }

    void InputManager::setCursorPos(GLFWwindow *window, float x, float y) {
        glfwSetCursorPos(window, (double) x, (double) y);
    }

    void InputManager::setLockCursor(GLFWwindow *window, bool value) {
        setCursorMode(window, value);
        lockCursor = value;
    }

    glm::vec2 InputManager::getCursorDelta(GLFWwindow *window) {
        if (lockCursor) {
            int s[2];
            glfwGetFramebufferSize(window, &s[0], &s[1]);
            glm::vec2 center = glm::vec2(s[0], s[1]) * 0.5f;
            glm::vec2 delta = mousePosition - center;
            glfwSetCursorPos(window, center.x, center.y);
            mousePosition.x = center.x;
            mousePosition.y = center.y;

            return delta;
        } else {
            return glm::vec2(0.f);
        }
    }
}