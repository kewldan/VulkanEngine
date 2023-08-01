#include <functional>
#include "graphics/Mesh.h"
#include "graphics/BufferHandler.h"
#include "graphics/VulkanContext.h"

namespace Engine {
    Mesh::Mesh(Vertex *vertices, uint16_t *indices, int indexCount, int vertexCount) : vertices(vertices),
                                                                                       indices(indices),
                                                                                       indexCount(indexCount),
                                                                                       vertexCount(vertexCount) {}

    void
    Mesh::upload() {
        VkDeviceSize verticesSize = sizeof(Vertex) * vertexCount;
        vertexBuffer = BufferHandler::createGpuBuffer(verticesSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, vertices);

        VkDeviceSize indicesSize = sizeof(uint16_t) * indexCount;
        indexBuffer = BufferHandler::createGpuBuffer(indicesSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, indices);
    }

    void Mesh::cleanup() const {
        vmaDestroyBuffer(VulkanContext::allocator, vertexBuffer.buffer, vertexBuffer.allocation);
        vmaDestroyBuffer(VulkanContext::allocator, indexBuffer.buffer, indexBuffer.allocation);
    }

    Mesh::Mesh() = default;

    VkVertexInputBindingDescription Vertex::getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    VkVertexInputAttributeDescription *Vertex::getAttributeDescriptions(int *count) {
        static auto attributeDescriptions = new VkVertexInputAttributeDescription[3];

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, normal);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, uv);

        *count = 3;

        return attributeDescriptions;
    }

}