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
        PLOGD << "Application init took: " << floor(((double) time() - (double) applicationInitTime) * 0.001) << "ms";

        uint64_t applicationLoadTime = time();
        application.loadAssets();
        PLOGD << "Application assets load took: " << floor(((double) time() - (double) applicationLoadTime) * 0.001)
              << "ms";


        PLOGD << "Engine init took: " << floor(((double) time() - (double) engineInitTime) * 0.001) << "ms";

        renderThread = std::thread([&application, &vkHandler]() {
            while (!application.shouldClose()) {
                if (smphSignalMainToThread.try_acquire_for(std::chrono::milliseconds(100))) {
                    uint64_t t = time();
                    int currentFrame = vkHandler.syncNewFrame();

                    application.render(vkHandler.commandBuffers[currentFrame]);

                    GUI::begin();
                    application.gui();
                    GUI::end();
                    GUI::render(vkHandler.commandBuffers[currentFrame]);

                    vkHandler.endFrame();
                    data.lastGpuThread = time() - t;
                    smphSignalThreadToMain.release();
                }
            }
        });

        while (!application.shouldClose()) {
            smphSignalMainToThread.release();
            uint64_t t = time();

            application.update();
            glfwPollEvents();

            data.lastCpuThread = time() - t;
            smphSignalThreadToMain.acquire();
        }

        vkHandler.idle();

        renderThread.join();

        application.cleanup();
        vkHandler.cleanup();

        glfwTerminate();
    }

    uint64_t Engine::time() {
        return std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
    }

    std::binary_semaphore Engine::smphSignalThreadToMain = std::binary_semaphore(0);
    std::binary_semaphore Engine::smphSignalMainToThread = std::binary_semaphore(0);
    std::thread Engine::renderThread{};
    EngineData Engine::data{};
}