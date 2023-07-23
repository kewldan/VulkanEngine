#pragma once

#include "graphics/Mesh.h"
#include "physics/Transform.h"

namespace Engine {
    class GameObject : public Transform {
    public:
        Mesh *meshes{};
        int meshCount{};

        GameObject();

        void upload(VmaAllocator allocator) const;

        void cleanup(VmaAllocator allocator) const;
    };
}
