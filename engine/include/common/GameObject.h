#pragma once

#include "graphics/Mesh.h"
#include "physics/Transform.h"
#include "misc/Destroyable.h"
#include "io/Asset.h"
#include <glm/glm.hpp>
#include <btBulletDynamicsCommon.h>

namespace Engine {
    class GameObject : public Transform, Destroyable, public Asset {
    private:
        const char *filename{};
        btVector3 localInertia;
        btCollisionShape *collisionShape;
        btDefaultMotionState *motionState;
    public:
        Mesh *meshes{};
        size_t meshCount{};
        glm::vec4 color = glm::vec4(1, 1, 1, 1);
        btRigidBody *rb;

        GameObject();

        explicit GameObject(const char *filename, float mass = 0.f, btCollisionShape *shape = nullptr,
                            btVector3 position = btVector3(0.f, 0.f, 0.f));

        void upload() const;

        void destroy() override;

        void load() override;
    };
}
