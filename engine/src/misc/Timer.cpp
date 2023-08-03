#include <chrono>
#include "misc/Timer.h"

namespace Engine {

    unsigned long long Timer::getTime() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()
        ).count();
    }
} // Engine