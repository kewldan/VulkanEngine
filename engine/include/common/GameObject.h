#pragma once

#include "graphics/Mesh.h"
#include "physics/Transform.h"
#include "misc/Destroyable.h"
#include "io/Asset.h"

namespace Engine {
    class GameObject : public Transform, Destroyable, public Asset {
    private:
        const char *filename{};
    public:
        Mesh *meshes{};
        size_t meshCount{};

        GameObject();

        explicit GameObject(const char *filename);

        void upload() const;

        void destroy() override;

        void load() override;
    };
}
