#pragma once

#include "graphics/Mesh.h"
#include "physics/Transform.h"

namespace Engine {
    class GameObject : public Transform {
    public:
        Mesh *meshes{};
        int meshCount{};

        GameObject();

        void upload(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool,
                    VkQueue graphicsQueue) const;

        void cleanup(VkDevice device) const;
    };
}
