#include "Engine.h"
#include "Vulkan.h"
#include "plog/Init.h"
#include "plog/Formatters/FuncMessageFormatter.h"
#include "plog/Appenders/ColorConsoleAppender.h"
#include "GUI.h"

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
        application.create({
                                   vkInstance,
                                   vkHandler.vkPhysicalDevice,
                                   vkHandler.vkLogicalDevice,
                                   vkHandler.graphicsQueue,
                                   vkHandler.presentQueue,
                                   vkHandler.commandPool,
                                   vkHandler.pipelineLayout
                           });

        application.init();
        while (!application.shouldClose()) {
            application.update();

            int currentFrame = vkHandler.syncNewFrame();

            vkHandler.updateUniformBuffer(currentFrame);

            application.render(vkHandler.commandBuffers[currentFrame], &vkHandler.descriptorSets[currentFrame]);

            GUI::begin();
            application.gui();
            GUI::end();
            GUI::render(vkHandler.commandBuffers[currentFrame]);

            vkHandler.endFrame();

            glfwPollEvents();
        }

        vkHandler.idle();

        application.cleanup();
        vkHandler.cleanup();

        glfwTerminate();
    }
}