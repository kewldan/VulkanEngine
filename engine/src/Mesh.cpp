#include "Mesh.h"
#include "vulkan/BufferHandler.h"
#include "vulkan/DebugUtils.h"

namespace Engine {
    Mesh::Mesh(Vertex *vertices, uint16_t *indices, int indexCount, int vertexCount) : vertices(vertices),
                                                                                       indices(indices),
                                                                                       indexCount(indexCount),
                                                                                       vertexCount(vertexCount) {}

    void
    Mesh::upload(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue) {
        {
            VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

            VkBuffer stagingBuffer;
            VkDeviceMemory stagingBufferMemory;
            BufferHandler::createBuffer(physicalDevice, device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                        stagingBuffer,
                                        stagingBufferMemory);

            void *data;
            vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
            memcpy(data, vertices, (size_t) bufferSize);
            vkUnmapMemory(device, stagingBufferMemory);

            BufferHandler::createBuffer(physicalDevice, device, bufferSize,
                                        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

            BufferHandler::copyBuffer(device, commandPool, graphicsQueue, stagingBuffer, vertexBuffer, bufferSize);

            vkDestroyBuffer(device, stagingBuffer, nullptr);
            vkFreeMemory(device, stagingBufferMemory, nullptr);
        }

        {
            VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;

            VkBuffer stagingBuffer;
            VkDeviceMemory stagingBufferMemory;
            BufferHandler::createBuffer(physicalDevice, device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                        stagingBuffer,
                                        stagingBufferMemory);

            void *data;
            vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
            memcpy(data, indices, (size_t) bufferSize);
            vkUnmapMemory(device, stagingBufferMemory);

            BufferHandler::createBuffer(physicalDevice, device, bufferSize,
                                        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                                        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

            BufferHandler::copyBuffer(device, commandPool, graphicsQueue, stagingBuffer, indexBuffer,
                                      bufferSize);

            vkDestroyBuffer(device, stagingBuffer, nullptr);
            vkFreeMemory(device, stagingBufferMemory, nullptr);
        }

        DebugUtils::setObjectName(vertexBuffer, "Mesh vertex buffer");
        DebugUtils::setObjectName(indexBuffer, "Mesh index buffer");
        DebugUtils::setObjectName(vertexBufferMemory, "Mesh vertex buffer memory");
        DebugUtils::setObjectName(indexBufferMemory, "Mesh index buffer memory");
    }

    void Mesh::cleanup(VkDevice device) const {
        vkDestroyBuffer(device, indexBuffer, nullptr);
        vkFreeMemory(device, indexBufferMemory, nullptr);

        vkDestroyBuffer(device, vertexBuffer, nullptr);
        vkFreeMemory(device, vertexBufferMemory, nullptr);
    }

    Mesh::Mesh() = default;
}