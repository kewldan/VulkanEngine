#pragma once

#include "common/Application.h"
#include <thread>
#include <semaphore>
#include <array>

namespace Engine {
    struct EngineData {
        uint64_t lastCpuThread{};
        uint64_t lastGpuThread{};
        uint64_t cpuWait{};
        uint64_t gpuWait{};
        float cpuSamples[256], gpuSamples[256], fpsSamples[32];
        std::atomic<uint32_t> lastFps;
    };

    class Engine {
        static std::thread renderThread;
        static std::binary_semaphore smphSignalMainToThread, smphSignalThreadToMain;
        static std::vector<std::function<void()>> cleanupResources;
    public:
        static EngineData data;

        static void run(Application &application);

        static uint64_t time();
    };
}
