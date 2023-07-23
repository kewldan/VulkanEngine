#pragma once

#include "common/GameObject.h"
#include <physx/PxPhysicsAPI.h>
#include <vector>

namespace Engine {
    using namespace physx;

    class GameWorld {
    private:
        std::vector<GameObject *> gameObjects;
        PxDefaultErrorCallback gDefaultErrorCallback;
        PxDefaultAllocator gDefaultAllocatorCallback;
        PxFoundation *foundation;
        PxPvd *pvd;
        PxPhysics *physics;
        PxScene *scene;
    public:
        GameWorld();

        void init();

        void cleanup();

        void render();

        GameObject *instantiate();
    };
}
