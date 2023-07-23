#include "common/Engine.h"
#include "graphics/VulkanHelper.h"
#include "plog/Init.h"
#include "plog/Formatters/FuncMessageFormatter.h"
#include "plog/Appenders/ColorConsoleAppender.h"
#include "gui/GUI.h"
#include "plog/Log.h"
#include <chrono>

namespace Engine {
    void Engine::run(Application &application) {
        uint64_t engineInitTime = time();

        static plog::ColorConsoleAppender<plog::FuncMessageFormatter> consoleAppender;
        plog::init(plog::verbose, &consoleAppender);

        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        application.createWindow();

#ifndef NDEBUG
        const bool validationLayers = true;
#else
        const bool validationLayers = false;
#endif

        VulkanHelper vkHandler(application.getWindow()->getHandle(), validationLayers);

#ifndef NDEBUG
        vkHandler.getValidationLayers().push_back("VK_LAYER_KHRONOS_validation");
#endif

        vkHandler.init(std::vector<const char *>() = {VK_EXT_DEBUG_UTILS_EXTENSION_NAME});

        VkInstance vkInstance = vkHandler.getInstance();
        application.create({
                                   vkInstance,
                                   vkHandler.vkPhysicalDevice,
                                   vkHandler.vkLogicalDevice,
                                   vkHandler.graphicsQueue,
                                   vkHandler.presentQueue,
                                   vkHandler.commandPool,
                                   vkHandler.renderPass,
                                   &vkHandler.swapChainExtent,
                                   vkHandler.descriptorPool,
                                   vkHandler.allocator,
                                   &vkHandler.currentFrame
                           });

        GUI::init(application.getWindow()->getHandle(), vkInstance, vkHandler.vkPhysicalDevice,
                  vkHandler.vkLogicalDevice, vkHandler.graphicsQueue, vkHandler.renderPass, vkHandler.commandPool,
                  vkHandler.commandBuffers[0]);

        uint64_t applicationInitTime = time();
        application.init();

        PLOGD << "Engine init took: " << time() - engineInitTime << "ms";
        PLOGD << "Application init took: " << time() - applicationInitTime << "ms";

        while (!application.shouldClose()) {
            application.update();

            int currentFrame = vkHandler.syncNewFrame();

            application.render(vkHandler.commandBuffers[currentFrame]);

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

    uint64_t Engine::time() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
    }
}