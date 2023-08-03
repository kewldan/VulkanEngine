#pragma once

#include "graphics/Mesh.h"
#include "physics/Transform.h"
#include "misc/Destroyable.h"
#include "io/Asset.h"
#include <glm/glm.hpp>
#include <btBulletDynamicsCommon.h>

namespace Engine {
    class GameObject : public Destroyable, public Asset {
    private:
        const char *filename{};
    public:
        Mesh *meshes{};
        size_t meshCount{};
        glm::vec4 color = glm::vec4(1, 1, 1, 1);
        btRigidBody *rb{};

        GameObject();

        explicit GameObject(const char *filename, float mass = 0.f);

        void destroy() override;

        void load() override;

        void getModel(glm::mat4 *mvp) const;
    };
}
