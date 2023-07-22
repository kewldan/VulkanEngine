#pragma once

#include "Vertex.h"

namespace Engine {
    class Mesh {
    public:
        Vertex *vertices = nullptr;
        uint16_t *indices = nullptr;
        int indexCount = 0;
        int vertexCount = 0;
        VkBuffer vertexBuffer{};
        VkDeviceMemory vertexBufferMemory{};
        VkBuffer indexBuffer{};
        VkDeviceMemory indexBufferMemory{};

        Mesh();

        Mesh(Vertex *vertices, uint16_t *indices, int indexCount, int vertexCount);

        void upload(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue);

        void cleanup(VkDevice device) const;
    };
}
