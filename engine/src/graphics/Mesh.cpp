#include "graphics/Mesh.h"

namespace Engine {
    Mesh::Mesh(Vertex *vertices, uint16_t *indices, int indexCount, int vertexCount) : vertices(vertices),
                                                                                       indices(indices),
                                                                                       indexCount(indexCount),
                                                                                       vertexCount(vertexCount) {}

    void
    Mesh::upload(VmaAllocator allocator) {
        {
            VkBufferCreateInfo bufferInfo = {};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = sizeof(Vertex) * vertexCount;
            bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

            //let the VMA library know that this data should be writeable by CPU, but also readable by GPU
            VmaAllocationCreateInfo vmaallocInfo = {};
            vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

            vmaCreateBuffer(allocator, &bufferInfo, &vmaallocInfo,
                            &vertexBuffer.buffer,
                            &vertexBuffer.allocation,
                            nullptr);

            void *data;
            vmaMapMemory(allocator, vertexBuffer.allocation, &data);

            memcpy(data, vertices, sizeof(Vertex) * vertexCount);

            vmaUnmapMemory(allocator, vertexBuffer.allocation);
        }

        {
            VkBufferCreateInfo bufferInfo = {};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = sizeof(uint16_t) * indexCount;
            bufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

            //let the VMA library know that this data should be writeable by CPU, but also readable by GPU
            VmaAllocationCreateInfo vmaallocInfo = {};
            vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

            vmaCreateBuffer(allocator, &bufferInfo, &vmaallocInfo,
                            &indexBuffer.buffer,
                            &indexBuffer.allocation,
                            nullptr);

            void *data;
            vmaMapMemory(allocator, indexBuffer.allocation, &data);

            memcpy(data, indices, sizeof(uint16_t) * indexCount);

            vmaUnmapMemory(allocator, indexBuffer.allocation);
        }
    }

    void Mesh::cleanup(VmaAllocator allocator) const {
        vmaDestroyBuffer(allocator, vertexBuffer.buffer, vertexBuffer.allocation);
        vmaDestroyBuffer(allocator, indexBuffer.buffer, indexBuffer.allocation);
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