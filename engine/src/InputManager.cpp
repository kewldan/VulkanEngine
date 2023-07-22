#include "InputManager.h"

namespace Engine {
    void InputManager::registerCallbacks(GLFWwindow *window) {
        glfwSetKeyCallback(window, [](GLFWwindow *, int key, int scancode, int action, int mods) {
            if (action == GLFW_PRESS) {
                for (void (*func)(int): keyPressListeners) {
                    func(key);
                }
            } else if (action == GLFW_RELEASE) {
                for (void (*func)(int): keyReleaseListeners) {
                    func(key);
                }
            }
        });

        glfwSetMouseButtonCallback(window, [](GLFWwindow *, int, int, int) {
            
        });
    }

    std::vector<void (*)(int)> InputManager::keyPressListeners = std::vector<void (*)(int)>();
    std::vector<void (*)(int)> InputManager::keyReleaseListeners = std::vector<void (*)(int)>();
    std::vector<void (*)(int)> InputManager::mousePressListeners = std::vector<void (*)(int)>();
    std::vector<void (*)(int)> InputManager::mouseReleaseListeners = std::vector<void (*)(int)>();

    void InputManager::setRawInput(GLFWwindow *window, bool value) {
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, value);
    }
}