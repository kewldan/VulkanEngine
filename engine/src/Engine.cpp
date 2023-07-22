#include "Engine.h"
#include "Vulkan.h"
#include "plog/Init.h"
#include "plog/Formatters/FuncMessageFormatter.h"
#include "plog/Appenders/ColorConsoleAppender.h"

namespace Engine {
    void Engine::run(Application &application) {
        static plog::ColorConsoleAppender<plog::FuncMessageFormatter> consoleAppender;
        plog::init(plog::verbose, &consoleAppender);

        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        application.createWindow();

        Vulkan vkHandler(application.getWindow()->getHandle(), true);

        vkHandler.getValidationLayers().push_back("VK_LAYER_KHRONOS_validation");

        vkHandler.init(std::vector<const char *>() = {VK_EXT_DEBUG_UTILS_EXTENSION_NAME});

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