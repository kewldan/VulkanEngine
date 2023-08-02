#pragma once

#include "common/GameObject.h"
#include <vector>

namespace Engine {

    class GameWorld : public Destroyable {
    private:
        std::vector<GameObject *> gameObjects;
    public:
        GameWorld();

        void init();

        void render();

        void destroy() override;

        GameObject *instantiate();
    };
}
