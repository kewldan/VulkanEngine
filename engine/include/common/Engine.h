#pragma once

#include "common/Application.h"
#include <thread>
#include <semaphore>

namespace Engine {
    struct EngineData {
        uint64_t lastCpuThread;
        uint64_t lastGpuThread;
    };

    class Engine {
        static std::thread renderThread;
        static std::binary_semaphore smphSignalMainToThread, smphSignalThreadToMain;
    public:
        static EngineData data;

        static void run(Application &application);

        static uint64_t time();
    };
}
