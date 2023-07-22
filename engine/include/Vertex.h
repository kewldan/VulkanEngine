#pragma once

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

namespace Engine {
    struct Vertex {
        glm::vec3 pos;
        glm::vec3 normal;
        glm::vec2 uv;

        static VkVertexInputBindingDescription getBindingDescription();

        static VkVertexInputAttributeDescription *getAttributeDescriptions(int *);
    };
}
