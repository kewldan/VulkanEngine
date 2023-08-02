#pragma once

#include "graphics/Mesh.h"
#include "physics/Transform.h"
#include "misc/Destroyable.h"
#include "io/Asset.h"
#include <glm/glm.hpp>

namespace Engine {
    class GameObject : public Transform, Destroyable, public Asset {
    private:
        const char *filename{};
    public:
        Mesh *meshes{};
        size_t meshCount{};
        glm::vec4 color = glm::vec4(1, 1, 1, 1);

        GameObject();

        explicit GameObject(const char *filename);

        void upload() const;

        void destroy() override;

        void load() override;
    };
}
