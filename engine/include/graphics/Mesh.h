#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

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
