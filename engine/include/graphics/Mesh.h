#pragma once

#include "glm/glm.hpp"
#include "graphics/VulkanHelper.h"

namespace Engine {
    struct Vertex {
    public:
        glm::vec3 pos;
        glm::vec3 normal;
        glm::vec2 uv;

        static VkVertexInputBindingDescription getBindingDescription();

        static VkVertexInputAttributeDescription *getAttributeDescriptions(int *);
    };

    class Mesh {
    public:
        Vertex *vertices = nullptr;
        uint16_t *indices = nullptr;
        int indexCount = 0;
        int vertexCount = 0;
        AllocatedBuffer vertexBuffer{};
        AllocatedBuffer indexBuffer{};

        Mesh();

        Mesh(Vertex *vertices, uint16_t *indices, int indexCount, int vertexCount);

        void upload();

        void cleanup() const;
    };
}
