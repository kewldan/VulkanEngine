#pragma once

#include "common/GameObject.h"
#include "graphics/RenderPipeline.h"
#include <BulletDynamics/btBulletDynamicsCommon.h>
#include <vector>

namespace Engine {

    class GameWorld : public Destroyable {
    private:
        std::vector<GameObject *> gameObjects;

        btDiscreteDynamicsWorld *dynamicsWorld{};
    public:
        GameWorld();

        void init();

        void render(const RenderPipeline &pipeline);

        void destroy() override;

        void load();

        GameObject *instantiate();

        GameObject *instantiate(const char *filename);
    };
}
