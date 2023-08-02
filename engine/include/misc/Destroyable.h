#ifndef VULKANENGINE_DESTROYABLE_H
#define VULKANENGINE_DESTROYABLE_H

namespace Engine {
    class Destroyable {
    public:
        virtual void destroy() = 0;
    };
}

#endif //VULKANENGINE_DESTROYABLE_H
