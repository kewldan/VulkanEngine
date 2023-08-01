#pragma once

#include "common/GameObject.h"
#include <vector>

namespace Engine {

    class GameWorld {
    private:
        std::vector<GameObject *> gameObjects;
    public:
        GameWorld();

        void init();

        void cleanup();

        void render();

        GameObject *instantiate();
    };
}
