#pragma once

#include "common/Application.h"

namespace Engine {
    class Engine {
    public:
        static void run(Application &application);

        static uint64_t time();
    };
}
