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

    class Mesh : public Destroyable {
    public:
        Vertex *vertices = nullptr;
        uint16_t *indices = nullptr;
        size_t indexCount = 0;
        size_t vertexCount = 0;
        AllocatedBuffer vertexBuffer{};
        AllocatedBuffer indexBuffer{};

        Mesh();

        Mesh(Vertex *vertices, uint16_t *indices, size_t indexCount, size_t vertexCount);

        void upload();

        void destroy() override;
    };
}
