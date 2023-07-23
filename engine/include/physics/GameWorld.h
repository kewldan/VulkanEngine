#pragma once

#include "common/GameObject.h"
#include <btBulletDynamicsCommon.h>
#include <vector>

namespace Engine {
    class GameWorld {
    private:
        std::vector<GameObject *> gameObjects;
        btDefaultCollisionConfiguration *collisionConfiguration{};
        btCollisionDispatcher *dispatcher{};
        btDbvtBroadphase *broadphase{};
        btSequentialImpulseConstraintSolver *solver{};
        btDynamicsWorld *world{};
    public:
        GameWorld();

        void init();

        void cleanup();

        void render();

        GameObject *instantiate();
    };
}
