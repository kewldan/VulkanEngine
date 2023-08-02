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

        GUI::init(application.getWindow()->getHandle());

        uint64_t applicationPreInitTime = time();
        application.preInit();
        PLOGD << "Application pre-init took: " << floor(((double) time() - (double) applicationPreInitTime) * 0.001)
              << "ms";

        uint64_t applicationLoadTime = time();
        application.loadAssets();
        PLOGD << "Application assets load took: " << floor(((double) time() - (double) applicationLoadTime) * 0.001)
              << "ms";

        uint64_t applicationInitTime = time();
        application.init();
        PLOGD << "Application init took: " << floor(((double) time() - (double) applicationInitTime) * 0.001) << "ms";

        PLOGD << "Engine init took: " << floor(((double) time() - (double) engineInitTime) * 0.001) << "ms";

        renderThread = std::thread([&application, &vkHandler]() {
            while (!application.shouldClose()) {
                uint64_t t = time();
                if (smphSignalMainToThread.try_acquire_for(std::chrono::milliseconds(100))) {
                    data.gpuWait = time() - t;
                    t = time();
                    int currentFrame = vkHandler.syncNewFrame();

                    application.render(VulkanContext::commandBuffers[currentFrame]);

                    GUI::begin();
                    application.gui();
                    GUI::end();
                    GUI::render(VulkanContext::commandBuffers[currentFrame]);

                    vkHandler.endFrame();
                    data.lastGpuThread = time() - t;
                    memcpy(data.gpuSamples, data.gpuSamples + 1, 255 * sizeof(float));
                    data.gpuSamples[255] = (float) data.lastGpuThread;
                    smphSignalThreadToMain.release();
                }
                data.lastFps++;
            }
        });

        std::thread fpsCounter([&application]() {
            while (!application.shouldClose()) {
                memcpy(data.fpsSamples, data.fpsSamples + 1, 31 * sizeof(float));
                data.fpsSamples[31] = (float) data.lastFps * 4;
                data.lastFps = 0;
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
            }
        });

        while (!application.shouldClose()) {
            smphSignalMainToThread.release();
            uint64_t t = time();

            application.update();
            glfwPollEvents();

            data.lastCpuThread = time() - t;

            memcpy(data.cpuSamples, data.cpuSamples + 1, 255 * sizeof(float));
            data.cpuSamples[255] = (float) data.lastCpuThread;

            t = time();
            smphSignalThreadToMain.acquire();
            data.cpuWait = time() - t;
        }

        VulkanHelper::idle();

        renderThread.join();
        fpsCounter.join();
        
        GUI::destroy();
        application.destroy();
        vkHandler.destroy();

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