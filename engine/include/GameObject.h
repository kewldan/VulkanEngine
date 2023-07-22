#pragma once

#include "Mesh.h"
#include "Transform.h"

namespace Engine {
    class GameObject : public Transform {
        Mesh *meshes{};
        int meshCount{};
    public:
        GameObject();
    };
}
