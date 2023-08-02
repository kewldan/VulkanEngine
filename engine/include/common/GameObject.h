#pragma once

#include "graphics/Mesh.h"
#include "physics/Transform.h"
#include "misc/Destroyable.h"

namespace Engine {
    class GameObject : public Transform, Destroyable {
    public:
        Mesh *meshes{};
        size_t meshCount{};

        GameObject();

        void upload() const;

        void destroy() override;
    };
}
