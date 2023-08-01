#pragma once

#include "graphics/Mesh.h"
#include "physics/Transform.h"

namespace Engine {
    class GameObject : public Transform {
    public:
        Mesh *meshes{};
        int meshCount{};

        GameObject();

        void upload() const;

        void cleanup() const;
    };
}
