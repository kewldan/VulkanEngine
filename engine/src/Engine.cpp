#include "Engine.h"
#include "Vulkan.h"

namespace Engine {
    void Engine::run(Application &application) {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        application.createWindow();

        Vulkan vkHandler(application.getWindow()->getHandle(), true);

        vkHandler.addGLFWExtensions();
        vkHandler.getExtensions().push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        vkHandler.getDeviceExtensions().push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

        vkHandler.getValidationLayers().push_back("VK_LAYER_KHRONOS_validation");

        vkHandler.init();

        VkInstance vkInstance = vkHandler.getInstance();
        application.create(vkInstance);

        application.init();
        while (!application.shouldClose()) {
            application.loop();
            vkHandler.drawFrame();
            glfwPollEvents();
        }

        vkHandler.idle();

        application.cleanup();
        vkHandler.cleanup();

        glfwTerminate();
    }
}